#include "user.h"

using namespace std;
Answer User::exec(std::string s) {
    Request r = Parser::parseRequest(s);
    if(!logged){
        if((r.type == Request::LOG)){
            fstream uf;
            uf.open(USERFILE, fstream::in);
            if(!uf.is_open()){
                uf.open(USERFILE, fstream::out);
                uf.close();
                uf.open(USERFILE, fstream::in);
            }
            if(uf.is_open()){
                string s;
                while(std::getline(uf, s)){
                    if(regex_match(s, std::regex(r.args[0]+"="+r.args[1]+":.*"))){
                        if(r.args[0] == "admin"){
                            type = ADMIN;
                        }
                        logged = true;
                        if(type != ADMIN){
                            userdb = new DB(s.substr(s.find(':')+1));
                        }
                        uf.close();
                        return Answer("LOG@SUCCESS");
                    }
                }
                return Answer("LOG@FAIL");
            }
        }
        return Answer("LOGIN REQUIRED");
    }
    else{
        if(Request::isAdminCommand(r.type)){
            if(type == ADMIN){
                switch(r.type){
                case Request::ADDUSER:
                    return addUser(r.args[0], r.args[1]);
                case Request::LSUSERS:
                    return lsUsers();
                case Request::RMUSER:
                    return rmUser(r.args[0], true);
                case Request::PASSWD:
                    return passwd(r.args[0], r.args[1]);
                }
            }
            else{
                return Answer("PERMISSION DENIED");
            }
        }
        else if (type != ADMIN){
            return userdb->handle(r);
        }
    }
    return Answer("SYNTAX ERROR");
}

Answer User::addUser(string login, string pass)
{
    fstream uf;
    uf.open(USERFILE, (fstream::out | fstream::app));
    if(uf.is_open() && !exists(login)){
        uf << login+"="+pass+ ":" + DBFOLDER + SEP + login + ".db\n";
        uf.close();
        return Answer("ADDUSER@SUCCESS");
    }
    return Answer("ADDUSER@FAIL");
}

Answer User::lsUsers()
{
    fstream uf;
    uf.open(USERFILE, fstream::in);
    if(uf.is_open()){
        vector<pair<string,string>> result;
        string buf;
        while(std::getline(uf, buf)){
            pair<string, string> tmp;
            string::size_type sep = buf.find(':');
            if(sep != string::npos){
                tmp.first = buf.substr(0, sep);
                tmp.second = buf.substr(sep+1);
            }
            else{
                tmp.first = buf;
            }
            result.push_back(tmp);
        }
        return Answer("LSUSERS@SUCCESS", result);
    }
    return Answer("LSUSERS@FAIL");
}

Answer User::rmUser(string login, bool hard)
{
    std::regex urgx(login + "=.*");
    fstream uf;
    uf.open(USERFILE, (fstream::in));
    fstream tmp;
    tmp.open(USERFILE+".tmp", fstream::out);
    bool result = false;
    if(uf.is_open() && tmp.is_open()){
        string buf;
        while(std::getline(uf, buf)){
            if(!regex_match(buf, urgx)){
                tmp << buf+"\n";
            }
            else{
                if(hard && (buf.find(':') != string::npos)){
                    remove(buf.substr(buf.find(':')+1).c_str());
                }
                result = true;
            }
        }
        tmp.close();
        uf.close();
        remove(USERFILE.c_str());
        rename((USERFILE+".tmp").c_str(), USERFILE.c_str());
    }
    if(result == true){
        return Answer("RMUSER@SUCCESS");
    }
    return Answer("RMUSER@FAIL");
}

Answer User::passwd(string login, string pass)
{
    std::regex urgx(login + "=.*");
    fstream uf;
    uf.open(USERFILE, (fstream::in));
    fstream tmp;
    tmp.open(USERFILE+".tmp", fstream::out);
    bool result = false;
    if(uf.is_open() && tmp.is_open()){
        string buf;
        while(std::getline(uf, buf)){
            if(!regex_match(buf, urgx)){
                tmp << buf+"\n";
            }
            else{
                string::size_type sep = buf.find(':');
                string udb;
                if(sep!=string::npos){
                    udb = buf.substr(sep);
                }
                tmp << login+"="+pass+udb+"\n";
                result = true;
            }
        }
        tmp.close();
        uf.close();
        remove(USERFILE.c_str());
        rename((USERFILE+".tmp").c_str(), USERFILE.c_str());
    }
    if(result == true){
        return Answer("PASSWD@SUCCESS");
    }
    return Answer("PASSWD@FAIL");
}

bool User::exists(string login)
{
    bool result = false;
    fstream uf;
    uf.open(USERFILE, fstream::in);
    if(uf.is_open()){
        string buf;
        while(std::getline(uf, buf)){
            buf = buf.substr(0, buf.find('='));
            if(login == buf){
                result = true;
                break;
            }
        }
        uf.close();
    }
    return result;
}
