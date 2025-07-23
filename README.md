# c_asm

本项目包含多种实现方式的示例代码，主要用于对比纯C、纯汇编、混合实现以及AVX2指令集优化的性能。

## 目录结构

- `pure_c.c`：纯C语言实现。
- `pure_asm.c`：纯汇编实现。
- `mixed-v1.c`,`mixed-v2.c` ：C与汇编混合实现，展示了如何直接用系统调用实现最小化C运行环境，包括自定义 `_start`、`my_printf`、`sys_write` 等。
- `compare_avx2/` 和 `compare_avx2.c`：使用AVX2指令集优化的实现。
- `build.sh`：编译脚本。

## mixed-v1.c mixed-v2.c 说明

- 该文件展示了如何在不依赖C运行库的情况下，直接通过系统调用实现程序入口（`_start`）、参数传递、输出等。
- 包含自定义的 `my_printf`（仅支持 `%s` 和 `%d`）、`itoa`、`sys_write` 等函数。
- 通过内联汇编实现系统调用，适合学习 Linux 下底层程序启动流程。

## 使用方法

1. 运行 `build.sh` 进行编译：
   ```bash
   bash build.sh
   ```
2. 根据编译结果运行对应的可执行文件，测试不同实现的性能。

## 依赖

- GCC 或 Clang 编译器
- 支持AVX2指令集的CPU（如需测试AVX2相关代码）

## 许可证

本项目采用 MIT 许可证。
