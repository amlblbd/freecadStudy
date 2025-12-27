filePath: e:\freeCAD\codes\FreeCAD\src\Mod\Assembly\App\CMakeLists.txt
          
基于对FreeCAD Assembly模块的深入分析，我为您总结装配模块的工作原理、流程和类图架构：

## Assembly 装配模块

### 工作原理

Assembly模块基于**约束驱动装配**和**多体动力学求解**理念，采用以下核心技术架构：

1. **装配容器管理**  
   - <mcsymbol name="AssemblyObject" filename="AssemblyObject.h" path="src/Mod/Assembly/App/AssemblyObject.h" startline="60" type="class">AssemblyObject</mcsymbol>作为装配体的核心容器，继承自App::Part
   - 管理零件、约束、求解器和仿真数据

2. **关节约束系统**  
   - <mcsymbol name="JointGroup" filename="JointGroup.h" path="src/Mod/Assembly/App/JointGroup.h" startline="30" type="class">JointGroup</mcsymbol>管理所有装配约束
   - 支持13种关节类型：Fixed、Revolute、Cylindrical、Slider、Ball、Distance等

3. **多体动力学求解**  
   - 集成OndselSolver进行复杂装配求解
   - 支持运动仿真和拖拽交互

### 工作流程

```cpp
// 装配建模流程
1. 创建Assembly容器
2. 插入零件（PartDesign Body或App::Link）
3. 创建关节约束（Joint）
4. 求解装配关系
5. 运动仿真分析
```

### 核心类图架构

```
App::Part (Part模块)
    ↓
AssemblyObject (装配容器)
    ├── JointGroup (约束管理)
    ├── BomObject (物料清单)
    ├── ViewGroup (爆炸视图)
    └── SimulationGroup (仿真管理)

JointObject (关节对象)
    ├── 坐标系定义 (JCS)
    ├── 约束参数 (距离/角度)
    └── 限制条件 (最小/最大值)
```

### 支持的关节类型

从<mcfile name="JointObject.py" path="src/Mod/Assembly/JointObject.py"></mcfile>分析，支持13种关节：

1. **Fixed** - 固定连接
2. **Revolute** - 旋转关节  
3. **Cylindrical** - 圆柱关节
4. **Slider** - 滑动关节
5. **Ball** - 球关节
6. **Distance** - 距离约束
7. **Parallel** - 平行约束
8. **Perpendicular** - 垂直约束
9. **Angle** - 角度约束
10. **RackPinion** - 齿轮齿条
11. **Screw** - 螺旋关节
12. **Gears** - 齿轮传动
13. **Belt** - 皮带传动

### 关键特性

1. **自动求解**：基于OndselSolver的多体动力学求解
2. **实时交互**：支持拖拽零件进行动态装配
3. **运动仿真**：生成装配体的运动序列
4. **爆炸视图**：可视化装配结构和关系
5. **物料清单**：自动生成BOM表
6. **约束管理**：灵活的约束创建和编辑

Assembly模块通过专业的约束求解引擎实现了复杂的机械装配建模，是FreeCAD中面向机械设计的重要功能模块。
        
