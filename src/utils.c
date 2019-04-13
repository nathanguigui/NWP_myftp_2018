/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** utils
*/

#include "my_ftp.h"

int read_client(client_t *client)
{
    char buff[2048];
    char *tmp = NULL;

    if (read(client->socket, buff, 2048) == 0)
        return (-1);
    tmp = strtok(buff, "\r");
    client->input = str_to_tab(tmp, ' ');
    return (0);
}

int write_client(client_t *client, char *to_write)
{
    return (write(client->socket, to_write, strlen(to_write)));
}

int my_error(char *str)
{
    perror(str);
    exit(84);
}

char *my_strcat(char *str1, char *str2)
{
    char *res;
    int i = 0;

    res = malloc(sizeof(char) * (strlen(str1) + strlen(str2)) + 1);
    if (res == NULL)
        return (NULL);
    if (str1 != NULL) {
        while (str1[i] != '\0') {
            res[i] = str1[i];
            i = i + 1;
        }
    }
    if (str2 != NULL) {
        for (int a = 0; str2[a] != '\0'; a = a + 1) {
            res[i] = str2[a];
            i = i + 1;
        }
    }
    res[i] = '\0';
    return (res);
}

int check_cmd(char *str)
{
    if (strcasecmp("USER", str) == 0)
        return (0);
    if (strcasecmp("PASS", str) == 0)
        return (0);
    if (strcasecmp("NOOP", str) == 0)
        return (0);
    if (strcasecmp("PWD", str) == 0)
        return (0);
    if (strcasecmp("QUIT", str) == 0)
        return (0);
    if (strcasecmp("HELP", str) == 0)
        return (0);
    if (strcasecmp("CDUP", str) == 0)
        return (0);
    if (strcasecmp("CWD", str) == 0)
        return (0);
    if (strcasecmp("DELE", str) == 0)
        return (0);
    if (strcasecmp("PASV", str) == 0)
        return (0);
    return (-1);
}