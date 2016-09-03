#ifndef PARSER_H
#define PARSER_H
#include "units.h"
#include <string>
#include <cstring>
#include <regex>
/**
 * @brief The Parser class is used for parsing strings into Request objects
 */
class Parser {
private:
    Parser();
    ~Parser();
    Parser(const Parser&);
public:
    static Request parseRequest(std::string s);
};

#endif // PARSER_H
