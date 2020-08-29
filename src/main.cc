#include <string>
#include <vector>
#include <memory>
#include "Database.h"
#include "Environment.h"
#include "Value.h"

using namespace std;

napi_value open(napi_env env, napi_callback_info info) {
  Environment environment(env, info);
  try {
    vector<Value> arguments = environment.getArguments(1);
    string name = arguments[0].asString();
    auto database = make_shared<Database>(name);
    return environment.createExternal(database).nativeValue();
  } catch (Exception error) {
    environment.throwError(error);
    return NULL;
  }
}

napi_value close(napi_env env, napi_callback_info info) {
  Environment environment(env, info);
  try {
    vector<Value> arguments = environment.getArguments(1);
    auto database = *((shared_ptr<Database>*) arguments[0].asExternal());
    database->close();
    return NULL;
  } catch (Exception error) {
    environment.throwError(error);
    return NULL;
  }
}

napi_value query(napi_env env, napi_callback_info info) {
  Environment environment(env, info);
  try {
    vector<Value> arguments = environment.getArguments(3);
    auto database = *((shared_ptr<Database>*) arguments[0].asExternal());
    string sql = arguments[1].asString();
    Statement statement = database->prepare(sql);
    if (!arguments[2].isArray()) {
      throw Exception("Expecting array");
    }
    for (uint32_t i = 0; i < arguments[2].getArrayLength(); i++) {
      Value value = arguments[2].getElement(i);
      if (value.isNumber()) {
        double doubleValue = value.asDouble();
        int32_t intValue = value.asInt32();
        if ((double) intValue == doubleValue) {
          statement.bindInt(i + 1, intValue); 
        } else {
          statement.bindDouble(i + 1, doubleValue); 
        }
      } else if (value.isBoolean()) {
        statement.bindInt(i + 1, value.asBoolean() ? 1 : 0); 
      } else if (value.isString()) {
        statement.bindText(i + 1, value.asString()); 
      } else if (value.isBuffer()) {
        statement.bindBlob(i + 1, value.asBuffer()); 
      } else if (value.isNull()) {
        statement.bindNull(i + 1); 
      } else if (value.isUndefined()) {
        statement.bindNull(i + 1); 
      } else {
        throw Exception("Unsupported value type");
      }
    }
    Value result = environment.createArray();
    while (true) {
      int status = statement.step();
      if (status == SQLITE_DONE) {
        break;
      } else if (status == SQLITE_ROW) {
        Value row = environment.createObject();
        for (int i = 0; i < statement.columnCount(); i++) {
          string key(statement.columnName(i));
          int type = statement.columnType(i);
          if (type == SQLITE_INTEGER) {
            Value value = environment.createInt32(statement.columnValueInt(i));
            row.setProperty(key, value);
          } else if (type == SQLITE_FLOAT) {
            Value value = environment.createDouble(statement.columnValueDouble(i));
            row.setProperty(key, value);
          } else if (type == SQLITE_TEXT) {
            Value value = environment.createString(statement.columnValueText(i));
            row.setProperty(key, value);
          } else if (type == SQLITE_BLOB) {
            Value value = environment.createBuffer(statement.columnValueBlob(i));
            row.setProperty(key, value);
          } else if (type == SQLITE_NULL) {
            Value value = environment.createNull();
            row.setProperty(key, value);
          } else {
            throw Exception("Unexpected column type");
          }
        }
        result.setElement(result.getArrayLength(), row);
      } else {
        ostringstream message;  
        message << "Could not execute query \"" << sql << "\"";
        database->throwError(message);
      }
    }
    return result.nativeValue();
  } catch (Exception error) {
    environment.throwError(error);
    return NULL;
  }
}

napi_value Init(napi_env env, napi_value exports) {
  napi_value openFunction;
  if (napi_create_function(env, NULL, 0, open, NULL, &openFunction) != napi_ok) {
      napi_throw_error(env, NULL, "Unable to wrap native function");
  }
  if (napi_set_named_property(env, exports, "open", openFunction) != napi_ok) {
      napi_throw_error(env, NULL, "Unable to populate exports");
  }

  napi_value queryFunction;
  if (napi_create_function(env, NULL, 0, query, NULL, &queryFunction) != napi_ok) {
      napi_throw_error(env, NULL, "Unable to wrap native function");
  }
  if (napi_set_named_property(env, exports, "query", queryFunction) != napi_ok) {
      napi_throw_error(env, NULL, "Unable to populate exports");
  }

  napi_value closeFunction;
  if (napi_create_function(env, NULL, 0, close, NULL, &closeFunction) != napi_ok) {
      napi_throw_error(env, NULL, "Unable to wrap native function");
  }
  if (napi_set_named_property(env, exports, "close", closeFunction) != napi_ok) {
      napi_throw_error(env, NULL, "Unable to populate exports");
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)