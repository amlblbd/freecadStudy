#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QtPlugin>

// 说明：在某些 Conan 的 Qt 构建方式里，平台插件（platforms/xcb 等）会以“静态库”形式提供
// Qt 默认的行为是运行时从 QT_PLUGIN_PATH 下加载动态插件（.so），这会导致：
//   Could not find the Qt platform plugin "xcb"
// 因此这里用 Q_IMPORT_PLUGIN 显式把插件静态注册进程序。
class QXcbIntegrationPlugin;
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)

int main(int argc, char** argv) {
    // 运行环境选择：
    // - 没有 DISPLAY/WAYLAND_DISPLAY 时（比如纯命令行/CI），强制使用 offscreen，避免初始化图形系统失败
    // - 有图形环境时，默认用 xcb（如果你想用 wayland，可以在环境变量 QT_QPA_PLATFORM 里显式设置）
    if (qEnvironmentVariableIsEmpty("DISPLAY") && qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY")) {
        qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    } else if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
    }

    // QApplication：Qt Widgets 程序必须使用它（而不是 QCoreApplication）
    QApplication app(argc, argv);

    // 打印“运行时版本”和“编译期版本”：
    // - runtime：实际链接/加载到的 Qt 版本
    // - compile-time：编译时使用的头文件版本
    // 两者都为 6.6.2 说明你确实在用 Conan 的 Qt 6.6.2
    qInfo() << "Qt runtime version:" << qVersion();
    qInfo() << "Qt compile-time version:" << QT_VERSION_STR;

    // 最小窗口：能显示出来（或 offscreen 下成功初始化）就说明 Widgets + 平台插件链路正常
    QLabel label("Qt 6.6.2 OK");
    label.resize(320, 80);
    label.show();

    // 自动退出：避免测试程序需要手动关闭窗口
    // QTimer::singleShot(50, &app, &QCoreApplication::quit);

    return app.exec();
}
