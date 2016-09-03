#ifndef USER_H
#define USER_H
#include "db.h"
#include "parser.h"
#include <string>
#include <fstream>
#include <cstring>
const std::string USERFILE = "passwords.txt";
const std::string DBFOLDER = "dbs";
const char SEP = '/';
/**
 * @brief The User class allows users to log into system and execute commands
 */
class User {
public:
    User(){}
    ~User(){ if(userdb!=nullptr){ userdb->~DB(); userdb = nullptr;} }
    Answer exec(std::string s);
    bool isLogged() const { return logged; }
    bool isAdmin() const { return ((type == ADMIN) ? true : false); }
    DB* getDB() const { return userdb; }
private:
    enum USERTYPE{
        USER, ADMIN
    } type = USER;
    bool logged = false;
    DB* userdb = nullptr;

    Answer addUser(std::string login, std::string pass);
    Answer lsUsers();
    Answer rmUser(std::string login, bool hard = false);
    Answer passwd(std::string login, std::string pass);
    bool exists(std::string login);
};

#endif // USER_H
