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
#include <limits.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 20
#define TRUE 1
#define AWAITING_PASS 2
#define CONNECTED 1
#define ERROR -1
#define NB_CMDS 9
extern char *CMDS[NB_CMDS];

typedef struct sockaddr * SOCK;

typedef struct pasv_s {
    int port;
    int pasvSocket;
    int cliSocket;
    struct sockaddr_in *csin;
} pasv_t;

typedef struct client_s {
    int connected;
    int socket;
    char *root_dir;
    char *wd;
    char *user;
    char **input;
    char *ip;
    pasv_t *PASV;
    pid_t PASV_pid;
    in_addr_t sin_addr;
    int actv_sock;
    int (*commands[NB_CMDS])(struct client_s *);
} client_t;

typedef struct core_s {
    int client_fds[MAX_CLIENTS];
    client_t *clients[MAX_CLIENTS];
    fd_set *readfds;
    int mSocket;
    int max_sd;
    char *root_dir;
} core_t;

int auth_user(client_t *);
int check_pass(client_t *);
int pwd_cmd(client_t *);
int quit_cmd(core_t *, int);
int noop_cmd(client_t *);
int help_cmd(client_t *);
int cwd_cmd(client_t *);
int cdup_cmd(client_t *);
int dele_cmd(client_t *);
int pasv_cmd(client_t *);
int list_cmd(client_t *);
int port_cmd(client_t *);

int wrong_cmd(client_t *);
int other_cmd(client_t *);

void server_base(int, char const **);
void client_manage(core_t *, int, int, struct sockaddr_in);

int read_client(client_t *);
int write_client(client_t *client, char *to_write);
int my_error(char *);
int check_cmd(char *);
char *my_strcat(char *, char *);
char **str_to_tab(char *, char);
void read_input(client_t *);
char *get_ip(void);
#endif /* !MY_FTP_H_ */
