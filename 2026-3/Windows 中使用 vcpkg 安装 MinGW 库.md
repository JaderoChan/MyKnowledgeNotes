# Windows 中使用 vcpkg 安装 MinGW 库

在 Windows 下使用 vcpkg 安装 MinGW（GCC）的包，需要指定 Triplet（三元组）为 `x64-mingw-dynamic` 或 `x64-mingw-static`（取决于是否需要静态链接）。以下是详细步骤：

---

## 1. 安装 MinGW 工具链

确保已安装 MinGW-w64（如 [MSYS2](https://www.msys2.org/) 提供的 GCC）：

```sh
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
```

并确保 `gcc`、`g++`、`make` 等工具在 `PATH` 环境变量中。

---

## 2. 设置 vcpkg 的默认 Triplet

### 方法 1：全局设置（推荐）

在 `vcpkg` 安装目录下创建或修改 `triplets/x64-mingw-dynamic.cmake` 文件：

```cmake
# triplets/x64-mingw-dynamic.cmake
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)  # 动态链接
set(VCPKG_ENV_PREFIX "MINGW")

# 指定 MinGW 工具链
set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
set(CMAKE_MAKE_PROGRAM "mingw32-make")
```

### 方法 2：临时指定 Triplet

安装包时直接指定 Triplet：

```sh
vcpkg install zlib:x64-mingw-dynamic
```

---

## 3. 安装 MinGW 版本的包

```sh
# 动态链接
vcpkg install zlib:x64-mingw-dynamic

# 静态链接（使用 x64-mingw-static）
vcpkg install zlib:x64-mingw-static
```

---

## 4. 在 CMake 中使用 MinGW 包

### 方法 1：通过工具链文件

在 `CMakeLists.txt` 中指定 vcpkg 工具链和 Triplet：

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 指定 vcpkg 工具链和 Triplet
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic" CACHE STRING "VCPKG triplet")

find_package(ZLIB REQUIRED)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ZLIB::ZLIB)
```

### 方法 2：命令行配置

```sh
cmake -B build \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
```

---

## 5. 验证安装

检查包的架构和编译器：

```sh
vcpkg list
# 输出示例：
zlib:x64-mingw-dynamic         1.2.11#9  MinGW-built zlib library
```

---

## 6. 常见问题

### Q1：找不到 MinGW 编译器

- **错误**：`Could not find compiler "x86_64-w64-mingw32-g++" in PATH`
- **解决**：确保 MinGW 的 `bin` 目录（如 `C:\msys64\mingw64\bin`）已添加到 `PATH`。

### Q2：链接失败

- **错误**：`undefined reference to...`
- **解决**：确保 Triplet 的 `VCPKG_LIBRARY_LINKAGE` 与项目一致（动态/静态）。

### Q3：vcpkg 不支持 MinGW Triplet

- **解决**：

  更新 vcpkg 至最新版本：

  ```sh
  git pull
  ./bootstrap-vcpkg.sh
  ```

---

## 7. 扩展：自定义 MinGW Triplet

若需进一步定制（如调试版本），复制默认 Triplet 并修改：

```sh
cp triplets/x64-mingw-dynamic.cmake triplets/x64-mingw-debug.cmake
```

编辑 `x64-mingw-debug.cmake`：

```cmake
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_BUILD_TYPE debug)  # 仅构建调试版本
```

---

通过以上步骤，即可在 Windows 下使用 vcpkg 管理 MinGW 的依赖库。关键点是正确设置 Triplet 和确保 MinGW 工具链可用。
