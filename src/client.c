/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** client
*/

#include "my_ftp.h"

void init_client(core_t *CORE, int sock, int i, struct sockaddr_in *csin)
{
    CORE->client_fds[i] = sock;
    client_t *client = malloc(sizeof(client_t));
    client->connected = 0;
    client->user = NULL;
    client->socket = sock;
    client->root_dir = strdup(CORE->root_dir);
    client->wd = strdup(CORE->root_dir);
    client->ip = inet_ntoa(csin->sin_addr);
    CORE->clients[i] = client;
    client->PASV = NULL;
    client->PASV_pid = 0;
}

int client_shell(int i, core_t *CORE)
{
    client_t *CLIENT = CORE->clients[i];

    if (read_client(CLIENT) == -1 ||
    (strcasecmp("QUIT", CLIENT->input[0]) == 0)) {
        quit_cmd(CORE, i);
        close(CORE->client_fds[i]);
        CORE->client_fds[i] = 0;
        CORE->clients[i] = NULL;
    } else {
        chdir(CLIENT->wd);
        if (strcasecmp("USER", CLIENT->input[0]) == 0)
            return (auth_user(CLIENT));
        if (strcasecmp("PASS", CLIENT->input[0]) == 0)
            return (check_pass(CLIENT));
        if (CLIENT->connected == CONNECTED)
            return (other_cmd(CLIENT));
        if (CLIENT->connected != CONNECTED)
            write_client(CLIENT, "530 Please login with USER and PASS.\n");
    }
    return (0);
}

void client_manage(core_t *CORE, int cli_socket, int sd,
struct sockaddr_in csin)
{
    socklen_t csin_size = sizeof(csin);
    int i = 0;

    if (FD_ISSET(CORE->mSocket, CORE->readfds)) {
        cli_socket = accept(CORE->mSocket, (SOCK)&csin, &csin_size);
        if (cli_socket < 0)
            my_error("accept()");
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (CORE->client_fds[i] == 0) {
                init_client(CORE, cli_socket, i, &csin);
                break;
            }
        }
        write_client(CORE->clients[i], "220\n");
    }
    for (int i = 0; i < MAX_CLIENTS; i++) {
        sd = CORE->client_fds[i];
        if (FD_ISSET(sd, CORE->readfds))
            client_shell(i, CORE);
    }
}