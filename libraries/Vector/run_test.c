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
  char line[100];
  //test_float2hex();
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
  for(i = 0; i < 2; i++)
  {
    arg[i] = create_segmented_vector(VECTOR_MAXLEN,VECTOR_MAXLEN/4);
    #if 0
      // print first few values
      sprintf(line, "value %d\n", i);
      print(line);
      printvector(arg[i], 0, 10);
    #endif
  }
  // first load vectors from RAM into soft registers
  // to get correct length, those registers will be used for random
  // generation
  soft_vector_io(0, arg[0], 0); // load vector reg vr[0] with value arg[0]
  soft_vector_io(1, arg[1], 0); // load vector reg vr[1] with value arg[1]

  for(t = 0; t < n; t++)
  {
    printf("test %d\n", t);
    soft_vector_random(0);
    soft_vector_random(1);
    //soft_vector_incremental(0);
    //soft_vector_incremental(1);
    soft_vector_io(0, arg[0], 1); // soft store random to arg[0]
    soft_vector_io(1, arg[1], 1); // soft store random to arg[1]
    hard_vector_io(0, arg[0], 0); // hard load vector reg vr[0] with value arg[0]
    hard_vector_io(1, arg[1], 0); // hard load vector reg vr[0] with value arg[0]
    for(i = 0; i < 4; i++)
    {
      int erri;
      printf("%c ", operation[i]);
      soft_vector_oper(2, 0, 1, i);   // compute vr[2] = vr[0] <oper> vr[1]
      soft_vector_io(2, soft_result[i], 1); // store to result[i]
      hard_vector_oper(2, 0, 1, i);
      hard_vector_io(2, hard_result[i], 1);
      erri = vector_difference(hard_result[i], soft_result[i]);
      if(erri >= 0)
      { // erri = index of 1st error occurence
        // print arguments and the results around the error occurence
        int around = 1;
        print("arg[0]         ");
        printvector(arg[0], erri-around, erri+around);
        print("arg[1]         ");
        printvector(arg[1], erri-around, erri+around);
        printf("soft_result[%i] ", i);
        printvector(soft_result[i], erri-around, erri+around);
        printf("hard_result[%i] ", i);
        printvector(hard_result[i], erri-around, erri+around);
      }
    }
  }
  #if 0
    sprintf(line, "soft_result[%d]\n", 0);
    print(line);
    printvector(soft_result[0], 0, 10); // print value result[0]
    sprintf(line, "hard_result[%d]\n", 0);
    print(line);
    printvector(hard_result[0], 0, 10); // print value result[0]
  #endif
}

