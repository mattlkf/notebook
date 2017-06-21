#include "FLAME.h"

int main( void ){
    double* buffer;
    int
    m, rs, cs;
    FLA_Obj A;
    // Initialize libflame.
    FLA_Init();
    // Get the matrix buffer address, size, and row and column strides.
    get_matrix_info( &buffer, &m, &rs, &cs );
    // Create an m x m double-precision libflame object without a buffer,
    // and then attach the matrix buffer to the object.
    FLA_Obj_create_without_buffer( FLA_DOUBLE, m, m, &A );
    FLA_Obj_attach_buffer( buffer, rs, cs, &A );
    // Compute the Cholesky factorization, storing to the lower triangle.
    FLA_Chol( FLA_LOWER_TRIANGULAR, A );
    // Free the object without freeing the matrix buffer.
    FLA_Obj_free_without_buffer( &A );
    // Free the matrix buffer.
    free_matrix( buffer );
    // Finalize libflame.
    FLA_Finalize();
    return 0;
}