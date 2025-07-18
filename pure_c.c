#include <sys/syscall.h>
#include <unistd.h>

// 使用syscall打印Hello World的函数
void print_hello_world() {
    const char *message = "Hello World\n";
    // syscall(SYS_write, fd, buf, count)
    // 1 = SYS_write, 1 = stdout, message, 12 = 字符串长度
    syscall(SYS_write, 1, message, 12);
}

// 直接调用系统调用
int main(int argc, char **argv)
{
    print_hello_world();
    return 0;
}
