// 使用内联汇编直接调用系统调用打印Hello World

#include <sys/time.h>
#include <unistd.h>
#define _EXIT(exit_code)                     \
	long _syscall_num = 60;                  \
	long _exit_code = (long)(exit_code);     \
	asm volatile(                            \
		"movq %0, %%rax\n\t"                 \
		"movq %1, %%rdi\n\t"                 \
		"syscall\n\t"                        \
		:                                    \
		: "r"(_syscall_num), "r"(_exit_code) \
		: "rax", "rdi");

#include <stdarg.h>

// 系统调用写入 stdout
long sys_write(int fd, const void *buf, unsigned long count) {
    long ret;
    asm volatile(
        "movq $1, %%rax\n\t"     // syscall number for write
        "movq %1, %%rdi\n\t"     // fd
        "movq %2, %%rsi\n\t"     // buf
        "movq %3, %%rdx\n\t"     // count
        "syscall\n\t"
        "movq %%rax, %0\n\t"
        : "=r"(ret)
        : "r"((long)fd), "r"(buf), "r"(count)
        : "rax", "rdi", "rsi", "rdx", "memory"
    );
    return ret;
}

// 整数转字符串（十进制）
void itoa(long val, char *buf) {
    char tmp[20];
    int i = 0, j = 0;
    int neg = val < 0;
    if (neg) val = -val;

    do {
        tmp[i++] = '0' + (val % 10);
        val /= 10;
    } while (val);

    if (neg) tmp[i++] = '-';

    while (i--) buf[j++] = tmp[i];
    buf[j] = '\0';
}

// 简易 printf，仅支持 %s 和 %d
void my_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char ch;
    while ((ch = *fmt++)) {
        if (ch == '%' && *fmt) {
            char next = *fmt++;
            if (next == 's') {
                char *str = va_arg(args, char *);
                sys_write(1, str, __builtin_strlen(str));
            } else if (next == 'd') {
                char numbuf[32];
                itoa(va_arg(args, int), numbuf);
                sys_write(1, numbuf, __builtin_strlen(numbuf));
            } else {
                sys_write(1, &next, 1);
            }
        } else {
            sys_write(1, &ch, 1);
        }
    }

    va_end(args);
}


int main(int argc, char **argv)
{
	int ret = 0;
	for (int i = 0; i < argc; ++i)
	{
		my_printf("Hello %s! The answer is %d.\n", "World", 42);
	}

	struct timeval tv;
    gettimeofday(&tv, 0);
	return ret;
}

__attribute__((naked)) void _start()
{
	__asm__ volatile(
		"mov (%rsp), %rdi\n"		// argc -> rdi
		"lea 8(%rsp), %rsi\n"		// argv -> rsi
		"lea (%rsi,%rdi,8), %rdx\n" // envp -> rdx
		"add $8, %rdx\n"			// 跳过NULL
		"call main\n"
		"mov %rax, %rdi\n"
		"mov $60, %rax\n"
		"syscall\n");
}
// 直接调用系统调用
