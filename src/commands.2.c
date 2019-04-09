/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** commands.2
*/

#include "my_ftp.h"

int quit_cmd(client_t *client)
{
    write_client(client, "221\n");
    return (0);
}

int noop_cmd(client_t *client)
{
    write_client(client, "200\n");
    return (0);
}

int help_cmd(client_t *client)
{
    write_client(client, "214 \n");
    write_client(client, " HELP NOOP PASS PWD QUIT USER\n");
    write_client(client, "214\n");
    return (0);
}