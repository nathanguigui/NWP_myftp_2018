/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** retr_cmd
*/

#include "my_ftp.h"

int retr_actv(client_t *client)
{
    chdir(client->wd);
    char *path = realpath(client->input[1], NULL);
    if (path == NULL)
        return (write_client(client, "425\n"));
    if (access(path, F_OK) == -1)
        return (write_client(client, "425\n"));
    int fd = open(path, O_RDONLY);
    write_client(client, "150\n")
}

int retr_cmd(client_t *client)
{
    if (client->PASV == NULL && client->actv_sock == 0)
        return (write_client(client, "425 Use PORT or PASV first.\n"));
    if (client->actv_sock)
        retr_actv(client);
}