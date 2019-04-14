/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** list_cmd
*/

#include "my_ftp.h"

int list_actv(client_t *client)
{
    write_client(client, "150 Here comes the directory listing.\n");
    if (fork() == 0) {
        char *command = "/bin/ls";
        char *arg_str;
        if (client->input[1])
            asprintf(&arg_str, "ls -l %s", client->input[1]);
        else
            asprintf(&arg_str, "ls -l %s", client->wd);
        char **arg = str_to_tab(arg_str, ' ');
        dup2(client->actv_sock, 1);
        execvp(command, arg);
    } else {
        wait(NULL);
        close(client->actv_sock);
        client->actv_sock = 0;
        write_client(client, "226 Directory send OK.\n");
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
}

int list_pasv(client_t *client)
{
    write_client(client, "150 Here comes the directory listing.\n");
    if (fork() == 0)
        exec_list_pasv(client);
    else {
        wait(NULL);
        write_client(client, "226 Directory send OK.\n");
    }
}

int list_cmd(client_t *client)
{
    if (client->PASV == NULL && client->actv_sock == 0)
        return (write_client(client, "425 Use PORT or PASV first.\n"));
    if (client->actv_sock) {
        if (fork() == 0)
            list_actv(client);
    }
    else {
        if (fork() == 0)
            list_pasv(client);
    }
    return (0);
}