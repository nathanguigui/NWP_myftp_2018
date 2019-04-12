/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** commands3
*/

#include "my_ftp.h"

int dele_cmd(client_t *client)
{
    if (client->input[1] == NULL)
        return (write_client(client, "501 Usage: DELE [PATH]\n"));
    if (access(realpath(client->input[1], NULL), W_OK))
        return (write_client(client, "450 Requested file action not taken.\n"));
    if (unlink(realpath(client->input, NULL)))
        return (write_client(client, "450 Requested file action not taken.\n"));
    return (write_client(client, "250 Requested file action okay, completed.\n"));
}