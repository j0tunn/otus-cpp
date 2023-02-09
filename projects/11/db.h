#pragma once

#include <string>
#include <functional>
#include <sstream>
#include "sqlite3.h"

class DataBase {
public:
    DataBase();
    ~DataBase();

    void insert(const std::string& table, const std::string& id, const std::string& name);
    void truncate(const std::string& table);
    std::string getIntersection();
    std::string getSymmetricDifference();

private:
    std::string execRead_(const std::string& sql);

    typedef int(*CallbackPtr)(void*,int,char**,char**);
    void exec_(const std::string& sql, CallbackPtr pCb = nullptr, std::stringstream* pOut = nullptr);
    sqlite3* handle_;
};
