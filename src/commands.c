/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** commands
*/

#include "my_ftp.h"

int check_pass(server_t *server)
{
    if (server->connected != AWAITING_PASS)
        return (write(server->socket, "500\n", 4));
    write(server->socket, "230\n", 4);
    printf("[SERVER]\tuser connected\n");
    server->connected = CONNECTED;
    return (0);
}

int auth_user(server_t *server)
{
    if (server->connected == AWAITING_PASS || server->connected == CONNECTED)
        return (write(server->socket, "500\n", 4));
    write(server->socket, "230\n", 4);
    write(server->socket, "331\n", 4);
    server->input[strlen(server->input) - 1] = 0;
    printf("[SERVER]\tuser trying to connect as %s\n", server->input + 5);
    server->connected = AWAITING_PASS;
    server->user = strdup(server->input + 5);
    return (0);
}

int wrong_cmd(server_t *server)
{
    server->connected = 0;
    write(server->socket, "500\n", 4);
    return (0);
}

int pwd_cmd(server_t *server)
{
    char buff[1024];
    char *pwd = NULL;
    getcwd(buff, sizeof(buff));
    pwd = strcat(strcat("230 ", pwd), "\n");
    write(server->socket, pwd, strlen(pwd));
    return (0);
}

int other_cmd(server_t *server)
{
    write(1, server->input, strlen(server->input));
    int status = -1;
    if (strncasecmp("PWD", server->input, 3) == 0)
        status = pwd_cmd(server);
    if (strncasecmp("WHOAMI", server->input, 6) == 0)
        status = dprintf(server->socket, "230 %s\n", server->user);
    if (status == -1)
        wrong_cmd(server);
    return (0);
}