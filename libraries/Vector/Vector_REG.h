#ifndef VECTOR_REG_H
#define VECTOR_REG_H

#include <inttypes.h>
#include "Vector_RAM.h"

extern "C"
{
  #include "vector_link.h"
}

enum vector_operation { VECTOR_ADD=0, VECTOR_SUB=1, VECTOR_MUL=2, VECTOR_DIV=3 };

class Vector_REG
{
  public:
    uint32_t number, number_lhs, number_rhs, operation; // hardware vector register number, usually 0-7
    Vector_REG(uint8_t n):number(n){}; // constructor sets register number
    Vector_REG& operator += (const Vector_REG& rhs)
    { // memorize lhs, rhs and operator type to use at assignment
      number_lhs = number;
      number_rhs = rhs.number;
      operation = VECTOR_ADD;
      return *this;
    }
    Vector_REG& operator -= (const Vector_REG& rhs)
    { // memorize lhs, rhs and operator type to use at assignment
      number_lhs = number;
      number_rhs = rhs.number;
      operation = VECTOR_SUB;
      return *this;
    }
    Vector_REG& operator *= (const Vector_REG& rhs)
    { // memorize lhs, rhs and operator type to use at assignment
      number_lhs = number;
      number_rhs = rhs.number;
      operation = VECTOR_MUL;
      return *this;
    }
    Vector_REG& operator /= (const Vector_REG& rhs)
    { // memorize lhs, rhs and operator type to use at assignment
      number_lhs = number;
      number_rhs = rhs.number;
      operation = VECTOR_DIV;
      return *this;
    }
    Vector_REG& operator = (const Vector_REG& rhs) // assignment performs actual vector hardware operation
    {
      uint32_t vector_opcode[4] = 
      {
        0x33000000, // VECTOR_ADD
        0x33010000, // VECTOR_SUB
        0x33020000, // VECTOR_MUL
        0x34000000, // VECTOR_DIV
      };
      vector_mmio[4] = vector_opcode[rhs.operation] | number | (rhs.number_lhs<<4) | (rhs.number_rhs<<8); // a=b+c float (selected by index)
      wait_vector_mask(1<<number);
      return *this;
    }
    // vector load from RAM
    Vector_REG& operator = (const class Vector_RAM& rhs);
};

Vector_REG operator + (Vector_REG&, const Vector_REG&);
Vector_REG operator - (Vector_REG&, const Vector_REG&);
Vector_REG operator * (Vector_REG&, const Vector_REG&);
Vector_REG operator / (Vector_REG&, const Vector_REG&);

#endif
