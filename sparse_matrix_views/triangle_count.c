#include "SPA.h"

int triangle_count_unb_var1( SPA_Obj A, SPA_Obj beta )
{
  SPA_Obj ATL,   ATR,      A00,  a01,     A02, 
          ABL,   ABR,      a10t, alpha11, a12t,
                           A20,  a21,     A22;

  SPA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, SPA_TL );

  while ( SPA_Obj_length( ATL ) < SPA_Obj_length( A ) ){

    SPA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00,  /**/ &a01,     &A02,
                        /* ************* */   /* ************************** */
                                                &a10t, /**/ &alpha11, &a12t,
                           ABL, /**/ ABR,       &A20,  /**/ &a21,     &A22,
                           1, 1, SPA_BR );

    /*------------------------------------------------------------*/

    /*                       update line 1                        */
    /*                             :                              */
    /*                       update line n                        */

    /*------------------------------------------------------------*/

    SPA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00,  a01,     /**/ A02,
                                                     a10t, alpha11, /**/ a12t,
                            /* ************** */  /* ************************ */
                              &ABL, /**/ &ABR,       A20,  a21,     /**/ A22,
                              SPA_TL );

  }

  return SPA_SUCCESS;
}

