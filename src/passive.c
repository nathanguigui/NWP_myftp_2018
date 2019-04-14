/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** passive
*/

#include "my_ftp.h"

int pasv_server(client_t *client)
{
    pasv_t *PASV = malloc(sizeof(pasv_t));
    PASV->csin = malloc(sizeof(struct sockaddr_in));
    PASV->pasvSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin = {0};
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = 0;
    sin.sin_family = AF_INET;

    if (bind(PASV->pasvSocket, (struct sockaddr *)&sin, sizeof(sin)))
        my_error("[SERVER][ERROR][PASV] bin()");
    bzero(&sin, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(struct sockaddr_in);
    getsockname(PASV->pasvSocket, (struct sockaddr *)&sin, &len);
    PASV->port = ntohs(sin.sin_port);
    if (listen(PASV->pasvSocket, 3))
        my_error("[SERVER][ERROR][PASV] listen()");
    client->PASV = PASV;
    return (0);
}

int pasv_cmd(client_t *client)
{
    pasv_server(client);
    int p1 = client->PASV->port / 256;
    int p2 = client->PASV->port % 256;
    char *mess;
    char **ip_tble = str_to_tab(get_ip(), '.');
    asprintf(&mess, "227 Entering Passive Mode (%s,%s,%s,%s,%03d,%03d)\n",
    ip_tble[0], ip_tble[1], ip_tble[2], ip_tble[3], p1, p2);
    write_client(client, mess);
    return (0);
}