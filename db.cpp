#include "db.h"
using namespace std;

DB::DB(std::string path)
{
    dbfile.open(dbpath = path, (fstream::out | fstream::app));
    dbfile.close();
}
DB::~DB(){
    if(dbfile.is_open()){
        dbfile.close();
    }
}

Answer DB::handle(Request req)
{
    switch(req.type){
    case Request::ADD:
        return add(req.args[0], req.args[1]);
        break;
    case Request::GET:
        return get(req.args[0]);
        break;
    case Request::RM:
        return rm(req.args[0]);
        break;
    case Request::RMALL:
        return rmAll();
        break;
    case Request::FIND:
        return find(req.args[0]);
        break;
    default:
        return Answer();
        break;
    }
}

Answer DB::add(string key, string value)
{
    if(key.size() > 255){
        key = key.substr(0, 255);
    }
    if(value.size() > 255){
        value = value.substr(0, 255);
    }
    makeLow(key);
    makeLow(value);
    bool isNew = get(key).HEADER != "GET@SUCCESS";
    dbfile.open(dbpath, (fstream::out | fstream::app));
    if(dbfile.is_open() && isNew){
        dbfile << key+"="+value+"\n";
        dbfile.close();
        return Answer("ADD@SUCCESS");
    }
    return Answer("ADD@FAIL");
}

Answer DB::get(string key)
{
    makeLow(key);
    bool result = false;
    dbfile.open(dbpath, fstream::in);
    if(dbfile.is_open()){
        string buf;
        pair<string,string> result_pair;
        while(std::getline(dbfile, buf)){
            makeLow(buf);
            if(buf.find(key+"=") != string::npos){
                result_pair = pair<string,string>(key, buf.substr(key.size()+1));
                result = true;
                break;
            }
        }
        dbfile.close();
        if(result == true){
            return Answer("GET@SUCCESS", vector<pair<string,string>>{result_pair});
        }
    }
    return Answer("GET@FAIL");
}

Answer DB::rm(string key)
{
    makeLow(key);
    dbfile.open(dbpath, (fstream::in));
    fstream tmp;
    tmp.open(dbpath+".tmp", fstream::out);
    bool result = false;
    if(dbfile.is_open() && tmp.is_open()){
        string buf;
        while(std::getline(dbfile, buf)){
            makeLow(buf);
            if(buf.find(key+"=") == string::npos){
                tmp << buf+"\n";
            }
            else{
                result = true;
            }
        }
        tmp.close();
        dbfile.close();
        remove(dbpath.c_str());
        rename((dbpath+".tmp").c_str(), dbpath.c_str());
    }
    if(result == true){
        return Answer("RM@SUCCESS");
    }
    return Answer("RM@FAIL");

}

Answer DB::rmAll()
{
    bool result = false;
    dbfile.open(dbpath, fstream::out);
    if(dbfile.is_open()){
        result = true;
    }
    dbfile.close();
    if(result == true){
        return Answer("RMALL@SUCCESS");
    }
    return Answer("RMALL@FAIL");
}

Answer DB::find(string pattern)
{
    makeLow(pattern);
    dbfile.open(dbpath, fstream::in);
    if(dbfile.is_open()){
        vector<pair<string,string>> result;
        string buf;
        while(std::getline(dbfile, buf)){
            string key = buf.substr(0, buf.find('='));
            string value = buf.substr(buf.find('=')+1);
            makeLow(value);
            if(regex_match(value, std::regex(pattern))){
                result.push_back(pair<string,string>(key,value));
            }
        }
        dbfile.close();
        if(!result.empty()){
            return Answer("FIND@SUCCESS", result);
        }
    }
    return Answer("FIND@FAIL");
}

void DB::makeLow(string& s){
    for(string::size_type i = 0; i < s.length(); ++i){
        s[i] = tolower(s[i]);
    }
}
