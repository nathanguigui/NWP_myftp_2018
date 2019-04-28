/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** upload
*/

#include "my_ftp.h"

int exec_stor_pasv(client_t *client)
{
    socklen_t csin_size = sizeof(struct sockaddr_in);
    int cliSocket = accept(client->PASV->pasvSocket, (SOCK)client->PASV->csin,
    &csin_size);
    if (cliSocket < 0)
        my_error("[PASV] accept()");
    int fd = open(realpath(client->input[1], NULL), O_CREAT|O_WRONLY|O_RDONLY);
    char buff[1024];
    for (size_t len = 0; len > 0; read(fd, buff, 1024))
        write(cliSocket, buff, len);
    close(cliSocket);
    close(client->PASV->pasvSocket);
    return (0);
}

int stor_pasv(client_t *client)
{
    chdir(client->wd);
    int fd = open(realpath(client->input[1], NULL), O_CREAT|O_WRONLY|O_RDONLY);
    if (fd < 0)
        return (write_client(client, "450 Can't access file\n"));
    close(fd);
    write_client(client, "150\n");
    if (fork() == 0) {
        exec_stor_pasv(client);
        write_client(client, "226 File send OK.\n");
    }
    client->PASV = NULL;
    return (0);
}

int exec_stor_actv(client_t *client)
{
    struct sockaddr_in sin = {0};
    sin.sin_addr.s_addr = inet_addr(client->actv_ip);
    sin.sin_port = htons(client->actv_port);
    sin.sin_family = AF_INET;
    client->actv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(client->actv_sock, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("connect");
        return (write_client(client, "425 Can't connect to host.\n"));
    }
    int fd = open(realpath(client->input[1], NULL), O_CREAT|O_WRONLY|O_RDONLY);
    char buff[1024];
    for (size_t len = 0; len > 0; read(fd, buff, 1024))
        write(client->actv_sock, buff, len);
    return (0);
}

int stor_actv(client_t *client)
{
    chdir(client->wd);
    int fd = open(realpath(client->input[1], NULL), O_CREAT|O_WRONLY|O_RDONLY);
    if (fd < 0)
        return (write_client(client, "450 Can't access file\n"));
    close(fd);
    write_client(client, "150\n");
    if (fork() == 0) {
        exec_stor_actv(client);
        write_client(client, "226 File send OK.\n");
        client->actv_sock = 0;
        client->actv_ip = NULL;
        client->actv_port = 0;
    }
    return (0);
}

int stor_cmd(client_t *client)
{
    if (client->PASV == NULL && client->actv_ip == NULL)
        return (write_client(client, "425 Use PORT or PASV first.\n"));
    if (client->input[1] == NULL)
        return (write_client(client, "501\n"));
    if (client->actv_ip)
        stor_actv(client);
    else
        stor_pasv(client);
    return (0);
}