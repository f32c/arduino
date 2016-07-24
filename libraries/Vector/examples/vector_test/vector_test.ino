/* simple vector creation example
 */
#include <Vector.h>

Vector V;
struct vector_header_s *a, *b, *c;

void setup()
{
  int i, j;
  int vlen = 14;
  struct vector_header_s *vh;
  delay(1000);
  V.test(1); // run test N times
  // output can be seen with Serial Monitor
  a = V.create(vlen); // create vector of 5 elements
  b = V.create(vlen);
  c = V.create(vlen);
  for(i = 0, vh = a; vh != NULL; vh = vh->next)
    for(j = 0; j <= vh->length; j++, i++)
      vh->data[j].f = 5.1 + (3+i);
  for(i = 0, vh = b; vh != NULL; vh = vh->next)
    for(j = 0; j <= vh->length; j++, i++)
      vh->data[j].f = 1.0 / (1+i);
  for(i = 0, vh = c; vh != NULL; vh = vh->next)
    for(j = 0; j <= vh->length; j++, i++)
      vh->data[j].f = -6.6;
  V.load(0, a); // load v(0)=a
  V.load(1, a); // load v(1)=a, just to initialize size, v(1) has same data as v(0)
  V.load(2, b); // load v(2)=b
  V.add(0, 1, 2); // v(0) = v(1)+v(2), actually does v(0) += v(2)
  V.store(c, 0);
  V.dumpreg();
  V.print(a); // print the vector
  V.print(b); // print the vector
  V.print(c); // print the vector
}

void loop()
{
  delay(1000);
}
