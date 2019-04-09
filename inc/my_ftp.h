/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** my_ftp
*/

#ifndef MY_FTP_H_
#define MY_FTP_H_
#define _GNU_SOURCE
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include "my_code.h"

#define MAX_CLIENTS 5
#define TRUE 1

typedef struct server_s {
    int connected;
    int socket;
    char *root_dir;
    char *wd;
    char *user;
    char *input;
} server_t;

typedef struct client_s {
    int connected;
    int socket;
    char *root_dir;
    char *wd;
    char *user;
    char *input;
} client_t;

typedef struct core_s {
    int client_fds[MAX_CLIENTS];
    client_t *clients[MAX_CLIENTS];
    fd_set *readfds;
    int mSocket;
    int max_sd;
    char *root_dir;
} core_t;

int auth_user(client_t*);
int check_pass(client_t*);
int pwd_cmd(client_t*);
int wrong_cmd(client_t*);
int other_cmd(client_t*);
void read_input(client_t*);
int quit_cmd(client_t*);
int noop_cmd(client_t*);
int help_cmd(client_t*);

int read_client(client_t*);
int write_client(client_t *client, char *to_write);
int my_error(char*);
int check_cmd(char*);

#endif /* !MY_FTP_H_ */
