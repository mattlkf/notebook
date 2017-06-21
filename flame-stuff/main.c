#include "FLAME.h"
#include <stdio.h>
#include <stdlib.h>

void fill_buffer(double* &buf){
  buf[0] = 1;
  buf[1] = 2;
  buf[2] = 3;
  buf[3] = 4;
}

void get_matrix_info(double** buffer, int *m, int *rs, int *cs){
  *buffer = (double*) malloc(4 * sizeof(double));
  fill_buffer(*buffer);
  *m = 2; // 2*2 matrix
  *rs = 2;
  *cs = 1;
}

int main( void ){
  double* buffer;
  int m, rs, cs;
  FLA_Obj A;
  // Initialize libflame.
  FLA_Init();
  // Get the matrix buffer address, size, row and column strides
  get_matrix_info( &buffer, &m, &rs, &cs);
  // Create an m x m double-precision libflame object.
  FLA_Obj_create( FLA_DOUBLE, m, m, rs, cs, &A );
  // Copy the buffer contents to the libflame object
  FLA_Copy_buffer_to_object(FLA_NO_TRANSPOSE, m, m, buffer, rs, cs, 0, 0, A);


  // FLA_Obj_show("--- hi ---", A, "%11.3e", "--- bye ---");
  FLA_Obj_show("--- hi ---", A, "%.2f", "--- bye ---");

  FLA_Obj_free(&A);

  free(buffer);

  FLA_Finalize();

  printf("Done\n");
  return 0;
}