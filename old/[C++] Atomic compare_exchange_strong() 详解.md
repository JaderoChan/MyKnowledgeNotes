# [C++] Atomic compare_exchange_strong() 详解

## 签名

```cpp
 bool compare_exchange_strong(T& expected, T desired,
                              std::memory_order order = std::memory_order_seq_cst);
```

## 作用

原子地执行 **CAS（Compare-And-Swap）**：

1. 如果 `*this == expected`，则将 `*this` 设为 `desired`，返回 `true`
2. 如果 `*this != expected`，则将 `expected` 更新为 `*this` 的当前值，返回 `false`

整个过程是**原子**的。

## 典型用法

```cpp
 std::atomic<int> val{1};
 int expected = 1;
 
 if (val.compare_exchange_strong(expected, 2)) {
     // 成功：val 从 1 变成了 2
 } else {
     // 失败：expected 现在是 val 的实际值
 }
```

## 与 `compare_exchange_weak` 的区别

`weak` 版本可能**虚假失败**（即使值相等也可能返回 false），适合用在循环中；`strong` 保证不会虚假失败。