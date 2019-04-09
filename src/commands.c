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
        return (write_client(client, "230 Already logged in.\r\n"));
    if (client->connected != AWAITING_PASS)
        return (write_client(client, "503 Login with USER first.\r\n"));
    if (strcasecmp(client->user, "anonymous"))
        return (write_client(client, "530 Login incorrect.\r\n"));
    write_client(client, "230 Login successful.\r\n");
    client->connected = CONNECTED;
    return (0);
}

int auth_user(client_t *client)
{
    if (client->connected == CONNECTED)
        return (write_client(client, "530 Can't change from guest user.\r\n"));
    write_client(client, "331 Please specifiy password.\r\n");
    client->connected = AWAITING_PASS;
    client->user = strdup(client->input + 5);
    return (0);
}

int wrong_cmd(client_t *client)
{
    client->connected = 0;
    write_client(client, "500 Unknown command.\r\n");
    return (0);
}

int pwd_cmd(client_t *client)
{
    write_client(client, my_strcat(my_strcat("257 \"", client->wd), "\"\r\n"));
    return (0);
}

int other_cmd(client_t *client)
{
    if (strncasecmp("PWD", client->input, 3) == 0)
        pwd_cmd(client);
    if (strncasecmp("HELP", client->input, 4) == 0)
        help_cmd(client);
    if (strncasecmp("NOOP", client->input, 4) == 0)
        noop_cmd(client);
    if (strncasecmp("QUIT", client->input, 4) == 0)
        quit_cmd(client);
    if (!check_cmd(client->input))
        wrong_cmd(client);
    return (0);
}