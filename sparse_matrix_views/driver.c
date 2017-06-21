#include <stdio.h>
#include "typedefs.h"
#include "partitions.h"
#include "create.h"
#include "show.h"

#define M 4
#define N 4
double data[] = { 1.0, 2.0, 0.0, 4.0, 
                  0.0, 0.0, 0.0, 0.0, 
                  0.0, 3.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 5.0 };

// #define M 4
// #define N 4
// double data[] = { 0.0, 0.0, 0.0, 0.0, 
//                   0.0, 0.0, 0.0, 0.0, 
//                   0.0, 0.0, 0.0, 0.0,
//                   0.0, 0.0, 0.0, 0.0 };
int main(){
                    
  // Create a matrix object
  obj_t A;
  SPA_Obj_create(M, N, &A);

  // Initialize its contents from buffer
  SPA_Copy_buffer_to_object(M, N, data, A);

  // Show it
  SPA_Obj_show(A);

  // Free it
  SPA_Obj_free(&A);

  return 0;
}