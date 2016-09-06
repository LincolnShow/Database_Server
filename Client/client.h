#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "config.h"

class Client
{
public:
    Client(int port);
    ~Client() {close(sock);}
    std::string SendMsg(std::string);
private:
    int sock;
};

#endif // CLIENT_H
