/* simple vector creation example
 */
#include <Vector.h>

Vector V;
struct vector_header_s *a, *b, *c;

void setup()
{
  int i;
  int vlen = 3;
  delay(1000);
  V.test(1); // run test N times
  // output can be seen with Serial Monitor
  a = V.create(vlen); // create vector of 5 elements
  b = V.create(vlen);
  c = V.create(vlen);
  for(i = 0; i < vlen; i++)
  {
    a->data[i].f = 1.0*i;
    b->data[i].f = 1.0/(1+i);
    c->data[i].f = -5.0; // should be overwritten with result
  }
  V.load(0, a); // load v(0)=a
  V.load(1, b); // load v(1)=b
  //V.mul(2, 0, 1); // v(2) = v(0)*v(1)
  V.f2i(2, 0);
  V.store(c, 2); // store c=v(2)
  //V.i2f(1, 2);
  //V.store(b, 1);
  V.dumpreg();
  #if 0
  float *pa = &(a->data[0].f);
  for(int i = 0; i < 4; i++)
    pa[i] = 9.9;
  #endif
  #if 1
  V.print(a); // print the vector
  V.print(b); // print the vector
  V.print(c); // print the vector
  #endif
  int err=0;
  for(i = 0; i < vlen; i++)
    if(c->data[i].f == -5.0)
      err++;
  printf("errors: %d\n", err);
}

void loop()
{
  delay(1000);
}
