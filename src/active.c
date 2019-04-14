/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** active
*/

#include "my_ftp.h"

int port_cmd(client_t *client)
{
    if (client->input[1] == NULL)
        return (write_client(client, "501\n"));
    struct sockaddr_in sin = {0};
    sin.sin_addr.s_addr = client->sin_addr;
    sin.sin_port = htons(atoi(client->input[1]));
    sin.sin_family = AF_INET;
    client->actv_sock = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t len = sizeof(sin);
    if (connect(client->actv_sock, (struct sockaddr *)&sin, len) == -1) {
        perror("connect");
        return (write_client(client, "501\n"));
    }
    return (write_client(client, "200\n"));
}