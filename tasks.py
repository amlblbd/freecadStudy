from pathlib import Path
import json
import platform
import shutil

from invoke.tasks import task


ROOT = Path(__file__).parent.resolve()
CONFIG_DIR = ROOT / "config"
BUILD_DIR = ROOT / "build"
DIST_DIR = ROOT / "dist"


def _default_profile() -> Path:
    system = platform.system().lower()
    if system == "windows":
        return CONFIG_DIR / "windows.profile"
    if system == "linux":
        return CONFIG_DIR / "linux.profile"
    raise RuntimeError(f"Unsupported system: {system}")


def _conan_install(
    ctx,
    build_type: str,
    profile: Path,
    conan_conf: list[str] | None,
) -> None:
    build_dir = BUILD_DIR / f"{profile.stem}-{build_type.lower()}"
    build_dir.mkdir(parents=True, exist_ok=True)
    conf_args = ""
    if conan_conf:
        conf_args = " ".join(f'-c "{item}"' for item in conan_conf) + " "
    ctx.run(
        (
            f'conan install . -of "{build_dir}" '
            f'-pr:h "{profile}" -pr:b "{profile}" '
            f"{conf_args}"
            f"-s build_type={build_type} --build=missing"
        ),
        echo=True,
    )


def _load_preset_files() -> list[Path]:
    presets_path = ROOT / "CMakeUserPresets.json"
    if not presets_path.exists():
        return []
    data = json.loads(presets_path.read_text(encoding="utf-8"))
    files = [presets_path]
    for item in data.get("include", []):
        files.append((ROOT / item).resolve())
    return files


def _find_binary_dir(preset: str) -> Path:
    for path in _load_preset_files():
        if not path.exists():
            continue
        data = json.loads(path.read_text(encoding="utf-8"))
        for entry in data.get("configurePresets", []):
            if entry.get("name") == preset:
                return Path(entry["binaryDir"])
    raise RuntimeError(f"Preset not found: {preset}")


@task(iterable=["conan_conf"])
def deps(ctx, profile=None, conan_conf=None):
    # 生成 debug/release 的 Conan 输出（含 toolchain）
    profile_path = Path(profile) if profile else _default_profile()
    _conan_install(ctx, "Debug", profile_path, conan_conf)
    _conan_install(ctx, "Release", profile_path, conan_conf)


@task
def build(ctx, preset=None):
    # 配置并编译 debug/release
    if preset:
        ctx.run(f'cmake --preset "{preset}"', echo=True)
        ctx.run(f'cmake --build --preset "{preset}"', echo=True)
        return

    system = platform.system().lower()
    if system == "windows":
        presets = ["conan-debug", "conan-release"]
    elif system == "linux":
        presets = ["conan-debug", "conan-release"]
    else:
        raise RuntimeError(f"Unsupported system: {system}")

    for name in presets:
        ctx.run(f'cmake --preset "{name}"', echo=True)
        ctx.run(f'cmake --build --preset "{name}"', echo=True)


def _install_preset(ctx, preset: str, prefix: Path | None) -> None:
    install_dir = prefix if prefix else (DIST_DIR / preset)
    install_dir.mkdir(parents=True, exist_ok=True)
    binary_dir = _find_binary_dir(preset)
    ctx.run(
        f'cmake --install "{binary_dir}" --prefix "{install_dir}"',
        echo=True,
    )


@task
def package(ctx, preset=None, prefix=None):
    # 安装到 dist 目录，自动收集运行时依赖
    if preset:
        prefix_path = Path(prefix) if prefix else None
        _install_preset(ctx, preset, prefix_path)
        return

    system = platform.system().lower()
    if system in {"windows", "linux"}:
        presets = ["conan-debug", "conan-release"]
    else:
        raise RuntimeError(f"Unsupported system: {system}")

    for name in presets:
        _install_preset(ctx, name, None)


@task
def clean(ctx):
    # 清理生成目录
    if BUILD_DIR.exists():
        shutil.rmtree(BUILD_DIR)


@task
def deps_graph(ctx):
    ctx.run(
        (f"conan graph info . --format=html > graph.html"),
        echo=True,
    )
