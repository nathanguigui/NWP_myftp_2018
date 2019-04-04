/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** server
*/

#include "my_ftp.h"

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