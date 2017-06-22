#include <stdio.h>
#include "typedefs.h"
#include "partitions.h"
#include "create.h"
#include "show.h"

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

// #define M 4
// #define N 4
// double data[] = { 0.0, 0.0, 0.0, 0.0, 
//                   0.0, 0.0, 0.0, 0.0, 
//                   0.0, 0.0, 0.0, 0.0,
//                   0.0, 0.0, 0.0, 0.0 };
                  

// #define COO_M 2
// #define COO_N 2
// #define COO_NZ 2
// coo_elem_t coo_data[COO_NZ] = {
//   {1.0, 0, 0},
//   {2.0, 1, 2}
// };

int main(){                    
  // Create a matrix object
  obj_t A;
  SPA_Obj_create(M, N, &A);

  // Initialize its contents from buffer
  SPA_Copy_buffer_to_object(M, N, data, A);

  // obj_t A_COO;
  // SPA_Obj_create(M, N, &A_COO);
  // SPA_Copy_coo_buffer_to_object(COO_M, COO_N, COO_NZ, coo_data, A_COO);
  // // Initialize its contents from a COO object
  
  // Show it
  SPA_Obj_show(A);

  // Free it
  SPA_Obj_free(&A);

  return 0;
}