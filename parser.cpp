#include "parser.h"
using namespace std;
Request Parser::parseRequest(std::string s) {
    string buf = s.substr(0, s.find(':'));
    string::size_type arg1 = s.find(':');
    string::size_type arg2 = s.find('=');
    vector<string> rargs;
    if(arg1 != string::npos){
        rargs.push_back(s.substr(arg1+1, (arg2-arg1-1)));
    }
    if(arg2 != string::npos){
        rargs.push_back(s.substr(arg2+1));
    }
    Request::REQ_TYPES rtype = Request::NIL;
    int numOfArgs = -1;
    if(buf == "ADD"){
        rtype = Request::ADD;
        numOfArgs = 2;
    } else if(buf == "GET"){
        rtype = Request::GET;
        numOfArgs = 1;
    } else if(buf == "RM"){
        rtype = Request::RM;
        numOfArgs = 1;
    } else if(buf == "RMALL"){
        rtype = Request::RMALL;
        numOfArgs = 0;
    } else if(buf == "FIND"){
        rtype = Request::FIND;
        numOfArgs = 1;
    } else if(buf == "LOG"){
        rtype = Request::LOG;
        numOfArgs = 2;
    } else if(buf == "ADDUSER"){
        rtype = Request::ADDUSER;
        numOfArgs = 2;
    } else if(buf == "LSUSERS"){
        rtype = Request::LSUSERS;
        numOfArgs = 0;
    } else if(buf == "RMUSER"){
        rtype = Request::RMUSER;
        numOfArgs = 1;
    } else if(buf == "PASSWD"){
        rtype = Request::PASSWD;
        numOfArgs = 2;
    } else {
        return Request();
    }
    if(rtype != Request::FIND){
        for(auto i = rargs.begin(); i != rargs.end(); ++i){
            if(!regex_match((*i), std::regex("[a-zA-Z0-9]*"))){
                return Request();
            }
        }
    }
    if(rargs.size() < numOfArgs){
        return Request();
    }
    return Request(rtype, rargs);
}
