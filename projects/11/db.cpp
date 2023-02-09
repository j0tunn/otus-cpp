#include <stdexcept>
#include <string>
#include <sstream>
#include "db.h"

using namespace std;

DataBase::DataBase()
    : handle_(nullptr)
{
    if (sqlite3_open_v2("db.sqlite", &handle_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX, NULL)) {
        throw logic_error(string("Can't open database: ") + sqlite3_errmsg(handle_));
    }

    exec_("CREATE TABLE IF NOT EXISTS A (id int primary key, name text);");
    exec_("CREATE TABLE IF NOT EXISTS B (id int primary key, name text);");
}

DataBase::~DataBase() {
    if (handle_) {
        sqlite3_close(handle_);
    }
}

void DataBase::insert(const string& table, const string& id, const string& name) {
    exec_((stringstream() << "INSERT INTO " << table << " VALUES(" << id << ", '" << name << "');").str());
}

void DataBase::truncate(const string& table) {
    exec_((stringstream() << "DELETE FROM " << table << "; VACUUM;").str());
}

string DataBase::getIntersection() {
    return execRead_("SELECT A.id, A.name, B.name FROM A, B WHERE A.id == B.id");
}

string DataBase::getSymmetricDifference() {
    return execRead_("SELECT CASE WHEN A.id IS NOT NULL THEN A.id ELSE B.id END as id, A.name, B.name FROM"
        " A FULL OUTER JOIN B ON A.id == B.id WHERE A.name IS NULL OR B.name IS NULL");
}

string DataBase::execRead_(const string& sql) {
    stringstream outStream;

    exec_(sql, [](void* pCtx, int columns, char **data, char **names) -> int {
        stringstream* pOut = reinterpret_cast<stringstream*>(pCtx);

        for (int i = 0; i < columns; ++i) {
            *pOut << (data[i] ? data[i] : "") << (i < columns - 1 ? "," : "");
        }

        *pOut << endl;

        return 0;
    }, &outStream);

    return outStream.str();
}

void DataBase::exec_(const string& sql, DataBase::CallbackPtr pCb, stringstream* pOut) {
    char* errMsg;
    int rc = sqlite3_exec(handle_, sql.data(), pCb, pOut, &errMsg);
    if (rc != SQLITE_OK) {
        const string err = string("Can't execute query: ") + errMsg;
        sqlite3_free(errMsg);

        throw logic_error(err);
    }
}
