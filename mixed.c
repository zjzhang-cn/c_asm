// 使用内联汇编直接调用系统调用打印Hello World

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

long print(const char *message, long length);

int main(int argc, char **argv);
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
int main(int argc, char **argv)
{
	int ret = 0;
	for (int i = 0; i < argc; ++i)
	{
		print("Hello World\n", 12);
	}
	ret = (int)print("Hello World\n", 12);
	return ret;
}

long print(const char *message, long length)
{
	long syscall_num = 1; // SYS_write
	long fd = 1;		  // stdout
	long result;

	// x86_64 系统调用约定：
	// rax = 系统调用号, rdi = 参数1, rsi = 参数2, rdx = 参数3
	asm volatile(
		"movq %1, %%rax\n\t" // 系统调用号放入rax
		"movq %2, %%rdi\n\t" // fd放入rdi
		"movq %3, %%rsi\n\t" // message放入rsi
		"movq %4, %%rdx\n\t" // length放入rdx
		"syscall\n\t"		 // 执行系统调用
		"movq %%rax, %0\n\t" // 返回值放入result
		: "=r"(result)
		: "r"(syscall_num), "r"(fd), "r"(message), "r"(length)
		: "rax", "rdi", "rsi", "rdx", "memory");
	return result;
}
// 直接调用系统调用
