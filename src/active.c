/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** active
*/

#include "my_ftp.h"

char *check_ipstr(char *ip_str)
{
    if (ip_str == NULL)
        return (NULL);
    char **ip_tab = str_to_tab(ip_str, ',');
    unsigned int i = 0;
    for (i = 0; ip_tab[i]; i++);
    if (i != 6)
        return (NULL);
    char *ret;
    for (i = 0; i != 4; i++) {
        if (i == 0)
            asprintf(&ret, "%d", atoi(ip_tab[i]));
        else
            asprintf(&ret, "%s.%d", ret, atoi(ip_tab[i]));
    }
    return (ret);
}

int port_cmd(client_t *client)
{
    if (client->input[1] == NULL)
        return (write_client(client, "501\n"));
    if (!check_ipstr(client->input[1]))
        return (write_client(client, "501\n"));
    client->actv_ip = check_ipstr(client->input[1]);
    char **ip_tab = str_to_tab(client->input[1], ',');
    client->actv_port = atoi(ip_tab[4]) * 255 + atoi(ip_tab[5]);
    return (write_client(client, "200\n"));
}