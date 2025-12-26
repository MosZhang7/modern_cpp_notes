#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    size_t MB = 1024 * 1024;
    size_t size = 256 * MB;          // 256MB，安全可控
    char *p = malloc(size);
    if (!p) { perror("malloc"); return 1; }

    printf("Allocated 256MB. PID=%d\n", getpid());
    printf("Phase 1: not touching memory. Press Enter...\n");
    getchar();

    // 每 4KB（典型页大小）触碰一次
    for (size_t i = 0; i < size; i += 4096) {
        p[i] = 1;
    }

    printf("Phase 2: touched every page. Press Enter...\n");
    getchar();
    sleep(10); // 留时间观察
    return 0;
}
