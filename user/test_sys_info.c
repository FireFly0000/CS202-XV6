/*#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int n = 0;
    if(argc >= 2) n = atoi(argv[1]);

    int active_proc = info(0);
    int syscall_count = info(1);
    printf("[Before forking %d children]: Active Proc: %d, Syscalls: %d\n", n, active_proc, syscall_count);
    int free_pages = info(2);
    printf("Free pages: %d\n", free_pages);

    for (int i = 0; i < n; i++)
    {
        sleep(1);
        int c = fork();
        if(c == 0){
            //printf("Number of children: %d\n", i);
            exit(0);
        }
    }

    active_proc = info(0);
    syscall_count = info(1);
    printf("[After forking %d children]: Active Proc: %d, Syscalls: %d\n", n, active_proc, syscall_count);

    free_pages = info(2);
    printf("Free pages: %d\n", free_pages);

    exit(0);
}*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAX_PROC 10
struct pinfo {
    int ppid;
    int syscall_count;
    int page_usage;
};
void print_sysinfo(void)
{
    
    int n_active_proc, n_syscalls, n_free_pages;
    n_active_proc = info(0);
    n_syscalls = info(1);
    n_free_pages = info(2);
    printf("[sysinfo] active proc: %d, syscalls: %d, free pages: %d\n",
    n_active_proc, n_syscalls, n_free_pages);
}

int main(int argc, char *argv[])
{
    int mem, n_proc, ret, proc_pid[MAX_PROC];
    //int n_proc, ret, proc_pid[MAX_PROC];
    if (argc < 3) {
        printf("Usage: %s [MEM] [N_PROC]\n", argv[0]);
        exit(-1);
    }
    mem = atoi(argv[1]);
    n_proc = atoi(argv[2]);
    if (n_proc > MAX_PROC) {
        printf("Cannot test with more than %d processes\n", MAX_PROC);
        exit(-1);
    }
    print_sysinfo();
    for (int i = 0; i < n_proc; i++) {
        sleep(1);
        ret = fork();
        if (ret == 0) { // child process
            struct pinfo param;
            malloc(mem); // this triggers a syscall
            for (int j = 0; j < 10; j++)
                procinfo(&param); // calls 10 times
            printf("[procinfo %d] ppid: %d, syscalls: %d, page usage: %d\n",
            getpid(), param.ppid, param.syscall_count, param.page_usage);
            while (1);
            //printf("Child Process %d\n", i);
            exit(0);
        }
        else { // parent
            proc_pid[i] = ret;
            continue;
        }
    }
    sleep(1);
    print_sysinfo();
    for (int i = 0; i < n_proc; i++) kill(proc_pid[i]);
    exit(0);
}