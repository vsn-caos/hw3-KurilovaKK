#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Программе на стандартный поток ввода задается арифметическое выражение
// в синтаксисе языка python3. Необходимо вычислить это выражение и вывести результат.
// Использовать дополнительные процессы запрещено — нужно использовать exec.

int main(void) {
    char a[4096];
    if (fgets(a, sizeof(a), stdin) == NULL) {
        return 1;
    }
    char b[8192];
    snprintf(b, sizeof(b), "print(%s)", a);
    char *c[] = {"python3", "-c", b, NULL};
    execvp("python3", c);
    
    return 0;
}
