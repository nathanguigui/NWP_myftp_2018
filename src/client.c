/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** client
*/

#include "my_ftp.h"

client_t *init_client(core_t *CORE, int socket)
{
    client_t *client = malloc(sizeof(client_t));
    client->connected = 0;
    client->user = NULL;
    client->socket = socket;
    client->root_dir = strdup(CORE->root_dir);
    client->wd = strdup(CORE->root_dir);
    return (client);
}

int client_shell(int i, core_t *CORE)
{
    client_t *CLIENT = CORE->clients[i];

    if (read_client(CLIENT) == -1) {
        close(CORE->client_fds[i]);
        CORE->client_fds[i] = 0;
        CORE->clients[i] = NULL;
    } else {
        if (strncasecmp("USER", CLIENT->input, 4) == 0)
            auth_user(CLIENT);
        if (strncasecmp("PASS", CLIENT->input, 4) == 0)
            check_pass(CLIENT);
        if (CLIENT->connected == CONNECTED)
            other_cmd(CLIENT);
        else if (!check_cmd(CLIENT->input))
            write_client(CLIENT, "530 Please login with USER and PASS.\r\n");
    }
    return (0);
}

void client_manage(core_t *CORE, int cli_socket, int sd,
struct sockaddr_in csin)
{
    socklen_t csin_size = sizeof(csin);

    if (FD_ISSET(CORE->mSocket, CORE->readfds)) {
        cli_socket = accept(CORE->mSocket, (SOCK)&csin, &csin_size);
        if (cli_socket < 0)
            my_error("accept()");
        write(cli_socket, "220\r\n", 4);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (CORE->client_fds[i] == 0) {
                CORE->client_fds[i] = cli_socket;
                CORE->clients[i] = init_client(CORE, cli_socket);
                break;
            }
        }
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sd = CORE->client_fds[i];
        if (FD_ISSET(sd, CORE->readfds))
            client_shell(i, CORE);
    }
}