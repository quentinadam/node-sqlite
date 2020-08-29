#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <sstream>

using namespace std;

class Exception : public exception {
  private: 
    string message;
  public:
    Exception(string message): message(message) {}
    Exception(const char* message): message(message) {}
    Exception(ostringstream& message): message(message.str()) {}
    const char * what () const throw () {
      return message.c_str();
    }
};

#endif
