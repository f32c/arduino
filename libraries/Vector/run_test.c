#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "vector_link.h"

/* linked list vector test */

// run test n times
void run_test(int n)
{
  int i;
  int t;
  char operation[] = "+-*/"; 

  soft_alloc_vector_registers(); // for software-float comparison test

  // create vectors in RAM for the results
  for(i = 0; i < 4; i++)
  {
    soft_result[i] = create_segmented_vector(VECTOR_MAXLEN,VECTOR_MAXLEN/4);
    hard_result[i] = create_segmented_vector(VECTOR_MAXLEN,VECTOR_MAXLEN/4);
  }  
  // create vectors in RAM for the arguments
  for(i = 0; i < 4; i++)
    arg[i] = create_segmented_vector(VECTOR_MAXLEN,VECTOR_MAXLEN/4);
  // first load vectors from RAM into soft registers
  // to get correct length, those registers will be used for random
  // generation
  soft_vector_io(0, arg[0], 0); // load vector reg vr[0] with value arg[0]
  soft_vector_io(2, arg[2], 0); // load vector reg vr[2] with value arg[2]

  for(t = 0; t < n; t++)
  {
    printf("test %d\n", t);
    soft_vector_random(0);
    soft_vector_random(2);
    //soft_vector_incremental(0);
    //soft_vector_incremental(1);
    soft_vector_io(0, arg[0], 1); // soft store random to arg[0]
    soft_vector_io(2, arg[2], 1); // soft store random to arg[1]
    hard_vector_io(0, arg[0], 0); // hard load vector reg vr[0] with value arg[0]
    hard_vector_io(2, arg[2], 0); // hard load vector reg vr[0] with value arg[1]
    hard_vector_io(4, arg[2], 0); // only to set result vector to the size
    for(i = 0; i < 4; i++)
    {
      int erri;
      printf("%c ", operation[i]);
      soft_vector_oper(4, 0, 2, i);   // compute vr[4] = vr[0] <oper> vr[2]
      soft_vector_io(4, soft_result[i], 1); // store to result[i]
      hard_vector_oper(4, 0, 2, i);
      hard_vector_io(4, hard_result[i], 1);
      erri = vector_difference(hard_result[i], soft_result[i]);
      if(erri >= 0)
      { // erri = index of 1st error occurence
        // print arguments and the results around the error occurence
        int around = 15;
        print("arg[0]         ");
        printvector(arg[0], erri-around, erri+around);
        print("arg[2]         ");
        printvector(arg[2], erri-around, erri+around);
        printf("soft_result[%i] ", i);
        printvector(soft_result[i], erri-around, erri+around);
        printf("hard_result[%i] ", i);
        printvector(hard_result[i], erri-around, erri+around);
      }
    }
  }
}

