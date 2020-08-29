
#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

class Buffer {
  public:
    const void* data;
    size_t length;
    Buffer(const void* data, size_t length): data(data), length(length) {}
};

#endif