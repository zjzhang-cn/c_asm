#include <sys/syscall.h>

// 纯汇编实现，完全不依赖C库
void _start() {
    // 定义要打印的字符串
    const char message[] = "Hello World from pure assembly!\n";
    
    // 使用内联汇编调用 sys_write
    asm volatile (
        "movq $1, %%rax\n\t"        // SYS_write = 1
        "movq $1, %%rdi\n\t"        // stdout = 1
        "movq %0, %%rsi\n\t"        // message地址
        "movq $33, %%rdx\n\t"       // 字符串长度
        "syscall\n\t"               // 执行系统调用
        :
        : "r" (message)
        : "rax", "rdi", "rsi", "rdx", "memory"
    );
    
    // 调用 sys_exit 退出程序
    asm volatile (
        "movq $60, %%rax\n\t"       // SYS_exit = 60
        "movq $0, %%rdi\n\t"        // exit code = 0
        "syscall\n\t"               // 执行系统调用
        :
        :
        : "rax", "rdi"
    );
}
