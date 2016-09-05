#ifndef UNITS_H

#define UNITS_H
#include <vector>
#include <string>

/**
 * @brief The Request class stores user-request related information
 */
class Request{
public:
/**
 * @brief Enumeration type represents all possible commands
 */
    enum REQ_TYPES{
        NIL, ADD, GET, RM, RMALL, FIND, LOG, ADDUSER, LSUSERS, RMUSER, PASSWD
    };
    /**
     * @brief Type of request
     */
    REQ_TYPES type = NIL;
    /**
     * @brief Arguments for the request
     */
    std::vector<std::string> args;
    Request(){}
    /**
     * @brief Constructor
     * @param _type - type of the request
     * @param _args - arguments of the request
     */
    Request(REQ_TYPES _type, std::vector<std::string> _args) : type(_type), args(_args) {}
    /**
     * @brief Returns whether the command type is admin-only
     * @return true if the command type is admin-only, false otherwise
     */
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
    /**
     * @brief Contains metainformation
     */
    std::string HEADER;
    /**
     * @brief Contains data
     */
    std::vector<std::pair<std::string, std::string>> DATA;
    /**
     * @brief Constructor
     * @param _header - a string to be put in HEADER
     */
    Answer(std::string _header = "") : HEADER(_header){}
    /**
     * @brief Constructor
     * @param _header - a string to be put in HEADER
     * @param _data - vector of pairs to be put in DATA
     */
    Answer(std::string _header, std::vector<std::pair<std::string, std::string>> _data): HEADER(_header), DATA(_data) {}
};

#endif // UNITS_H
