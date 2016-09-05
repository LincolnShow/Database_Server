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
    ~User(){ if(userdb!=nullptr){ delete userdb; userdb = nullptr;} }
    /**
     * @brief Executes the command and returns an Answer object
     * @param s - text command to be executed
     */
    Answer exec(std::string s);
    /**
     * @brief Tells whether user is logged or not
     */
    bool isLogged() const { return logged; }
    /**
     * @brief Tells whether user is admin or not
     */
    bool isAdmin() const { return ((type == ADMIN) ? true : false); }
    /**
     * @brief Returns a pointer to the database associated with user
     */
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
