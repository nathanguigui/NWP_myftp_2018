/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** server
*/

#include "my_ftp.h"

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
    CORE->root_dir = realpath(av[2], NULL);
    CORE->mSocket = server_socket;
    CORE->readfds = malloc(sizeof(fd_set));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        CORE->client_fds[i] = 0;
        CORE->clients[i] = NULL;
    }
    return (CORE);
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
        chdir(CORE->root_dir);
    }
    return (0);
}

void server_base(int ac, char const **av)
{
    int mSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin = {0};
    struct sockaddr_in csin = {0};
    int opt = TRUE;

    if (check_arg(ac, av, &sin) == ERROR)
        exit(84);
    if (mSocket == -1)
        my_error("[SERVER][ERROR]  socket()\t");
    if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR,
    (char *)&opt, sizeof(opt)))
        my_error("[SERVER][ERROR]  setsockopt()\t");
    if (bind(mSocket, (struct sockaddr *)&sin, sizeof(sin)))
        my_error("[SERVER][ERROR]  bind()\t");
    if (listen(mSocket, 3))
        my_error("[SERVER][ERROR]  listen()\t");
    start_server(mSocket, csin, av);
}