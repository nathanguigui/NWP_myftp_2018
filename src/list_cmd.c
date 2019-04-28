/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** list_cmd
*/

#include "my_ftp.h"

int exec_list_actv(client_t *client)
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
    char *command = "/bin/ls";
    char *arg_str;
    if (client->input[1])
        asprintf(&arg_str, "ls -l %s", client->input[1]);
    else
        asprintf(&arg_str, "ls -l %s", client->wd);
    char **arg = str_to_tab(arg_str, ' ');
    dup2(client->actv_sock, 1);
    execvp(command, arg);
    return (0);
}

int list_actv(client_t *client)
{
    write_client(client, "150 Here comes the directory listing.\n");
    if (fork() == 0) {
        exec_list_actv(client);
        write_client(client, "226 Directory send OK.\n");
        close(client->actv_sock);
        client->actv_sock = 0;
        client->actv_ip = NULL;
        client->actv_port = 0;
    }
    return (0);
}

int exec_list_pasv(client_t *client)
{
    char *command = "/bin/ls";
    char *arg_str;
    if (client->input[1])
        asprintf(&arg_str, "ls -l %s", client->input[1]);
    else
        asprintf(&arg_str, "ls -l %s", client->wd);
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

int list_pasv(client_t *client)
{
    write_client(client, "150 Here comes the directory listing.\n");
    if (fork() == 0) {
        exec_list_pasv(client);
        write_client(client, "226 Directory send OK.\n");
    }
    client->PASV = NULL;
    return (0);
}

int list_cmd(client_t *client)
{
    if (client->PASV == NULL && client->actv_ip == NULL)
        return (write_client(client, "425 Use PORT or PASV first.\n"));
    if (client->actv_ip)
        list_actv(client);
    else
        list_pasv(client);
    return (0);
}