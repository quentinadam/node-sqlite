#include <string.h>
#include "Statement.h"
#include "Exception.h"
#include "Database.h"

Statement::Statement(shared_ptr<Database> database, sqlite3_stmt* handle, string sql): database(database), handle(handle), sql(sql) {}

Statement::~Statement() {
  sqlite3_finalize(handle);
}

void Statement::bindInt(int index, int value) {
  if (sqlite3_bind_int(handle, index, value)) {
    ostringstream message;  
    message << "Could not bind int " << value << " at index " << index << " of query \"" << sql << "\"";
    database->throwError(message);
  }
}

void Statement::bindDouble(int index, double value) {
  if (sqlite3_bind_double(handle, index, value)) {
    ostringstream message;  
    message << "Could not bind double " << value << " at index " << index << " of query \"" << sql << "\"";
    database->throwError(message);
  }
}

void Statement::bindText(int index, string value) {
  char* copiedValue = new char[value.size() + 1];
  strcpy(copiedValue, value.c_str());
  if (sqlite3_bind_text(handle, index, copiedValue, -1, [](void* data) {
    delete[] ((char*) data);
  })) {
    ostringstream message;  
    message << "Could not bind text " << value << " at index " << index << " of query \"" << sql << "\"";
    database->throwError(message);
  }
}

void Statement::bindBlob(int index, Buffer value) {
  if (sqlite3_bind_blob(handle, index, value.data, value.length, NULL)) {
    ostringstream message;  
    message << "Could not bind blob at index " << index << " of query \"" << sql << "\"";
    database->throwError(message);
  }
}

void Statement::bindNull(int index) {
  if (sqlite3_bind_null(handle, index)) {
    ostringstream message;  
    message << "Could not bind null at index " << index << " of query \"" << sql << "\"";
    database->throwError(message);
  }
}

int Statement::step() {
  return sqlite3_step(handle);
}

int Statement::columnCount() {
  return sqlite3_column_count(handle);
}

int Statement::columnType(int index) {
  return sqlite3_column_type(handle, index);
}

const char* Statement::columnName(int index) {
  return sqlite3_column_name(handle, index);
}

int Statement::columnValueInt(int index)  {
  return sqlite3_column_int(handle, index);
}

double Statement::columnValueDouble(int index)  {
  return sqlite3_column_double(handle, index);
}

string Statement::columnValueText(int index)  {
  return string((const char*) sqlite3_column_text(handle, index));
}

Buffer Statement::columnValueBlob(int index)  {
  const void* data = sqlite3_column_blob(handle, index);
  size_t length = sqlite3_column_bytes(handle, index);
  return Buffer(data, length);
}
