/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** my_ftp
*/

#include "my_ftp.h"

server_t *init_server(const char **av, struct sockaddr_in csin, int socket)
{   
    char ip_in[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &csin.sin_addr, ip_in, INET_ADDRSTRLEN);
    printf("[SERVER]\tclient connected from %s\n", ip_in);

    server_t *server = malloc(sizeof(server));
    server->connected = 0;
    server->user = NULL;
    server->socket = socket;
    server->root_dir = strdup(av[2]);
    return (server);
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

void ftp_shell(int connected_socket, const char **av, struct sockaddr_in csin)
{
    server_t *server = init_server(av, csin, connected_socket);
    char buff[2048];
    while (1) {
        read(server->socket, buff, 2048);
        server->input = strdup(buff);
        if (strncasecmp("USER", buff, 4) == 0) {
            auth_user(server);
            continue;
        } if (strncasecmp("PASS", buff, 4) == 0) {
            check_pass(server);
            continue;
        } if (server->connected == CONNECTED)
            other_cmd(server);
        memset(buff, 0, 2048);
        printf("%d", server->connected);
    }
}

int start_server(int server_socket, struct sockaddr_in csin, const char **av)
{
    socklen_t csin_size = sizeof(csin);
    int connected_socket_fd;
    printf("[SERVER]\tserver started\n");
    do {
        connected_socket_fd = accept(server_socket, (struct sockaddr *)&csin, &csin_size);
        if (connected_socket_fd == -1) {
            perror("accept()");
            exit(84);
        } if (fork() == 0) {
            close(server_socket);
            write(connected_socket_fd, "220\n", 4);
            ftp_shell(connected_socket_fd, av, csin);
        }
    } while (1);
    return (0);
}

int main(int ac, char const **av)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin = {0};
    struct sockaddr_in csin = {0};
    
    if (check_arg(ac, av, &sin) == ERROR)
        exit(84);
    if (server_socket == -1) {
        perror("[SERVER][ERROR]  socket()\t");
        exit(84);
    } if (bind(server_socket, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("[SERVER][ERROR]  bind()\t");
        exit(84);
    } if (listen(server_socket, 5)) {
        perror("[SERVER][ERROR]  listen()\t");
        exit(84);
    }
    printf("[SERVER]\tstarting server on port %d\n", atoi(av[1]));
    start_server(server_socket, csin, av);
    return (0);
}