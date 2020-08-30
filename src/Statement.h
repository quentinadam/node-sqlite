#ifndef STATEMENT_H
#define STATEMENT_H

#include <memory>
#include <sstream>
#include "sqlite3.h"
#include "Buffer.h"

using namespace std;

class Database;

class Statement {
  private:
    shared_ptr<Database> database;
    sqlite3_stmt* handle;
    string sql;
  public:
    Statement(shared_ptr<Database> database, sqlite3_stmt* handle, string sql);

    ~Statement();

    void bindInt(int index, int value);

    void bindDouble(int index, double value);

    void bindText(int index, string value);

    void bindBlob(int index, Buffer value);

    void bindNull(int index);

    int step();

    int columnCount();

    int columnType(int index);

    const char* columnName(int index);

    int columnValueInt(int index);

    double columnValueDouble(int index);

    string columnValueText(int index);

    Buffer columnValueBlob(int index);
};

#endif
