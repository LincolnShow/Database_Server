#ifndef MULTYSERVER_H
#define MULTYSERVER_H

#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <sys/epoll.h>

#include <errno.h>
#include <string.h>

#include <map>

#include "config.h"
#include "user.h"

class User;

/**
 * @brief Initialises Network Connection, Creates User Objects and executes its
 *
 */
class MultyServer
{
protected:
    struct epoll_event * Events;
    int MasterSocket;
    std::map <int, User *> Users;

    /**
     * @brief Unblocks Socket
     * @param fd - Socket descriptor
     * @return Returns -1 if error
     */
    int set_nonblock(int fd);

    /**
     * @brief Recieves/Sends messages and executes nessesary User's object with incoming string as a parametr.
     * @param fd - Socket descriptor
     * @return Returns -1 if error
     */
    void listeningFromSlaveDO(int i);

public:
    /**
     * @brief MultyServer constructor. Initialises connection with clients. Waits for incoming message.
     * @param masterPort - Listening port
     *
     */
    MultyServer(int masterPort);
};

#endif // MULTYSERVER_H
