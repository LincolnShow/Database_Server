#include "client.h"

#include <cstring>

Client::Client(int port)
{
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
}

std::string Client::SendMsg(std::__cxx11::string msg)
{
//    char* message = new char(BUF_SIZE);
    char message[BUF_SIZE]={0};
    std::copy(msg.begin(), msg.end(), message);
    send(sock, message, msg.length(), 0);
    std::memset(message, 0, BUF_SIZE);
    recv(sock, message, BUF_SIZE, 0);
    return std::string (message);
}
