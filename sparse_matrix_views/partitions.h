#include "typedefs.h"

// Return the # of rows in an object (view or csr)
int SPA_Obj_length(obj_t A);

// Create a partition object and advance the column-pointers to the correct column
SPA_Error SPA_Part_2x2(obj_t A, obj_t* ATL, obj_t* ATR,
                                obj_t* ABL, obj_t* ABR, int mb, int nb, int quadrant);


// Do not actually advance the column-pointers of the partition. Instead, update the views
SPA_Error SPA_Repart_2x2_to_3x3(obj_t ATL, /**/ obj_t ATR,      obj_t *A00,  /**/ obj_t *a01,     obj_t *A02,
                                /* ******************** */      /* **************************************** */
                                                                obj_t *a10t, /**/ obj_t *alpha11, obj_t *a12t,
                                obj_t ABL, /**/ obj_t ABR,      obj_t *A20,  /**/ obj_t *a21,     obj_t *A22,
                                
                                int mb, int nb, int quadrant);

// Actually move the column-pointers of the partition
SPA_Error SPA_Cont_with_3x3_to_2x2( obj_t *ATL, /**/ obj_t *ATR,  obj_t A00,  obj_t a01,     /**/ obj_t A02,
                                                                  obj_t a10t, obj_t alpha11, /**/ obj_t a12t,
                                    /* ********************** */  /* ************************************* */
                                    obj_t *ABL, /**/ obj_t *ABR,  obj_t A20,  obj_t a21,     /**/ obj_t A22,
                                
                                    int quadrant);