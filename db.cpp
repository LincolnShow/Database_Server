#include "db.h"
using namespace std;

DB::DB(std::string path)
{
    dbfile.open(path, (fstream::out | fstream::app));
    dbfile.close();
}
DB::~DB(){
    if(dbfile.is_open()){
        dbfile.close();
    }
}

Answer DB::handle(Request &req)
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
