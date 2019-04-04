/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** server
*/

#include "my_ftp.h"

int start_server(server_t *server)
{
    if (server->server_socket == -1) {
        perror("socket()");
        return (-1);
    }

    if (bind(server->server_socket, (struct sockaddr *)&server->sin, server->csin_size)) {
        perror("bind()");
        return (-1);
    }

    if (listen(server->server_socket, 5)) {
        perror("listen()");
        return (-1);
    }
    return (0);
}

int check_arg(char const **av, int ac, server_t *server)
{
    if (ac != 3)
        return (-1);
    if (atoi(av[1]) == 0)
        return (-1);
    server->port = atoi(av[1]);
    server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server->sin.sin_addr.s_addr = htonl(INADDR_ANY);
    server->sin.sin_port = htons(7878);
    server->sin.sin_family = AF_INET;
    server->csin_size = sizeof(server->csin);
    return (0);
}