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
    if (read(client->socket, buff, 2048) == 0)
        return (-1);
    client->input = strtok(buff, "\n");
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
    if (strncasecmp("USER", str, 4) == 0)
        return (1);
    if (strncasecmp("PASS", str, 4) == 0)
        return (1);
    if (strncasecmp("NOOP", str, 4) == 0)
        return (1);
    if (strncasecmp("PWD", str, 3) == 0)
        return (1);
    if (strncasecmp("QUIT", str, 4) == 0)
        return (1);
    if (strncasecmp("HELP", str, 4) == 0)
        return (1);
    return (0);
}