/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** my_ftp
*/

#ifndef MY_FTP_H_
#define MY_FTP_H_

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

typedef struct server_s {
    struct sockaddr_in sin;
    int server_socket;
    struct sockaddr_in csin;
    int client_socket;
    int csin_size;
    int port;
    char *folder;
} server_t;

int check_arg(char const**, int, server_t*);
int start_server(server_t*);

#endif /* !MY_FTP_H_ */
