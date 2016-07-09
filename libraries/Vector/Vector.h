#ifndef VECTOR_H
#define VECTOR_H

//#include <Arduino.h>
#include <inttypes.h>

extern "C"
{
  #include "vector_link.h"
}

class Vector
{
  private:
    uint8_t vector_present;

  public:
    Vector();
    void dumpreg();
    void test(int i);
    struct vector_header_s *create(int n);
    void print(struct vector_header_s *v);
    void io(int i, struct vector_header_s *vh, int store_mode);
    void oper(int a, int b, int c, int oper);
};

#endif
