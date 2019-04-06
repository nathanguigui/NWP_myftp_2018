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
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "my_code.h"

typedef struct server_s {
    int connected;
    int socket;
    char *root_dir;
    char *user;
    char *input;
} server_t;

int auth_user(server_t*);
int check_pass(server_t*);
int pwd_cmd(server_t*);
int wrong_cmd(server_t*);
int other_cmd(server_t*);

#endif /* !MY_FTP_H_ */
