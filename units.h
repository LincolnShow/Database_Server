#ifndef UNITS_H

#define UNITS_H
#include <vector>
#include <string>

/**
 * @brief The Request class stores user-request related information
 */
class Request{
public:
    enum REQ_TYPES{
        NIL, ADD, GET, RM, RMALL, FIND, LOG, ADDUSER, LSUSERS, RMUSER, PASSWD
    };
    REQ_TYPES type = NIL;
    std::vector<std::string> args;
    Request(){}
    Request(REQ_TYPES _type, std::vector<std::string> _args) : type(_type), args(_args) {}
    static bool isAdminCommand(REQ_TYPES r){
        if((r == ADDUSER) | (r == LSUSERS) | (r == RMUSER) | (r == PASSWD)){
            return true;
        }
        return false;
    }
};

/**
 * @brief The Answer class is used to return data from a database
 */
class Answer{
public:
    std::string HEADER;
    std::vector<std::pair<std::string, std::string>> DATA;

    Answer(std::string _header = "") : HEADER(_header){}
    Answer(std::string _header, std::vector<std::pair<std::string, std::string>> _data): HEADER(_header), DATA(_data) {}
};

#endif // UNITS_H
