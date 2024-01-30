#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAX_PROC 10
struct pinfo {
    int ppid;
    int syscall_count;
    int page_usage;
};

int main(int argc, char *argv[])
{
    struct pinfo param;
    
    procinfo(&param);

    printf("PPID: %d\n", param.ppid);
    printf("Syscalls: %d\n", param.syscall_count);
    printf("Page usage: %d\n", param.page_usage);
    exit(0);
}