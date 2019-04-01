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
    sin.sin_port = htons(80);
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

    int csin_size = sizeof(csin);

    int connected_socket_fd = accept(server_socket, (struct sockaddr *)&csin, &csin_size);
    if (connected_socket_fd == -1) {
        perror("accept()");
        exit(errno);
    }
    
    write(connected_socket_fd, SERVICE_READY, 4);
    char buff[2048];
    for (ssize_t read_b = 1; read_b; read(connected_socket_fd, buff, 2048)) {
        if (strcmp("USER Anonymous", buff) == 0)
            write(connected_socket_fd, WAIT_PASS, 4);
    }

    return (0);
}