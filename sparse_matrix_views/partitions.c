#include "partitions.h"
#include <assert.h>
#include <stdlib.h>

int SPA_Obj_length(obj_t A){
  if(A.type == CSR_VIEW){
    #ifdef SAFE
    assert(A.view != NULL);
    assert(A.view->part != NULL);
    #endif

    switch(A.view->type){
      case SPA_TR:
      case SPA_TL:
        return A.view->part->r_idx;
        break;
      case SPA_BR:
      case SPA_BL:
        return A.view->part->csr->m - A.view->part->r_idx;
      default:
        assert(false);
    }
  }
  else if(A.type == CSR_OBJ){
    #ifdef SAFE
    assert(A.csr != NULL);
    #endif

    return A.csr->m;
  }
}

SPA_Error SPA_Part_2x2(obj_t A, obj_t* ATL, obj_t* ATR,
                                obj_t* ABL, obj_t* ABR, int mb, int nb, int quadrant){

#ifdef SAFE
  // Confirm that A is an object
  if(A.type != CSR_OBJ){
    return SPA_Error_err;
  }
  // Confirm that ATL, ATR, ABR and ABL are views
  if(ATL->type != CSR_VIEW || ATR->type != CSR_VIEW || ABL->type != CSR_VIEW || ABR->type != CSR_VIEW){
    return SPA_Error_err;
  }

  // Confirm that the views are not tied to a partition yet
  if(ATL->view->part == NULL || ATR->view->part == NULL || ABL->view->part == NULL || ABR->view->part == NULL){
    return SPA_Error_err;
  }
#endif

  // Based on the quadrant indicated, determine which row and which column we want
  const int m = A.csr->m;
  const int n = A.csr->n;
  int r, c;
  switch(quadrant){
    case SPA_TL:
      r = mb;
      c = nb;
      break;
    case SPA_TR:
      r = mb;
      c = n - nb;
      break;
    case SPA_BL:
      r = m - mb;
      c = nb;
      break;
    case SPA_BR:
      r = m - mb;
      c = n - nb;
      break;
    default:
      return SPA_Error_err;
  }

  // Create a partition...
  // Allocate a partition object to be shared by the views
  partition_t *part = (partition_t*) malloc(sizeof(partition_t));

  // Set the row of the partition
  part->r_idx = r;

  // The partition should have a series of column indices.. allocate space
  part->c_idx = (int *) malloc(m * sizeof(int));

  // Advance each column index of the partition until they point to "column" c
  for(int i=0;i<m;i++){
    int p = A.csr->ia[i];
    int row_end = A.csr->ia[i+1];
    // Advance until you reach a j-index at least c
    while(A.csr->ja[p] < c && p < row_end){
      p++; 
    }
    part->c_idx[i] = p;
  }

  // Set the views to point to this partition
  ATL->view->type = SPA_TL;
  ATR->view->type = SPA_TR;
  ABL->view->type = SPA_BL;
  ABR->view->type = SPA_BR;

  ATL->view->part = ATR->view->part = ABL->view->part = ABR->view->part = part;

  return SPA_Error_no_err;
}


// Do not actually advance the column-pointers of the partition. Instead, update the views
SPA_Error SPA_Repart_2x2_to_3x3(obj_t ATL, /**/ obj_t ATR,      obj_t *A00,  /**/ obj_t *a01,     obj_t *A02,
                                /* ******************** */      /* **************************************** */
                                                                obj_t *a10t, /**/ obj_t *alpha11, obj_t *a12t,
                                obj_t ABL, /**/ obj_t ABR,      obj_t *A20,  /**/ obj_t *a21,     obj_t *A22,
                                
                                int mb, int nb, int quadrant){
  #ifdef SAFE
  if(ATL.type != CSR_VIEW || ATR.type != CSR_VIEW || ABL.type != CSR_VIEW || ABR.type != CSR_VIEW){
    return SPA_Error_err;
  }
  // The views should be of the same partition
  if( ATL.view->part != ATR.view->part || 
      ATL.view->part != ABL.view->part || 
      ATL.view->part != ABR.view->part ){
    return SPA_Error_err;
  }
  // TODO: check that the values of mb and nb are sane (not out-of-bounds)
  #endif

  // Record in the partition the size of the "fat divider"
  ATL.view->part->mb = mb;
  ATL.view->part->nb = nb;

  // All the 9 new views are of the same partition
  *A00.view = 

  switch(quadrant){
    case SPA_TL:
      // The un-split quadrant
      *A22 = ABR;

      // The vertically split quadrant (L|R)
      A20->type = SPA_SV_L;
      A20->view = ABL;



      break;
    case SPA_TR:
      break;
    case SPA_BL:
      break;
    case SPA_BR:
      break;
    default:
      return SPA_Error_err;
  } 



}