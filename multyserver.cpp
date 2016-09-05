#include "multyserver.h"

#include <iostream>
#include <string>

int MultyServer::set_nonblock(int fd)
{
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}


MultyServer::MultyServer(int masterPort)
{
    std::cout << "Initialisation..." << std::endl;
    MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int option = 1;
    setsockopt(MasterSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if(MasterSocket == -1)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);  //return 1
    }
    std::cout << "MasterSocket is "<< MasterSocket << std::endl;
    struct sockaddr_in SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(masterPort);
    SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    std::cout << "Binding is... ";
    int Result = bind(MasterSocket, (struct sockaddr *)&SockAddr, sizeof(SockAddr));

    if(Result == -1)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);  //return 1
    }
    std::cout << "OK"<< std::endl;

    set_nonblock(MasterSocket);

    std::cout << "Listening..."<< std::endl;

    Result = listen(MasterSocket, SOMAXCONN);

    if(Result == -1)
    {
        std::cout << strerror(errno) << std::endl;
        exit(1);//return 1;
    }

    struct epoll_event Event;
    Event.data.fd = MasterSocket;
    Event.events = EPOLLIN | EPOLLET;

    Events = (struct epoll_event *) calloc(MAX_EVENTS, sizeof(struct epoll_event));

    int EPoll = epoll_create1(0);
    epoll_ctl(EPoll, EPOLL_CTL_ADD, MasterSocket, &Event);

    while(true)
    {
        int N = epoll_wait(EPoll, Events, MAX_EVENTS, -1);
        for(unsigned int i = 0; i < N; i++)
        {
            if((Events[i].events & EPOLLERR)||(Events[i].events & EPOLLHUP))
            {
                std::cout << "User socket = "<< Events[i].data.fd
                          << " disconnected from Server" << std::endl;
                shutdown(Events[i].data.fd, SHUT_RDWR);
                auto it=Users.find(Events[i].data.fd);
                delete it->second;
                Users.erase(it);
                close(Events[i].data.fd);
            }
            else if(Events[i].data.fd == MasterSocket)
            {
                int SlaveSocket = accept(MasterSocket, 0, 0);
                set_nonblock(SlaveSocket);

                struct epoll_event Event;
                Event.data.fd = SlaveSocket;
                Event.events = EPOLLIN | EPOLLET;

                epoll_ctl(EPoll, EPOLL_CTL_ADD, SlaveSocket, &Event);

                User* newUser = new User();
                Users.insert(std::pair<int, User*>(SlaveSocket, newUser));

                std::cout << "New user socket = "<< SlaveSocket
                          << " connected to Server" << std::endl;
            }
            else
            {
                listeningFromSlaveDO(i);
            }
        }
    }
}


void MultyServer::listeningFromSlaveDO(int i)
{
    char Buffer[BUF_SIZE]={0};
    int RecvSize = recv(Events[i].data.fd, Buffer, BUF_SIZE, MSG_NOSIGNAL);
    if(Buffer[RecvSize-1]=='\n') Buffer[RecvSize-1]='\0';

    std::cout << "MSG from socket = " << Events->data.fd
              <<" is: " << Buffer << std::endl;

    auto it=Users.find(Events[i].data.fd);
    std::string inMsg (Buffer);

    std::cout << "Requesting for an Answer... ";
    Answer msg = it->second->exec(inMsg);
    std::cout << "Complited" << std::endl;

    std::string outMsg;
    outMsg.clear();
    outMsg+=msg.HEADER;
    outMsg+='\n';
    for (auto iter=msg.DATA.begin(); iter!=msg.DATA.end(); ++iter)
    {
           outMsg=outMsg+iter->first+" = "+iter->second + "\n";
    }
    std::cout<<"outMsg is string = "<<outMsg<<std::endl;

    memset(Buffer, 0, sizeof(Buffer));
    int j=0;
    for (auto iter = outMsg.begin(); iter != outMsg.end(); ++iter)
    {
        Buffer[j] = char(*iter);
        j++;
    }
    Buffer[j] = '\n';
    std::cout<<"outMsg is char* = "<< Buffer << std::endl
            <<"Sizeof out MSG is " << sizeof (Buffer)<<std::endl;

    send(Events[i].data.fd, Buffer, strlen(Buffer), MSG_NOSIGNAL);
}
