#ifndef DB_H

#define DB_H
#include "units.h"
#include <fstream>
#include <iostream>
/**
 * @brief The Database class
 *
 */
class DB {
public:
    DB(std::string path);
    ~DB();
    Answer handle(Request& req);
private:
    std::fstream dbfile;

    Answer add(std::string& key, std::string& value);
    Answer get(std::string& key);
    Answer rm(std::string& key);
    Answer rmAll();
    Answer find(std::string& pattern);
};

#endif // DB_H
