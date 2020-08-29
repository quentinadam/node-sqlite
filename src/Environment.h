#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <node_api.h>
#include "Value.h"
#include "Exception.h"

class Environment {
  private:
    napi_env env;
    napi_callback_info info;
  public:
    Environment(napi_env env, napi_callback_info info): env(env), info(info) {}

    vector<Value> getArguments(size_t count) {
      size_t argc = count;
      vector<napi_value> argv(argc);
      if (napi_get_cb_info(env, info, &argc, &argv[0], NULL, NULL) != napi_ok) {
        throw Exception("Failed to parse arguments");
      }
      if (argc < count) {
        ostringstream message;  
        message << "Expecting " << count << " arguments";
        throw Exception(message.str());
      }
      vector<Value> arguments;
      for (size_t i = 0; i < sizeof(argv); i++) {
        arguments.push_back(Value(env, argv[i]));
      }
      return arguments;
    }

    Value createObject() {
      napi_value result;
      if (napi_create_object(env, &result) != napi_ok) {
        throw Exception("Error napi_create_object");
      }
      return Value(env, result);
    }

    Value createString(string value) {
      return createString(value.c_str());
    }

    Value createString(const char* value) {
      napi_value result;
      if (napi_create_string_utf8(env, value, NAPI_AUTO_LENGTH, &result) != napi_ok) {
        throw Exception("Error napi_create_string");
      }
      return Value(env, result);
    }

    Value createString(const unsigned char* value) {
      return createString((const char*) value);
    }

    Value createInt32(int32_t value) {
      napi_value result;
      if (napi_create_int32(env, value, &result) != napi_ok) {
        throw Exception("Error napi_create_int32");
      }
      return Value(env, result);
    }

    Value createDouble(double value) {
      napi_value result;
      if (napi_create_double(env, value, &result) != napi_ok) {
        throw Exception("Error napi_create_double");
      }
      return Value(env, result);
    }

    Value createBigint64(uint64_t value) {
      napi_value result;
      if (napi_create_bigint_uint64(env, value, &result) != napi_ok) {
        throw Exception("Error napi_create_bigint_uint64");
      }
      return Value(env, result);
    }

    Value createNull() {
      napi_value result;
      if (napi_get_null(env, &result) != napi_ok) {
        throw Exception("Error napi_get_null");
      }
      return Value(env, result);
    }

    Value createUndefined() {
      napi_value result;
      if (napi_get_undefined(env, &result) != napi_ok) {
        throw Exception("Error napi_get_undefined");
      }
      return Value(env, result);
    }

    Value createArray() {
      napi_value result;
      if (napi_create_array(env, &result) != napi_ok) {
        throw Exception("Error napi_create_array");
      }
      return Value(env, result);
    }

    Value createBuffer(Buffer buffer) {
      napi_value result;
      if (napi_create_buffer_copy(env, buffer.length, buffer.data, NULL, &result) != napi_ok) {
        throw Exception("Error napi_create_array");
      }
      return Value(env, result);
    }

    template<typename T>
    Value createExternal(T value) {
      napi_value result;
      if (napi_create_external(env, new T(value), [](napi_env env, void* finalize_data, void* finalize_hint) {
        delete ((T*) finalize_data);
      }, NULL, &result) != napi_ok) {
        throw Exception("Error napi_create_external");
      }
      return Value(env, result);
    }

    void throwError(Exception exception) {
      napi_throw_error(env, NULL, exception.what());
    }

    void throwError(const char* message) {
      napi_throw_error(env, NULL, message);
    }
};

#endif
