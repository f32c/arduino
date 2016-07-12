#ifndef VECTOR_RAM_H
#define VECTOR_RAM_H

#include <inttypes.h>
#include "Vector_REG.h"

extern "C"
{
  #include "vector_link.h"
}

class Vector_RAM
{
  public:
    struct vector_header_s *vh;
    Vector_RAM(int length) // create RAM-based vector
    {
      vh = create_segmented_vector(length, 0);
    }
    // vector store to RAM
    Vector_RAM& operator = (const Vector_REG& rhs)
    {
      vector_mmio[0] = (uint32_t)vh;
      vector_mmio[4] = 0x01800000 | rhs.number;
      vector_flush(vh);
      wait_vector_mask(1<<16);
      return *this;
    }
};

#endif
