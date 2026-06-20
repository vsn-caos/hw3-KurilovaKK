#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Программе передаётся два аргумента: CMD1 и CMD2.
// Необходимо запустить два процесса, выполняющих эти команды,
// и перенаправить стандартный поток вывода CMD1 на стандартный поток ввода CMD2.
// Эквивалентно: CMD1 | CMD2
// Родительский процесс должен завершаться самым последним.

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <CMD1> <CMD2>\n", argv[0]);
        return 1;
    }
    int a[2];
    if (pipe(a) < 0) {
        return 1;
    }
    pid_t p = fork();
    if (p < 0) {
        return 1;
    }
    if (p == 0) {
        dup2(a[1], STDOUT_FILENO);
        close(a[0]);
        close(a[1]);
        execlp(argv[1], argv[1], NULL);
        return 1;
    }
    pid_t q = fork();
    if (q < 0) {
        return 1;
    }
    if (q == 0) {
        dup2(a[0], STDIN_FILENO);
        close(a[0]);
        close(a[1]);
        execlp(argv[2], argv[2], NULL);
        return 1;
    }
    close(a[0]);
    close(a[1]);
    waitpid(p, NULL, 0);
    waitpid(q, NULL, 0);
    return 0;
}
