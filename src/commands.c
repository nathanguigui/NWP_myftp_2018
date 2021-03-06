/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** commands
*/

#include "my_ftp.h"

int check_pass(client_t *client)
{
    if (client->connected == CONNECTED)
        return (write_client(client, "230 Already logged in.\n"));
    if (client->connected != AWAITING_PASS)
        return (write_client(client, "503 Login with USER first.\n"));
    if (strcasecmp(client->user, "anonymous"))
        return (write_client(client, "530 Login incorrect.\n"));
    write_client(client, "230 Login successful.\n");
    client->connected = CONNECTED;
    return (0);
}

int auth_user(client_t *client)
{
    if (client->connected == CONNECTED)
        return (write_client(client, "530 Can't change from guest user.\n"));
    write_client(client, "331 Please specifiy password.\n");
    client->connected = AWAITING_PASS;
    client->user = strdup(client->input[1]);
    return (0);
}

int wrong_cmd(client_t *client)
{
    write_client(client, "500 Unknown command.\n");
    return (0);
}

int pwd_cmd(client_t *client)
{
    write_client(client, my_strcat(my_strcat("257 \"", client->wd), "\"\n"));
    return (0);
}

int other_cmd(client_t *client)
{
    for (int i = 0; i < NB_CMDS; i++) {
        if (strcasecmp(CMDS[i], client->input[0]) == 0)
            return ((*client->commands[i])(client));
    }
    if (check_cmd(client->input[0]))
        wrong_cmd(client);
    return (0);
}