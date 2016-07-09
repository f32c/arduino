#include "Vector.h"

extern "C"
{
  #include "vector_link.h"
  #include "run_test.h"
}

Vector::Vector()
{
  // todo: autodetection of the vector unit
  vector_present = vector_detect();
  soft_alloc_vector_registers(); // allocate regs for vector emulation
  // clear any interrupts
  hard_init();
}
 
void Vector::dumpreg()
{
  vector_dumpreg();
  return;
}

void Vector::test(int i)
{
  run_test(i);
}

struct vector_header_s *Vector::create(int n)
{
  return create_segmented_vector(n, 0);
}

void Vector::print(struct vector_header_s *v)
{
  printvector(v, 0, 2048);
}

void Vector::io(int i, struct vector_header_s *vh, int store_mode)
{
  if(vector_present)
    hard_vector_io(i, vh, store_mode);
  else
    soft_vector_io(i, vh, store_mode);
}

void Vector::oper(int a, int b, int c, int op)
{
  if(vector_present)
    hard_vector_oper(a,b,c,op);
  else
    soft_vector_oper(a,b,c,op);
}
