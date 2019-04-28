/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** download
*/

#include "my_ftp.h"

int exec_retr_actv(client_t *client)
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
    char *command = "/bin/cat";
    char *arg_str;
    asprintf(&arg_str, "cat %s", realpath(client->input[1], NULL));
    char **arg = str_to_tab(arg_str, ' ');
    dup2(client->actv_sock, 1);
    execvp(command, arg);
    return (0);
}

int retr_actv(client_t *client)
{
    chdir(client->wd);
    if (access(realpath(client->input[1], NULL), R_OK))
        return (write_client(client, "450 Can't access file\n"));
    struct stat stat_buff = {0};
    stat(realpath(client->input[1], NULL), &stat_buff);
    char *mess;
    asprintf(&mess, "150 Sending %s (%ld bytes).\n", realpath(client->input[1],
    NULL), stat_buff.st_size);
    write_client(client, mess);
    if (fork() == 0) {
        exec_retr_actv(client);
        write_client(client, "226 File send OK.\n");
        client->actv_sock = 0;
        client->actv_ip = NULL;
        client->actv_port = 0;
    }
    return (0);
}

int exec_retr_pasv(client_t *client)
{
    char *command = "/bin/cat";
    char *arg_str;
    asprintf(&arg_str, "cat %s", realpath(client->input[1], NULL));
    char **arg = str_to_tab(arg_str, ' ');
    socklen_t csin_size = sizeof(struct sockaddr_in);
    int cliSocket = accept(client->PASV->pasvSocket, (SOCK)client->PASV->csin,
    &csin_size);
    if (cliSocket < 0)
        my_error("[PASV] accept()");
    dup2(cliSocket, 1);
    execvp(command, arg);
    close(cliSocket);
    close(client->PASV->pasvSocket);
    return (0);
}

int retr_pasv(client_t *client)
{
    chdir(client->wd);
    if (access(realpath(client->input[1], NULL), R_OK))
        return (write_client(client, "450 Can't access file\n"));
    struct stat stat_buff = {0};
    stat(realpath(client->input[1], NULL), &stat_buff);
    char *mess;
    asprintf(&mess, "150 Sending %s (%ld bytes).\n", realpath(client->input[1],
    NULL), stat_buff.st_size);
    write_client(client, mess);
    if (fork() == 0) {
        exec_retr_pasv(client);
        write_client(client, "226 File send OK.\n");
    }
    client->PASV = NULL;
    return (0);
}

int retr_cmd(client_t *client)
{
    if (client->PASV == NULL && client->actv_ip == NULL)
        return (write_client(client, "425 Use PORT or PASV first.\n"));
    if (client->input[1] == NULL)
        return (write_client(client, "501\n"));
    if (client->actv_ip)
        retr_actv(client);
    else
        retr_pasv(client);
    return (0);
}