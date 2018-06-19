/* Test for 32MB RAM
** Fill RAM with pseudoranom data and read back its content
*/
#include <Arduino.h>

volatile uint32_t *ram_begin = (uint32_t *) 0x80100000; // test start
volatile uint32_t *ram_end   = (uint32_t *) 0x82000000; // test end


static uint32_t rngseed = 123459876;

uint32_t rng(void)
{
	int x, t;
	uint32_t hi, lo;

	/*
	 * Compute x[n + 1] = (7^5 * x[n]) mod (2^31 - 1).
	 * From "Random number generators: good ones are hard to find",
	 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
	 * October 1988, p. 1195.
	 */
	/* Can't be initialized with 0, so use another value. */
	if ((x = rngseed) == 0)
		x = 123459876;
	hi = x / 127773;
	lo = x % 127773;
	t = 16807 * lo - 2836 * hi;
	if (t < 0)
		t += 0x7fffffff;
	rngseed = t;
	return t;
}

void ram_test(char *a)
{
  volatile uint32_t *addr;
  uint32_t errors = 0;
  static uint32_t seed = 123459876;
  rngseed = seed;
  for(addr = ram_begin; addr < ram_end; addr +=4)
    *addr = rng();
  rngseed = seed;
  for(addr = ram_begin; addr < ram_end; addr +=4)
    if(*addr != rng())
      errors++;
  if(errors)
    sprintf(a, "RAM: ERRORS:%d FAIL\n", errors);
  else
    sprintf(a, "RAM: OK\n");
  seed++; // change seed each time
}

