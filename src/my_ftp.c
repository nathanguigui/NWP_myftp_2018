/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** my_ftp
*/

#include "my_ftp.h"

int main(int ac, char const **av)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin = {0};
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(7878);
    sin.sin_family = AF_INET;

    int client_socket;
    struct sockaddr_in csin = {0};
    
    if (server_socket == -1) {
        perror("socket()");
        exit(errno);
    }

    if (bind(server_socket, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("bind()");
        exit(errno);
    }

    if (listen(server_socket, 5)) {
        perror("listen()");
        exit(errno);
    }

    connect_client(server_socket, csin);
    return (0);
}

int auth_user(int connected_socket_fd, char *buff)
{
    if (strncmp("USER", buff, 4) == 0) {
        write(connected_socket_fd, "230\n", 4);
        write(connected_socket_fd, "331\n", 4);
        read(connected_socket_fd, buff, 2048);
        if (strncmp("PASS", buff, 4) == 0) {
            write(connected_socket_fd, "230\n", 4);
            return (1);
        }
    }
    return (0);
}

int connect_client(int server_socket, struct sockaddr_in csin)
{
    int csin_size = sizeof(csin);

    int connected_socket_fd = accept(server_socket, (struct sockaddr *)&csin, &csin_size);
    if (connected_socket_fd == -1) {
        perror("accept()");
        exit(errno);
    }
    
    write(connected_socket_fd, "220\n", 4);
    char buff[2048];
    do {
        read(connected_socket_fd, buff, 2048);
        auth_user(connected_socket_fd, buff);
        memset(buff, 0, 2048);
    } while (strcmp("QUIT", buff) != 0);
}