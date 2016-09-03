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
    if(!strcasecmp(buf.c_str(), "ADD")){
        rtype = Request::ADD;
    } else if(!strcasecmp(buf.c_str(), "GET")){
        rtype = Request::GET;
    } else if(!strcasecmp(buf.c_str(), "RM")){
        rtype = Request::RM;
    } else if(!strcasecmp(buf.c_str(), "RMALL")){
        rtype = Request::RMALL;
    } else if(!strcasecmp(buf.c_str(), "FIND")){
        rtype = Request::FIND;
    } else if(!strcasecmp(buf.c_str(), "LOG")){
        rtype = Request::LOG;
    } else if(!strcasecmp(buf.c_str(), "ADDUSER")){
        rtype = Request::ADDUSER;
    } else if(!strcasecmp(buf.c_str(), "LSUSERS")){
        rtype = Request::LSUSERS;
    } else if(!strcasecmp(buf.c_str(), "RMUSER")){
        rtype = Request::RMUSER;
    } else if(!strcasecmp(buf.c_str(), "PASSWD")){
        rtype = Request::PASSWD;
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
    return Request(rtype, rargs);
}
