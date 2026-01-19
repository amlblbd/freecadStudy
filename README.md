# freecadStudy
本项目是freecad的学习验证项目，参考freecad的架构和技术方案，实现草图、零件、装配功能的验证。
本项目图形界面使用qt6，建模使用occt库，装配求解器使用OndselSolver
```
📁 project
├── 📄 README.md
├── 📁 data/                      # 数据目录
│   └── 📄 .gitignore
├── 📁 src/                       # 源代码目录
│   ├── 📁 App/                   # 应用层
│   │   └── 📄 .gitignore
│   ├── 📁 Base/                  # 基础库层
│   │   └── 📄 .gitignore
│   ├── 📁 Gui/                   # 图形界面层，qt6实现
│   │   └── 📄 .gitignore
│   ├── 📁 Mod/                   # 模块目录
│   │   ├── 📁 Assembly/         # 装配模块，使用OndselSolver求解
│   │   │   └── 📄 .gitignore
│   │   ├── 📁 Part/             # 零件模块，使用occt
│   │   │   └── 📄 .gitignore
│   │   └── 📁 Sketcher/         # 草图模块
│   │       └── 📄 .gitignore
│   └── 📁 Solver/                # 求解器模块，封装OndselSolver求解器
│       └── 📄 .gitignore
└── 📁 tests/                     # 测试目录，引入gtest
    └── 📄 .gitignore
```
