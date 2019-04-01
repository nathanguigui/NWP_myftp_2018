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

#define SERVICE_READY "220\n"
#define WAIT_PASS "331\n"
#define AUTH_OK "230\n"
#endif /* !MY_FTP_H_ */
