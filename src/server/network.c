#include "../../include/server/network.h"
#include "../../include/common/error.h"


int openSocket(int *connect_socket) {
    printf("Open socket...................");
    //инициализация сокета, если все ок возвращаем дискриптор сокета(целое число), если нет -1
    *connect_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect_socket < 0) {
        printf("faild\n");
        perror("Error: 'socket() failed'");
        return ERR_SERVER_SOCKET_OPEN;
    }
    printf("done\n");
    return SUCCESS;
}

int setSocketName(const int *connect_socket, int *reuse) {
    printf("Set socket name...................");
    // Устанавливаем опции сокета

    // connect_socket сокет десриптор
    // SOL_SOCKET уровень - библиотека сокетов,
    // SO_REUSEADDR - разрешает повторное использование локальных адресов
    // reuse -  булево значение, для включения посылки
    // последний аргумент -> размер reuse
    if (setsockopt(*connect_socket, SOL_SOCKET, SO_REUSEADDR, reuse, sizeof(int)) == -1) {
        printf("faild\n");
        perror("Error: 'setsockopt()'");
        return ERR_SERVER_SOCKET_SETNAME;
    }
    return SUCCESS;
}

//Присваивания сокету имени, для обеспечения соедениия с клиентом
int bindSocket(const int *connect_socket, struct sockaddr_in *server_address) {
    // Связываем сокет с локальным адресом протокола

    // connect_socket сокет десриптор
    // struct server_address указатель на структуру имени сокета
    // длинна сокета
    if (bind(*connect_socket, (struct sockaddr *) server_address, sizeof(struct sockaddr_in)) == -1) {
        printf("faild\n");
        perror("Error: 'bind()'");
        return ERR_SERVER_SOCKET_BIND;
    }
    printf("done\n");
    return SUCCESS;
}

int listenSocket(const int *connect_socket, int max_client) {
    printf("Socket to listen mode...........................");
    // Привязываемый сокет к принятию входящих соединений

    // connect_socket - дискриптор сокета
    // max_client - число соеднений
    if (listen(*connect_socket, max_client) == -1) {
        printf("faild\n");
        perror("Error: 'listen()'");
        return ERR_SERVER_SOCKET_LISTEN;
    }
    printf("done\n");
    return SUCCESS;
}


// Инициализация соединения
int init_connect(int *connect_socket, struct sockaddr_in *server_address, int *reuse, long port, int max_client) {
    int errno = SUCCESS;

    errno = openSocket(connect_socket);
    if (errno != SUCCESS) return errno;

    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(port);
    server_address->sin_addr.s_addr = htonl(INADDR_ANY);

    errno = setSocketName(connect_socket, reuse);
    if (errno != SUCCESS) return errno;

    errno = bindSocket(connect_socket, server_address);
    if (errno != SUCCESS) return errno;

    errno = listenSocket(connect_socket, max_client);
    if (errno != SUCCESS) return errno;

    return SUCCESS;
}