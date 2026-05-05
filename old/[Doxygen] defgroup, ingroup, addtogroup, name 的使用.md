# [Doxygen] defgroup, ingroup, addtogroup 的使用

在 Doxygen 里，`\\defgroup`、`\\ingroup`、`\\addtogroup` 都是**分组文档**的核心命令，用来把函数/类/文件组织成模块。

## 1) `\defgroup`：定义一个新组

用于**首次创建组**（组ID + 标题 + 描述）。

- 语法：`\defgroup <group_id> <title>`
- `<group_id>`：内部唯一标识（不能重复）
- `<title>`：文档里显示的组名

```cpp
/**
 * \defgroup io_module IO 模块
 * \brief 输入输出相关功能
 * 这里是模块详细说明。
 */
```

---

## 2) `\ingroup`：把当前实体加入已有组

用于将**函数、类、结构体、文件、命名空间等**归入某个组。

- 可写多个组：`\ingroup g1 g2`
- 常见放在注释块里，紧贴目标实体

```cpp
/**
 * \ingroup io_module
 * \brief 打开设备
 */
bool openDevice(const char* path);
```

---

## 3) `\addtogroup`：给已有组补充说明/内容（可分散定义）

用于在不同文件中“继续写同一个组”的文档，避免 `\defgroup` 重复定义报错。

- 语法与 `\defgroup` 类似
- 如果组不存在，会创建；若存在，则追加内容
- 常配合 `{` `}` 把一批声明归组（块分组）

```cpp
/**
 * \addtogroup io_module
 * @{
 */

/** \brief 关闭设备 */
void closeDevice();

/** \brief 读取数据 */
int readData(void* buf, int len);

/** @} */
```

---

## 三者区别速记

- `\defgroup`：**新建组（通常一次）**
- `\ingroup`：**成员声明“我属于哪个组”**
- `\addtogroup`：**在别处给这个组继续加文档或成员**

---

## 推荐用法（实践）

1. 在一个“主入口”文件用 `\defgroup` 定义组。
2. 各函数/类用 `\ingroup` 精确归类。
3. 跨文件补充组说明时用 `\addtogroup`。
4. 避免多个地方重复 `\defgroup` 同一 `group_id`。

**同一个成员可以同时属于多个组**。

最直接做法是在同一条 `\ingroup` 里写多个组 ID：

```cpp
/**
 * \ingroup core io_module
 * \brief 初始化并打开设备
 */
bool initAndOpen();
```

也可以用“块分组”方式，把一批成员同时加入多个组：

```cpp
/**
 * \ingroup core io_module
 * @{
 */

void start();
void stop();

/** @} */
```

要点：

- 组 ID 之间用空格分隔。
- 这些组需要已通过 `\defgroup`/`\addtogroup` 定义过（或将被创建）。

## 命名建议

推荐固定模式：`模块_类名_主题`

例如：

- `hid_device_lifecycle`
- `hid_device_io`
- `hid_parser_lifecycle`
- `hid_parser_io`

这样即使不同类都有“lifecycle/io”分组，也不会冲突。

---

## 符号限制（建议遵循）

Doxygen 对组ID本质是“单个标记”，实践中建议：

- ✅ `a-z A-Z 0-9 _`
- ❌ 空格、制表符
- ⚠️ 尽量避免 `- . : / \` 等符号（可读性和兼容性都不如下划线）

## 4) `\name`：起分组标题

在 Doxygen 里，`@name`（或 `\name`）用于给**一组成员起一个分组标题**，常用于类/结构体内部把相关函数归类显示。

### 作用

- 在文档中生成一个小节标题（如“clickKey 函数”）。
- 配合 `@{` 和 `@}` 把后续成员归到该小节。
- 这是**局部分组**，不需要像 `@defgroup` 那样维护全局唯一组 ID。

### 常见写法

```cpp
/**
 * @name clickKey 函数
 * @brief 等同于 press + release
 * @{
 */
bool clickKey(uint32_t nativeKey);
bool clickKey(KeyboardKey key);
/** @} */
```

简言之：`@name` 适合“类内分组展示”；`@defgroup` 适合“全局模块分组”。

1. **一个函数不建议/基本不能同时属于多个 `@name` 组**。
    `@name` 是“就地分组”（按代码块归组），不是像 `@ingroup` 那样可多归属的标签机制。

2. **不能像 `\addtogroup` 那样把同一个 `@name` 组在多处稳定地“追加”成员**。
    `@name` 更适合在同一处连续写：
    `@name` + `@{` ...成员... `@}`。

---

如果你需要：

- 一个函数进**多个组**，或
- 在**不同文件/不同位置**分散归组，

请用 `\defgroup + \ingroup`（必要时 `\addtogroup`），不要用 `@name`。
