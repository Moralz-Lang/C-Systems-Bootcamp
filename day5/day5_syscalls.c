#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("day5.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd < 0){ perror("open failed"); exit(1); }

    const char *msg = "Bootcamp Day 5\n";

    if(write(fd, msg, 17) < 0){
        perror("write failed");
        exit(1);
    }

    close(fd);

    fd = open("day5.txt", O_RDONLY);
    if(fd < 0){ perror("open failed"); exit(1); }

    char buf[64];
    int n = read(fd, buf, 63);
    buf[n] = '\0';

    printf("File contents:\n%s", buf);

    close(fd);
    return 0;
}
