filePath: e:\freeCAD\codes\FreeCAD\src\Mod\PartDesign\App\FeatureAddSub.h
          
基于对FreeCAD PartDesign模块的深入分析，我为您总结参数化零件设计模块的工作原理、流程和类图架构：

## PartDesign 参数化零件设计模块

### 工作原理

PartDesign模块基于**特征建模**和**参数化设计**理念，采用以下核心技术架构：

1. **基于Body的建模容器**  
   - <mcsymbol name="Body" filename="Body.h" path="src/Mod/PartDesign/App/Body.h" startline="50" type="class">Body类</mcsymbol>作为所有特征的容器，管理特征的历史顺序
   - 每个Body维护一个特征列表，确保建模过程的有序性

2. **特征继承体系**  
   - <mcsymbol name="Feature" filename="Feature.h" path="src/Mod/PartDesign/App/Feature.h" startline="55" type="class">PartDesign::Feature</mcsymbol>：所有特征的基类
   - <mcsymbol name="FeatureAddSub" filename="FeatureAddSub.h" path="src/Mod/PartDesign/App/FeatureAddSub.h" startline="25" type="class">FeatureAddSub</mcsymbol>：处理加减材料操作
   - <mcsymbol name="ProfileBased" filename="FeatureSketchBased.h" path="src/Mod/PartDesign/App/FeatureSketchBased.h" startline="30" type="class">ProfileBased</mcsymbol>：基于草图的特征

3. **工作流管理**  
   - <mcsymbol name="WorkflowManager" filename="WorkflowManager.h" path="src/Mod/PartDesign/Gui/WorkflowManager.h" startline="45" type="class">WorkflowManager</mcsymbol>控制Legacy和Modern两种工作模式
   - Modern模式支持Body和Part概念，Legacy模式为向后兼容

### 工作流程

```cpp
// 参数化建模流程
1. 创建Body容器
2. 添加基础特征（如AdditiveBox）
3. 基于草图创建特征（Pad/Pocket）
4. 应用变换特征（LinearPattern/PolarPattern）
5. 特征自动重新计算
```

### 核心类图架构

```
Part::Feature (Part模块)
    ↓
PartDesign::Feature (核心特征基类)
    ├── FeatureAddSub (加减材料特征)
    │   ├── FeatureAdditive (添加材料)
    │   └── FeatureSubtractive (移除材料)
    ├── ProfileBased (基于草图特征)
    │   ├── Pad (拉伸)
    │   ├── Pocket (挖槽)
    │   └── Revolution (旋转)
    └── Transformed (变换特征)
        ├── LinearPattern (线性阵列)
        └── PolarPattern (环形阵列)
```

### 关键特性

1. **自动重新计算**：修改参数后所有依赖特征自动更新
2. **历史记录管理**：支持特征回滚和重新排序
3. **草图关联**：基于2D草图创建3D特征
4. **布尔运算**：支持复杂的加减材料操作
5. **模式变换**：线性/环形阵列等批量特征创建

PartDesign模块通过参数化特征树实现了强大的历史记录管理和自动更新机制，是FreeCAD中专业的零件设计解决方案。
        
