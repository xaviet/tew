#pragma once

#include "libssh2_config.h"
#include "libssh2.h"
 
#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#endif
 
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <sys/types.h>
 
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
 
#ifndef INADDR_NONE
#define INADDR_NONE (in_addr_t)-1
#endif

enum {
    AUTH_NONE = 0,
    AUTH_PASSWORD,
    AUTH_PUBLICKEY
};