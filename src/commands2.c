/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** commands.2
*/

#include "my_ftp.h"

int quit_cmd(core_t *CORE, int i)
{
    write_client(CORE->clients[i], "221 Goodbye.\n");
    close(CORE->clients[i]->socket);
    CORE->clients[i] = NULL;
    return (0);
}

int noop_cmd(client_t *client)
{
    write_client(client, "200\n");
    return (0);
}

int help_cmd(client_t *client)
{
    write_client(client, "214\n");
    write_client(client, " CDUP CWD HELP NOOP PASS PWD QUIT USER\n");
    write_client(client, "214\n");
    return (0);
}

int cwd_cmd(client_t *client)
{
    char *mess;
    DIR *tmp;
    if (client->input[1] == NULL)
        return (write_client(client, "550\n"));
    if (realpath(client->input[1], NULL) == NULL)
        return (write_client(client, "550 Invalid path.\n"));
    tmp = opendir(realpath(client->input[1], NULL));
    if (tmp == NULL)
        return (write_client(client, "550 Invalid path.\n"));
    closedir(tmp);
    client->wd = realpath(client->input[1], NULL);
    asprintf(&mess, "250 '%s'\n", client->wd);
    write_client(client, mess);
    return (0);
}

int cdup_cmd(client_t *client)
{
    client->wd = realpath(my_strcat(client->wd, "/.."), NULL);
    char *mess;
    asprintf(&mess, "200 '%s'\n", client->wd);
    write_client(client, mess);
    return (0);
}