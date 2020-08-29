
#include "Database.h"
#include "Exception.h"

Statement Database::prepare(string sql) {
  sqlite3_stmt* statement;
  if (sqlite3_prepare_v2(handle, sql.c_str(), -1, &statement, NULL)) {
    ostringstream message;  
    message << "Could not prepare statement \"" << sql << "\"";
    throwError(message);
  }
  return Statement(shared_from_this(), statement, sql);
}

Database::Database(sqlite3* handle): handle(handle) {}

Database::Database(string name) {
  if (sqlite3_open(name.c_str(), &handle)) {
    ostringstream message;  
    message << "Could not open database \"" << name << "\": " << errorMessage();
    throwError(message);
  }
}

Database::~Database() {
  close();
}

const char* Database::errorMessage() {
  return sqlite3_errmsg(handle);
}

void Database::close() {
  sqlite3_close(handle);
}

void Database::throwError(string message) {
  ostringstream stream;  
  stream << message << ": " << errorMessage();
  throw Exception(stream);
}

void Database::throwError(ostringstream& message) {
  throwError(message.str());
}