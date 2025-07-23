#include <sys/time.h>
#include <stdarg.h>

// 系统调用写入 stdout
long sys_write(int fd, const void *buf, unsigned long count)
{
	long ret;
	asm volatile(
		"movq $1, %%rax\n\t" // syscall number for write
		"movq %1, %%rdi\n\t" // fd
		"movq %2, %%rsi\n\t" // buf
		"movq %3, %%rdx\n\t" // count
		"syscall\n\t"
		"movq %%rax, %0\n\t"
		: "=r"(ret)
		: "r"((long)fd), "r"(buf), "r"(count)
		: "rax", "rdi", "rsi", "rdx", "memory");
	return ret;
}

// 整数转字符串（十进制）
void itoa(long val, char *buf)
{
	char tmp[20];
	int i = 0, j = 0;
	int neg = val < 0;
	if (neg)
		val = -val;

	do
	{
		tmp[i++] = '0' + (val % 10);
		val /= 10;
	} while (val);

	if (neg)
		tmp[i++] = '-';

	while (i--)
		buf[j++] = tmp[i];
	buf[j] = '\0';
}

// 简易 printf，仅支持 %s 和 %d
void my_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char ch;
	while ((ch = *fmt++))
	{
		if (ch == '%' && *fmt)
		{
			char next = *fmt++;
			if (next == 's')
			{
				char *str = va_arg(args, char *);
				sys_write(1, str, __builtin_strlen(str));
			}
			else if (next == 'd')
			{
				char numbuf[32];
				itoa(va_arg(args, int), numbuf);
				sys_write(1, numbuf, __builtin_strlen(numbuf));
			}
			else
			{
				sys_write(1, &next, 1);
			}
		}
		else
		{
			sys_write(1, &ch, 1);
		}
	}

	va_end(args);
}

long get_time(struct timeval *tv)
{
	long syscall_num = 96; // SYS_gettimeofday
	long result;
	// 使用 inline assembly 调用 gettimeofday 系统调用
	asm volatile(
		"movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq $0, %%rsi\n\t" // timezone 参数设为 NULL
		"syscall\n\t"
		"movq %%rax, %0\n\t"
		: "=r"(result)
		: "r"(syscall_num), "r"(tv)
		: "rax", "rdi", "rsi", "memory");
	return result;
}
int main(int argc, char **argv, char **envp)
{
	int ret = 0;
	for (int i = 0; i < argc; ++i)
	{
		my_printf("argv[%d]: %s\n", i, argv[i]);
	}

	for (char **env = envp; *env != 0; env++)
	{
		my_printf("env: %s\n", *env);
		// 打印环境变量
	}
	struct timeval tv;
	get_time(&tv);
	my_printf("Hello World ! Current time: %d seconds and %d microseconds since epoch\n", tv.tv_sec, tv.tv_usec);
	return ret;
}
__attribute__((naked)) void _start()
{
	// 跳转到 main 函数并传递 argc, argv, envp 作为参数
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
