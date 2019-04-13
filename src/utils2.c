/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** utils2
*/

#include "my_ftp.h"

char *get_ip(void)
{
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;

    int hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1)
        my_error("gethostname");
    host_entry = gethostbyname(hostbuffer);
    if (host_entry == NULL)
        my_error("gethostbyname");
    IPbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
    if (IPbuffer == NULL)
        my_error("inet_ntoa");
    return (IPbuffer);
}