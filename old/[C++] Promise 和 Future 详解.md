# [C++] Promise 和 Future 详解

## 一、`std::promise<T>`：生产者端（写结果）

`promise` 用来在某个线程里“填结果”，让关联的 `future` 在另一端“取结果”。

### 常用成员函数

- `promise()`
  - 默认构造，一个空的共享状态“写端”。
- `promise(promise&&)` / `operator=(promise&&)`
  - 只支持移动，不支持拷贝。
- `~promise()`
  - 若析构时还没设置结果/异常，且有 `future` 在等，会让 `future.get()` 抛 `std::future_error`（`broken_promise`）。
- `future<T> get_future()`
  - 获取与该 promise 绑定的 `future`（只能调用一次）。
  - 第二次调用会抛 `future_error`（`future_already_retrieved`）。
- `void set_value(const T&)` / `void set_value(T&&)` / `void set_value()`（`T=void`）
  - 设置正常结果，唤醒等待方。
  - 只能设置一次；重复设置抛异常（`promise_already_satisfied`）。
- `void set_exception(std::exception_ptr p)`
  - 设置异常结果；`future.get()` 会重抛该异常。
  - 与 `set_value` 二选一，也只能一次。
- `void set_value_at_thread_exit(...)`
- `void set_exception_at_thread_exit(...)`
  - 不立即就绪；在线程退出时才把共享状态标记为 ready。
- `void swap(promise& other) noexcept`
  - 交换内部状态。

------

## 二、`std::future<T>`：消费者端（读结果）

`future` 用来等待并获取异步结果。

### 常用成员函数

- `future() noexcept`
  - 默认构造，通常是无效 future。
- `future(future&&)` / `operator=(future&&)`
  - 只支持移动，不支持拷贝。
- `~future()`
  - 析构本身不抛异常。
  - 若它来自 `std::async(std::launch::async, ...)`，析构可能阻塞等待任务结束（常见坑点）。
- `bool valid() const noexcept`
  - 判断是否绑定有效共享状态。
  - 调用 `get()` 后通常变为无效。
- `T get()` / `void get()`（`T=void`）
  - 阻塞直到 ready，然后取结果。
  - 结果若是异常，会重抛。
  - **只能调用一次**（调用后 future 失效）。
- `void wait() const`
  - 阻塞到 ready，不取值。
- `future_status wait_for(duration)`
  - 最多等一段时间，返回：
    - `future_status::ready`
    - `future_status::timeout`
    - `future_status::deferred`（延迟执行任务尚未运行）
- `future_status wait_until(time_point)`
  - 等到指定时间点，返回同上。
- `shared_future<T> share()`
  - 把当前 `future` 转为 `shared_future`（可多次 `get()`、可拷贝给多个消费者）。
  - 调用后原 `future` 失效。

------

## 三、常见配对关系

- `promise.set_value / set_exception` ↔ `future.get`
- `promise` 端只写一次，`future` 端只读一次（除非转 `shared_future`）

------

## 四、易错点速记

1. `promise.get_future()` 只能一次。
2. `promise` 结果只能设置一次（值或异常）。
3. `future.get()` 只能一次。
4. `promise` 提前销毁未设置结果，会产生 `broken_promise`。
5. 需要多消费者时用 `shared_future`。