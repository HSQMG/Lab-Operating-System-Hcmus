#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 35

void sieve(int p);

int main() {
    int primes_pipe[2];
    pipe(primes_pipe);
    int p;

    if (fork() == 0) {
        close(primes_pipe[1]);
        sieve(primes_pipe[0]);
    } else {
        close(primes_pipe[0]);
        for (p = 2; p <= N; p++) {
            write(primes_pipe[1], &p, sizeof(p));
        }
        close(primes_pipe[1]);
        wait(0);
    }

    exit(0);
}

void sieve(int p) {
    int num;
    if (read(p, &num, sizeof(num)) <= 0) {
        close(p);
        exit(0);
    }

    printf("prime %d\n", num);

    int next_pipe[2];
    pipe(next_pipe);

    if (fork() == 0) {
        close(next_pipe[1]);
        close(p);
        sieve(next_pipe[0]);
    } else {
        close(next_pipe[0]);
    }

    while (1) {
        int n;
        if (read(p, &n, sizeof(n)) <= 0) {
            close(p);
            break;
        }
        if (n % num != 0) {
            write(next_pipe[1], &n, sizeof(n));
        }
    }

    close(next_pipe[1]);
    wait(0);
    exit(0);
}