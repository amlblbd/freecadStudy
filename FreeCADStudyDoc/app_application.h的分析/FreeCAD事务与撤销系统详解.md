
我来详细解释FreeCAD的事务与撤销系统。让我先查看相关的实现代码。
        
            
toolName: search_codebase
            
status: success
          
query: FreeCAD transaction system undo redo openTransaction commitTransaction abortTransaction basic tutorial
          


​            
toolName: view_files
​            
status: success
​          
​            
filePath: e:\freeCAD\codes\FreeCAD\src\App\Document.pyi
​          


​            
toolName: view_files
​            
status: success
​          
​            
filePath: e:\freeCAD\codes\FreeCAD\src\Mod\Test\Document.py
​          


​            
toolName: search_codebase
​            
status: success
​          
query: FreeCAD transaction system internal implementation undo redo stack memory management Document.h
​          


​            
toolName: search_codebase
​            
status: success
​          
query: FreeCAD Document.h Document.cpp transaction system implementation undo redo stack memory management
​          


​            
toolName: view_files
​            
status: success
​          
​            
filePath: e:\freeCAD\codes\FreeCAD\src\App\Transactions.h
​          
基于我对FreeCAD源代码的分析，我来详细解释FreeCAD的事务与撤销系统：

## FreeCAD事务与撤销系统详解

### 什么是事务与撤销系统？

事务与撤销系统是FreeCAD中用于**记录、管理和回滚文档更改**的核心机制。它允许用户：
- **撤销（Undo）** 之前的操作
- **重做（Redo）** 被撤销的操作
- **分组管理** 相关操作
- **数据一致性保护**

### 核心概念

#### 1. 事务（Transaction）
事务是**一组原子操作**的集合，要么全部成功，要么全部失败：

```python
# 从事务代码示例（来自Document.pyi）
def openTransaction(self, name: str) -> None:
    """
    打开一个新的撤销/重做事务
    当任何打开的文档中发生更改时，将自动创建事务
    """
```

#### 2. 事务栈（Transaction Stack）
FreeCAD维护两个栈：
- **撤销栈**：存储已提交的事务
- **重做栈**：存储已撤销的事务

#### 3. 事务状态
```python
# 从测试代码中可以看到事务状态管理
self.Doc.UndoMode = 1  # 开启撤销功能
self.Doc.UndoNames     # 撤销事务名称列表
self.Doc.UndoCount      # 撤销事务数量
self.Doc.RedoNames      # 重做事务名称列表  
self.Doc.RedoCount      # 重做事务数量
```

### 基本使用方法

#### 1. 开启事务
```python
# 开启一个名为"Create Box"的事务
FreeCAD.ActiveDocument.openTransaction("Create Box")
```

#### 2. 执行操作
```python
# 在事务中执行操作
box = FreeCAD.ActiveDocument.addObject("Part::Box", "MyBox")
box.Length = 10
box.Width = 20
box.Height = 30
```

#### 3. 提交事务
```python
# 提交事务，操作被记录到撤销栈
FreeCAD.ActiveDocument.commitTransaction()
```

#### 4. 撤销操作
```python
# 撤销最近的事务
FreeCAD.ActiveDocument.undo()
```

#### 5. 重做操作
```python
# 重做最近被撤销的事务
FreeCAD.ActiveDocument.redo()
```

### 高级功能

#### 1. 中止事务（回滚）
```python
# 中止当前事务，放弃所有更改
FreeCAD.ActiveDocument.abortTransaction()
```

#### 2. 清除撤销历史
```python
# 清空撤销栈，释放内存
FreeCAD.ActiveDocument.clearUndos()
```

#### 3. 多文档事务同步
```python
# 多个文档的事务可以具有相同的内部ID
# 这样它们可以一起被撤销/重做
```

### 内部实现机制

#### 1. 事务对象结构
从<mcfile name="Transactions.h" path="src/App/Transactions.h"></mcfile>可以看到：

```cpp
class Transaction: public Base::Persistence
{
    std::string Name;           // 事务名称
    int transID;                // 事务ID
    // 存储对象更改记录
    std::unordered_map<...> _Objects;
};
```

#### 2. 事务记录类型
事务可以记录：
- **对象新增** (`addObjectNew`)
- **对象删除** (`addObjectDel`) 
- **对象修改** (`addObjectChange`)
- **属性重命名** (`renameProperty`)
- **属性增删** (`addOrRemoveProperty`)

#### 3. 应用机制
```cpp
void apply(Document& Doc, bool forward);
// forward=true: 应用事务（正常操作）
// forward=false: 撤销事务（回退操作）
```

### 实际应用示例

#### 1. 创建几何体
```python
FreeCAD.ActiveDocument.openTransaction("Create Cylinder")
try:
    cylinder = FreeCAD.ActiveDocument.addObject("Part::Cylinder", "Cylinder")
    cylinder.Radius = 5
    cylinder.Height = 10
    FreeCAD.ActiveDocument.commitTransaction()
except:
    FreeCAD.ActiveDocument.abortTransaction()
```

#### 2. 修改属性
```python
FreeCAD.ActiveDocument.openTransaction("Change Color")
obj.ViewObject.ShapeColor = (1.0, 0.0, 0.0)  # 红色
FreeCAD.ActiveDocument.commitTransaction()
```

#### 3. 复杂操作
```python
# 从事务测试代码可以看到复杂操作的处理
FreeCAD.ActiveDocument.openTransaction("Complex Operation")
obj.Integer = 1
obj.String = "test"
obj.Float = 1.0
obj.Bool = True
FreeCAD.ActiveDocument.commitTransaction()
```

### 设计优势

1. **用户友好**：直观的撤销/重做操作
2. **内存效率**：智能的事务管理
3. **数据一致性**：原子性操作保证
4. **可扩展性**：支持自定义事务类型
5. **多文档支持**：跨文档事务同步

### 总结

FreeCAD的事务与撤销系统是一个**强大而灵活**的机制，它不仅提供了基本的撤销重做功能，还通过事务分组、多文档同步等高级特性，确保了用户操作的**安全性**和**便利性**。这个系统是FreeCAD能够提供专业级CAD体验的重要基础。
        