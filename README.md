# c_asm  
Linux 下 C / 汇编混合编程示例仓库

## 项目简介
本仓库通过 6 种不同实现方式，演示了如何在 Linux x86-64 环境下从“完全依赖 glibc”到“完全不依赖 libc”编写并运行程序，并附带一段 AVX2 指令集优化的图像处理示例，用于对比性能差异。

## 目录结构
```
.
├── pure_c.c          # 常规 C 实现（依赖 glibc）
├── pure_asm.c        # 纯汇编实现（_start 入口，无 libc）
├── mixed-v1.c        # 混合实现：自定义 _start + 系统调用，支持 argc/argv/envp
├── mixed-v2.c        # 混合实现：在 v1 基础上增加简易 printf 与 gettimeofday
├── compare_avx2.c    # 使用 AVX2 指令集加速的 RGB 图像块差异检测
├── build.sh          # 一键编译脚本
├── bin/              # 编译产物（可执行文件）
└── README.md         # 本文档
```

## 各文件功能速览
| 源文件 | 编译方式 | 特点 |
|---|---|---|
| [pure_c.c](pure_c.c) | `gcc -o bin/pure_c-v1 pure_c.c` | 最普通的 C 程序，使用 `syscall` 封装，依赖 glibc 启动代码。 |
| [pure_c.c](pure_c.c) | `gcc -static -o bin/pure_c-v2 pure_c.c` | 静态链接 glibc，单文件即可运行。 |
| [pure_asm.c](pure_asm.c) | `gcc -nostdlib -o bin/pure_asm pure_asm.c` | 仅用内联汇编完成 `sys_write` 与 `sys_exit`，无任何 libc 依赖。 |
| [mixed-v1.c](mixed-v1.c) | `gcc -nostdlib -fno-stack-protector -no-pie -o bin/mixed-v1 mixed-v1.c` | 自定义 `_start`，手动传递 `argc/argv/envp`；提供 `sys_write`、`itoa`、`my_printf` 等最小运行时。 |
| [mixed-v2.c](mixed-v2.c) | `gcc -nostdlib -fno-stack-protector -no-pie -o bin/mixed-v2 mixed-v2.c -lc` | 在 v1 基础上增加 `gettimeofday` 系统调用示例，并演示如何动态链接 libc（虽然入口仍自定义）。 |
| [compare_avx2.c](compare_avx2.c) | `gcc -mavx2 -O2 -o bin/compare_avx2 compare_avx2.c` | 使用 AVX2 指令集加速 1920×1200 RGB 图像的块差异检测，附带计时与结果输出。 |

## 快速开始
1. 克隆仓库  
   ```bash
   git clone <repo-url> && cd c_asm
   ```
2. 一键编译  
   ```bash
   bash build.sh
   ```
3. 运行示例  
   ```bash
   ./bin/pure_c-v1
   ./bin/pure_asm
   ./bin/mixed-v2
   ./bin/compare_avx2
   ```

## 编译脚本说明（[build.sh](build.sh)）
```bash
gcc -o bin/pure_c-v1 pure_c.c
gcc -static -o bin/pure_c-v2 pure_c.c
gcc -nostdlib -o bin/pure_asm pure_asm.c
gcc -nostdlib -fno-stack-protector -no-pie -o bin/mixed-v1 mixed-v1.c
gcc -nostdlib -fno-stack-protector -no-pie -o bin/mixed-v2 mixed-v2.c -lc
gcc -mavx2 -O2 -o bin/compare_avx2 compare_avx2.c
```
- `-nostdlib`：不链接任何启动文件及 libc。  
- `-static`：静态链接，生成无依赖可执行文件。  
- `-mavx2 -O2`：启用 AVX2 指令集并做 O2 优化。

## 依赖
- Linux x86-64 系统  
- GCC ≥ 7 或 Clang ≥ 6  
- CPU 支持 AVX2（仅运行 `compare_avx2` 时需要）

## 许可证
MIT License — 可自由使用、修改及分发
