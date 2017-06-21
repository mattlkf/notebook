#include <stdio.h>
#include <stdlib.h>

#include "harness.h"
#include "baseline.h"

int main(int argc, char **argv){
  if(argc != 4){
    printf("Usage: %s <n> <m> <k>\n", argv[0]);
    return 1;
  }

  int n, m;
  n = atoi(argv[1]);
  m = atoi(argv[2]);
  k = atoi(argv[3]);


  printf("Matrix-multiply %dx%d and %dx%d\n", n, k, k, m);

  test(n, m, k);

  return 0;
}

