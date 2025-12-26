#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void* f(void* arg) {
    while (1) sleep(1);
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, f, NULL);
    pthread_create(&t2, NULL, f, NULL);
    while (1) sleep(1);
}
