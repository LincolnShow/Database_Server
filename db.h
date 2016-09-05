#ifndef DB_H

#define DB_H
#include "units.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
/**
 * @brief The Database class
 *
 */
class DB {
public:
    /**
     * @brief DB constructor
     * @param path - defines the path where db will be created
     */
    DB(std::string path);
    ~DB();
    /**
     * @brief Executes a request and returns the result
     * @param req - Request to be executed
     * @return An Answer object
     */
    Answer handle(Request req);
    /**
     * @brief Return a path to a database
     * @return Path to a database
     */
    std::string path() const { return dbpath; }
    static void makeLow(std::string& s);
private:
    std::fstream dbfile;
    std::string dbpath;
    Answer add(std::string key, std::string value);
    Answer get(std::string key);
    Answer rm(std::string key);
    Answer rmAll();
    Answer find(std::string pattern);
};
#endif // DB_H
