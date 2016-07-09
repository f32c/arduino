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

void Vector::load(int i, struct vector_header_s *vh)
{
  if(vector_present)
  {
    vector_mmio[0] = (uint32_t) vh;
    vector_mmio[4] = 0x01000000 | (1<<(8+i)); // load vector (selected by bitmask)
    wait_vector_mask(1<<16);
  }
  else
    soft_vector_io(i, vh, 0);
}

void Vector::store(struct vector_header_s *vh, int i)
{
  if(vector_present)
  {
    vector_mmio[0] = (uint32_t) vh;
    vector_mmio[4] = 0x01800000 | i; // store vector (selected by index)
    vector_flush(vh);
    wait_vector_mask(1<<16);
  }
  else
    soft_vector_io(i, vh, 1);
}

void Vector::oper(int a, int b, int c, int op)
{
  if(vector_present)
    hard_vector_oper(a,b,c,op);
  else
    soft_vector_oper(a,b,c,op);
}

// a = b + c
void Vector::add(int a, int b, int c)
{
  if(vector_present)
  {
    vector_mmio[4] = 0x33000000 | a | (b<<4) | (c<<8); // a=b+c float (selected by index)
    wait_vector_mask(1<<a);
  }
  else
    soft_vector_oper(a,b,c,0);
}

// a = b - c
void Vector::sub(int a, int b, int c)
{
  if(vector_present)
  {
    vector_mmio[4] = 0x33010000 | a | (b<<4) | (c<<8); // a=b-c float (selected by index)
    wait_vector_mask(1<<a);
  }
  else
    soft_vector_oper(a,b,c,1);
}

// a = b * c
void Vector::mul(int a, int b, int c)
{
  if(vector_present)
  {
    vector_mmio[4] = 0x33020000 | a | (b<<4) | (c<<8); // a=b*c float (selected by index)
    wait_vector_mask(1<<a);
  }
  else
    soft_vector_oper(a,b,c,2);
}

// a = b / c
void Vector::div(int a, int b, int c)
{
  if(vector_present)
  {
    vector_mmio[4] = 0x34000000 | a | (b<<4) | (c<<8); // a=b/c float (selected by index)
    wait_vector_mask(1<<a);
  }
  else
    soft_vector_oper(a,b,c,3);
}

// a = i2f(b) integer to float
void Vector::i2f(int a, int b)
{
  if(vector_present)
  {
    vector_mmio[4] = 0x33040000 | a | (b<<4); // a=i2f(b) integer to float (selected by index)
    wait_vector_mask(1<<a);
  }
  else
    soft_vector_oper(a,b,0,4);
}

// a = f2i(b) float to integer
void Vector::f2i(int a, int b)
{
  if(vector_present)
  {
    vector_mmio[4] = 0x33050000 | a | (b<<4); // a=f2i(b) float to integer (selected by index)
    wait_vector_mask(1<<a);
  }
  else
    soft_vector_oper(a,b,0,5);
}
