#include <errno.h>
#include <string.h>
#include "../include/client/client.h"
#include "../include/server/server.h"
#include "../include/common/utils.h"

#define CLIENT "--client"
#define SERVER "--server"


//argc - кол-во аргументов, **argv - передаваемое значение
int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "Выберите один из двух режимов для запуска программы: %s or %s.\n", CLIENT, SERVER);
        return EAGAIN; //не блокируем процесс после вызова
    }

    //Проверка на введенный режим
    if(argc == 2) {
        if(strcmp(argv[1], CLIENT) == 0) {
            fprintf(stderr, "Передайте в аргумент '%s' IP адрес сервера и порт\n", argv[1]);
        } else if(strcmp(argv[1], SERVER) == 0){
            fprintf(stderr, "Передайте в аргумент '%s' порт\n", argv[1]);
        } else {
            fprintf(stderr, "Некоректный ввод\n");
        }
        return EAGAIN;
    }

    //проверка вводимого порта
    if(argc == 3) {
        if(strcmp(argv[1], CLIENT) == 0) {
            fprintf(stderr, "Передайте в аргумент '%s'  IP адрес сервера и порт\n", argv[1]);
        } else if(strcmp(argv[1], SERVER) == 0){
            long port = strtol(argv[2], NULL, 10);
            if(port >= 65535 || port < 1) {
                fprintf(stderr, "Укажите порт в приделах от 1 до 65535\n");
                return EAGAIN;
            }
            return server(port);
        } else {
            fprintf(stderr, "Некоректный ввод\n");
        }
        return EAGAIN;
    }


    //проверка вводимого ip адреса клиентом
    if(argc > 3) {
        if(strcmp(argv[1], CLIENT) == 0) {
            long port = strtol(argv[3], NULL, 10);
            if(port >= 65535 || port < 1) {
                fprintf(stderr, "Укажите порт в приделах от 1 до 65535\n");
                return EAGAIN;
            }
            if(!isValidIpAddress(argv[2])) {
                fprintf(stderr, "Некоректный IP-адрес\n");
                return EAGAIN;
            }
            return client(argv[2], port);
        } else if(strcmp(argv[1], SERVER) == 0){
            fprintf(stderr, "В Argument '%s' нужно передать только порт\n", argv[1]);
            return EAGAIN;
        } else {
            fprintf(stderr, "Некоректный ввод\n");
        }
        return EAGAIN;
    }

    return EFAULT;
}