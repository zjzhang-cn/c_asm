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

void print_hello_world();

int main(int argc, char **argv);
void _start()
{
	long result = 0;
	print_hello_world();
	result = main(0, 0);
	_EXIT(result);
}
int main(int argc, char **argv)
{
	print_hello_world();
	print_hello_world();
	print_hello_world();
	return 1;
}

void print_hello_world()
{
	const char *message = "Hello World\n";
	long syscall_num = 1; // SYS_write
	long fd = 1;		  // stdout
	long count = 12;	  // 字符串长度
	long result;

	// x86_64 系统调用约定：
	// rax = 系统调用号, rdi = 参数1, rsi = 参数2, rdx = 参数3
	asm volatile(
		"movq %1, %%rax\n\t" // 系统调用号放入rax
		"movq %2, %%rdi\n\t" // fd放入rdi
		"movq %3, %%rsi\n\t" // message放入rsi
		"movq %4, %%rdx\n\t" // count放入rdx
		"syscall\n\t"		 // 执行系统调用
		"movq %%rax, %0\n\t" // 返回值放入result
		: "=r"(result)
		: "r"(syscall_num), "r"(fd), "r"(message), "r"(count)
		: "rax", "rdi", "rsi", "rdx", "memory");
}

// 直接调用系统调用
