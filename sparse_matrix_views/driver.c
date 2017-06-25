#include <stdio.h>
#include "SPA.h"
#include "triangle_count.h"
#include "assert.h"

#define M 4
#define N 4



// double data[] = { 1.0, 2.0, 0.0, 4.0, 
//                   0.0, 0.0, 0.0, 0.0, 
//                   0.0, 3.0, 0.0, 0.0,
//                   0.0, 0.0, 0.0, 5.0 };

double data[] = { 1.0, 1.0, 1.0, 1.0, 
                  1.0, 2.0, 2.0, 2.0, 
                  1.0, 2.0, 3.0, 3.0,
                  1.0, 3.0, 3.0, 4.0 };

double zero[] = {0};

// #define M 4
// #define N 4
// double data[] = { 0.0, 0.0, 0.0, 0.0, 
//                   0.0, 0.0, 0.0, 0.0, 
//                   0.0, 0.0, 0.0, 0.0,
//                   0.0, 0.0, 0.0, 0.0 };
                  
int main(){                    
  // Create a matrix object
  obj_t A, b;
  SPA_Error err;
  err = SPA_Obj_create(M, N, &A);
  assert(err == SPA_Error_no_err);

  err = SPA_Obj_create(1, 1, &b); // b is a scalar
  assert(err == SPA_Error_no_err);



  // Initialize its contents from buffer
  err = SPA_Copy_buffer_to_object(M, N, data, A);
  assert(err == SPA_Error_no_err);
  err = SPA_Copy_buffer_to_object(1, 1, zero, b);
  assert(err == SPA_Error_no_err);

  // Show objects
  SPA_Obj_show(A);
  SPA_Obj_show(b);

  triangle_count_unb_var1(A, b);

  // Free it
  SPA_Obj_free(&A);
  SPA_Obj_free(&b);

  return 0;
}