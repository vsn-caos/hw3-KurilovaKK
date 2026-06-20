#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

// Программе передаются два аргумента: имя файла и строка для поиска.
// Необходимо найти все вхождения строки в текстовом файле,
// используя отображение на память с помощью системного вызова mmap.
// На стандартный поток вывода вывести список всех позиций (с нуля),
// где встречается искомая строка, по одной на строку.

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <search_string>\n", argv[0]);
        return 1;
    }
    int f = open(argv[1], O_RDONLY);
    if (f < 0) {
        return 1;
    }
    struct stat s;
    if (fstat(f, &s) < 0) {
        close(f);
        return 1;
    }
    off_t n = s.st_size;
    char *p = argv[2];
    long m = strlen(p);
    if (m == 0 || n == 0 || m > n) {
        close(f);
        return 0;
    }
    char *a = mmap(NULL, n, PROT_READ, MAP_PRIVATE, f, 0);
    if (a == MAP_FAILED) {
        close(f);
        return 1;
    }
    for (long i = 0; i <= n - m; i++) {
        if (memcmp(a + i, p, m) == 0) {
            printf("%ld\n", i);
        }
    }
    munmap(a, n);
    close(f);
    return 0;
}
