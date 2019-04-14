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
        write_client(client, "226 Directory send OK.\n");
    }
    return (0);
}

int list_cmd(client_t *client)
{
    if (client->PASV == NULL && client->actv_sock == 0)
        return (write_client(client, "425 Use PORT or PASV first.\n"));
    if (client->actv_sock)
        list_actv(client);
    return (0);
}