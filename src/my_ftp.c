/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** my_ftp
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

int check_arg(int ac, char const **av, struct sockaddr_in *sin)
{
    if (ac != 3)
        return (-1);
    if (atoi(av[1]) == 0)
        return (-1);
    if (chdir(av[2]) == -1)
        return (-1);
    sin->sin_addr.s_addr = htonl(INADDR_ANY);
    sin->sin_port = htons(atoi(av[1]));
    sin->sin_family = AF_INET;
    return (0);
}

core_t *init_core(const char **av, int server_socket)
{
    core_t *CORE = malloc(sizeof(core_t));
    CORE->root_dir = strdup(av[2]);
    CORE->mSocket = server_socket;
    CORE->readfds = malloc(sizeof(fd_set));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        CORE->client_fds[i] = 0;
        CORE->clients[i] = NULL;
    }
    return (CORE);
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
            write_client(CLIENT, "530 Please login with USER and PASS.\n");
    }
    return (0);
}

void client_manage(core_t *CORE, int cli_socket, int sd, struct sockaddr_in csin)
{
    socklen_t csin_size = sizeof(csin);

    if (FD_ISSET(CORE->mSocket, CORE->readfds)) {
        cli_socket = accept(CORE->mSocket, (struct sockaddr*)&csin, &csin_size);
        if (cli_socket < 0)
            my_error("accept()");
        write(cli_socket, "220\n", 4);
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

int start_server(int server_socket, struct sockaddr_in csin, const char **av)
{
    core_t *CORE = init_core(av, server_socket);
    int activity;

    while (1) {
        FD_ZERO(CORE->readfds);
        FD_SET(CORE->mSocket, CORE->readfds);
        CORE->max_sd = CORE->mSocket;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (CORE->client_fds[i] > 0)
                FD_SET(CORE->client_fds[i], CORE->readfds);
            if (CORE->client_fds[i] > CORE->max_sd)
                CORE->max_sd = CORE->client_fds[i];
        }
        activity = select(CORE->max_sd + 1, CORE->readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
            my_error("select()");
        client_manage(CORE, 0, 0, csin);
    }
    return (0);
}

int main(int ac, char const **av)
{
    int mSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin = {0};
    struct sockaddr_in csin = {0};
    int opt = TRUE;
    
    if (check_arg(ac, av, &sin) == ERROR)
        exit(84);
    if (mSocket == -1)
        my_error("[SERVER][ERROR]  socket()\t");
    if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)))
        my_error("[SERVER][ERROR]  setsockopt()\t");
    if (bind(mSocket, (struct sockaddr *)&sin, sizeof(sin)))
        my_error("[SERVER][ERROR]  bind()\t");
    if (listen(mSocket, 3))
        my_error("[SERVER][ERROR]  listen()\t");
    start_server(mSocket, csin, av);
    return (0);
}