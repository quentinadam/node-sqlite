#ifndef DATABASE_H
#define DATABASE_H

#include <memory>
#include <string>
#include "sqlite3.h"
#include "Exception.h"
#include "Statement.h"

using namespace std;

class Database: public enable_shared_from_this<Database> {
  private:
    sqlite3* handle;

  public:
    Database(sqlite3* handle);

    Database(string name);

    ~Database();

    Statement prepare(string sql);

    const char* errorMessage();

    void close();

    void throwError(string message);

    void throwError(ostringstream& message);

};

#endif
