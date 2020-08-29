#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <node_api.h>
#include "Buffer.h"
#include "Exception.h"

using namespace std;

class Value {
  private:
    napi_env env;
    napi_value value;
  public:
    Value(napi_env env, napi_value value): env(env), value(value) {}

    string asString() {
      size_t length;
      if (napi_get_value_string_utf8(env, value, NULL, 0, &length) != napi_ok) {
        throw Exception("Expecting string");
      }
      vector<char> chars(length + 1, 0);
      if (napi_get_value_string_utf8(env, value, &chars[0], length + 1, &length) != napi_ok) {
        throw Exception("Expecting string");
      }
      return string(&chars[0]);
    }

    uint64_t asBigint64() {
      uint64_t result;
      bool lossless;
      if (napi_get_value_bigint_uint64(env, value, &result, &lossless) != napi_ok) {
        throw Exception("Expecting bigint");
      }
      if (!lossless) {
        throw Exception("Expecting bigint");
      }
      return result;
    }

    double asDouble() {  
      double result;
      if (napi_get_value_double(env, value, &result) != napi_ok) {
        throw Exception("Expecting number");
      }
      return result;
    }

    int32_t asInt32() {  
      int32_t result;
      if (napi_get_value_int32(env, value, &result) != napi_ok) {
        throw Exception("Expecting number");
      }
      return result;
    }

    bool asBoolean() {  
      bool result;
      if (napi_get_value_bool(env, value, &result) != napi_ok) {
        throw Exception("Expecting number");
      }
      return result;
    }

    Buffer asBuffer() {  
      void* data;
      size_t length;
      if (napi_get_buffer_info(env, value, &data, &length) != napi_ok) {
        throw Exception("Expecting Buffer");
      }
      return Buffer(data, length);
    }

    void* asExternal() {  
      void* result;
      if (napi_get_value_external(env, value, &result) != napi_ok) {
        throw Exception("Expecting external");
      }
      return result;
    }

    bool isArray() {
      bool result;
      if (napi_is_array(env, value, &result) != napi_ok) {
        throw Exception("Error napi_is_array");
      }
      return result;
    }

    bool isBuffer() {
      bool result;
      if (napi_is_buffer(env, value, &result) != napi_ok) {
        throw Exception("Error napi_is_buffer");
      }
      return result;
    }

    bool isString() {
      napi_valuetype type;
      if (napi_typeof(env, value, &type) != napi_ok) {
        throw Exception("Error napi_typeof");
      }
      return (type == napi_string);
    }

    bool isNumber() {
      napi_valuetype type;
      if (napi_typeof(env, value, &type) != napi_ok) {
        throw Exception("Error napi_typeof");
      }
      return (type == napi_number);
    }

    bool isBoolean() {
      napi_valuetype type;
      if (napi_typeof(env, value, &type) != napi_ok) {
        throw Exception("Error napi_typeof");
      }
      return (type == napi_boolean);
    }

    bool isUndefined() {
      napi_valuetype type;
      if (napi_typeof(env, value, &type) != napi_ok) {
        throw Exception("Error napi_typeof");
      }
      return (type == napi_undefined);
    }

    bool isNull() {
      napi_valuetype type;
      if (napi_typeof(env, value, &type) != napi_ok) {
        throw Exception("Error napi_typeof");
      }
      return (type == napi_null);
    }

    Value getElement(uint32_t index) {
      napi_value result;
      if (napi_get_element(env, value, index, &result) != napi_ok) {
        throw Exception("Error napi_get_element");
      }
      return Value(env, result);
    }

    void setElement(uint32_t index, Value value) {
      if (napi_set_element(env, this->value, index, value.nativeValue()) != napi_ok) {
        throw Exception("Error napi_set_element");
      }
    }

    void setProperty(string key, Value value) {
      if (napi_set_named_property(env, this->value, key.c_str(), value.nativeValue()) != napi_ok) {
        throw Exception("Error napi_set_property");
      }
    }

    uint32_t getArrayLength() {
      uint32_t result;
      if (napi_get_array_length(env, value, &result) != napi_ok) {
        throw Exception("Error napi_get_array_length");
      }
      return result;
    }

    napi_value nativeValue() {
      return value;
    }
};

#endif
