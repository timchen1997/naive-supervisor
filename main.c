#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stddef.h>

int wait_until_syscall(pid_t pid) {
    int status;
    while (1) {
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, &status, 0);
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            return 0;
        }
        if (WIFEXITED(status)) {
            return 1;
        }
        printf("unhandled stop\n");
    }
}

void handle_syscall(pid_t pid) {
    long rax = ptrace(PTRACE_PEEKUSER, pid, offsetof(struct user, regs.orig_rax), 0);
    printf("Syscall: %ld\n", rax);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "invalid argument\n");
        return -1;
    }
    pid_t pid = atoi(argv[1]);
    printf("pid = %d\n", pid);
    ptrace(PTRACE_ATTACH, pid, 0, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
    while (1) {
        if (wait_until_syscall(pid)) {
            break;
        }
        handle_syscall(pid);
    }
    return 0;
}