#ifndef VECTOR_LINK_H
#define VECTOR_LINK_H

//#include <stdio.h>
//#include <unistd.h>
#include <stdint.h>
//#include <stdlib.h>
//#include <math.h>

/* linked list vector test header file*/

#define EXPONENT_BITS 8
#define MANTISSA_BITS 23

#define EXPONENT_OFFSET ((1<<(EXPONENT_BITS-1))-1)

// soft-math will use internal goldschmidt's
// division instead of C-library floating point division
#define SOFT_USE_GOLDSCHMIDT 1

// hardware routines will use software math
#define HARD_USE_SOFT 0

struct ifloat_s
{
  uint32_t mantissa:MANTISSA_BITS, exponent:EXPONENT_BITS, sign:1;
};

union ifloat_u
{
  float f;
  struct ifloat_s part;
};

struct vector_header_s
{
  uint16_t length; // length=0 means 1 element, length=1 means 2 elements etc. 
  uint16_t type;
  union ifloat_u *data;
  volatile struct vector_header_s *next;
};

// for goldschmidt's division
//#define PRECISION_BITS 27
//#define ITERATION_STEPS 6

#define VECTOR_MAXLEN 2048

// the vector values 2 input argumnets, 4 results for soft and hard
extern volatile struct vector_header_s *arg[2], *soft_result[4], *hard_result[4];

struct vector_register_s
{
  int length; // actual number of elements
  union ifloat_u data[VECTOR_MAXLEN]; // the data array
};

#define VECTOR_REGISTERS 8
extern struct vector_register_s *vr[VECTOR_REGISTERS]; // 8 vector registers

extern volatile uint32_t *vector_mmio;

void print(char *a);
volatile struct vector_header_s *create_segmented_vector(int n, int m);
void float2hex(char *hex, union ifloat_u *a);
void test_float2hex(void);
void printvector(volatile struct vector_header_s *vh, int from, int to);
void soft_alloc_vector_registers(void);
void soft_vector_io(int i, volatile struct vector_header_s *vh, int store_mode);
void vector_flush(volatile struct vector_header_s *vh);
void hard_init(void);
void hard_vector_io(int i, volatile struct vector_header_s *vh, int store_mode);
void soft_vector_oper(int a, int b, int c, int oper);
void hard_vector_oper(int a, int b, int c, int oper);
void wait_vector_mask(uint32_t mask);
void wait_vector(void);
void soft_vector_random(int n);
void soft_vector_incremental(int n);
int vector_difference(volatile struct vector_header_s *a, volatile struct vector_header_s *b);
void vector_dumpreg(void);
int vector_detect(void);

#endif

