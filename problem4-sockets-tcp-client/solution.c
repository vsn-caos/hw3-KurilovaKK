#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Программе передаются два аргумента:
//   argv[1] — IPv4-адрес сервера в десятичной записи (например, "127.0.0.1")
//   argv[2] — номер порта
//
// Программа должна:
//   1. Установить TCP-соединение с указанным сервером.
//   2. В цикле читать со stdin целые знаковые числа в текстовом формате.
//   3. Отправлять каждое число на сервер в бинарном виде (int32, Little Endian).
//   4. Получать от сервера int32 LE в ответ и выводить его в stdout в текстовом виде.
//   5. Если сервер закрыл соединение — завершиться с кодом возврата 0.

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ipv4_addr> <port>\n", argv[0]);
        return 1;
    }
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        return 1;
    }

    struct sockaddr_in a;
    memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET;
    a.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &a.sin_addr) != 1) {
        close(s);
        return 1;
    }
    if (connect(s, (struct sockaddr *)&a, sizeof(a)) < 0) {
        close(s);
        return 1;
    }
    
    int x;
    while (scanf("%d", &x) == 1) {
        int y = x;
        if (send(s, &y, 4, 0) != 4) {
            close(s);
            return 0;
        }
        int z;
        int r = recv(s, &z, 4, MSG_WAITALL);
        if (r == 0) {
            close(s);
            return 0;
        }
        if (r != 4) {
            close(s);
            return 0;
        }
        printf("%d\n", z);
        fflush(stdout);
    }
    close(s);

    return 0;
}
