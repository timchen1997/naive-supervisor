#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    while (1) {
        sleep(1);
        pid_t p = getpid();
        int fd = open("test.txt", O_RDONLY);
        printf("%d\n", p);
        close(fd);
    }
    return 0;
}