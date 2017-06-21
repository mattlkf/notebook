#include "FLAME.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int m, n, rs, cs;
} info_t;

// A dot B = 1 + 4 + 9 + 16 = 30

info_t info_A = {1,4,4,1};
double buf_A[] = {1.0,2.0,3.0,4.0};

info_t info_B = {4,1,1,4};
double buf_B[] = {1.0,2.0,3.0,4.0};

info_t info_G = {1,1,1,1};
double buf_G[] = {0};

void create_and_init(FLA_Obj &A, info_t &i, double* buf){
  FLA_Obj_create(FLA_DOUBLE, i.m, i.n, i.rs, i.cs, &A);
  // Copy the buffer contents to the libflame object
  FLA_Copy_buffer_to_object(FLA_NO_TRANSPOSE, i.m, i.n, buf, i.rs, i.cs, 0, 0, A);
}

int inner_product_unb( FLA_Obj at, FLA_Obj b, FLA_Obj gamma )
{
  FLA_Obj aLt,    aRt,       a0t,  alpha1,  a2t;

  FLA_Obj bT,              b0,
          bB,              beta1,
                           b2;

  FLA_Part_1x2( at,    &aLt,  &aRt,      0, FLA_LEFT );

  FLA_Part_2x1( b,    &bT, 
                      &bB,            0, FLA_TOP );

  while ( FLA_Obj_width( aLt ) < FLA_Obj_width( at ) ){

    FLA_Repart_1x2_to_1x3( aLt,  /**/ aRt,        &a0t, /**/ &alpha1, &a2t,
                           1, FLA_RIGHT );

    FLA_Repart_2x1_to_3x1( bT,                &b0, 
                        /* ** */            /* ***** */
                                              &beta1, 
                           bB,                &b2,        1, FLA_BOTTOM );

    /*------------------------------------------------------------*/

    FLA_Mult_add(alpha1, beta1, gamma);

    /*------------------------------------------------------------*/

    FLA_Cont_with_1x3_to_1x2( &aLt,  /**/ &aRt,        a0t, alpha1, /**/ a2t,
                              FLA_LEFT );

    FLA_Cont_with_3x1_to_2x1( &bT,                b0, 
                                                  beta1, 
                            /* ** */           /* ***** */
                              &bB,                b2,     FLA_TOP );

  }

  return FLA_SUCCESS;
}
int main( void ){
  FLA_Obj A, B, G;
  // Initialize libflame.
  FLA_Init();
  create_and_init(A, info_A, buf_A);
  create_and_init(B, info_B, buf_B);
  create_and_init(G, info_G, buf_G);

  // Show the vectors we created
  FLA_Obj_show("----", A, "%.2f", "----");
  FLA_Obj_show("----", B, "%.2f", "----");
  FLA_Obj_show("----", G, "%.2f", "----");

  // Perform a dot product
  inner_product_unb(A, B, G);
  FLA_Obj_show("----", G, "%.2f", "----");


  FLA_Obj_free(&A);
  FLA_Obj_free(&B);
  FLA_Finalize();
  return 0;
}