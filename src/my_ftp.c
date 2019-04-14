/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** my_ftp
*/

#include "my_ftp.h"

char *CMDS[NB_CMDS]  = {
    "NOOP", "PWD", "HELP",
    "CDUP", "CWD", "DELE",
    "PASV", "LIST", "PORT"
};

int main(int ac, char const **av)
{
    server_base(ac, av);
    return (0);
}