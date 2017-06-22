#include "partitions.h"
#include <assert.h>
#include <stdlib.h>

int SPA_Obj_length(obj_t A){
  if(A.type == CSR_VIEW){
    #ifdef SAFE
    assert(A.part != NULL);
    #endif

    switch(A.subtype){
      case VIEW_2x2_TR:
      case VIEW_2x2_TL:
        return A.part->r_idx;
        break;
      case VIEW_2x2_BR:
      case VIEW_2x2_BL:
        return A.part->csr->m - A.part->r_idx;
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
  if(ATL->part == NULL || ATR->part == NULL || ABL->part == NULL || ABR->part == NULL){
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
  ATL->type = VIEW_2x2_TL;
  ATR->type = VIEW_2x2_TR;
  ABL->type = VIEW_2x2_BL;
  ABR->type = VIEW_2x2_BR;

  ATL->part = ATR->part = ABL->part = ABR->part = part;

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
  if( ATL.part != ATR.part || 
      ATL.part != ABL.part || 
      ATL.part != ABR.part ){
    return SPA_Error_err;
  }
  // TODO: check that the values of mb and nb are sane (not out-of-bounds)
  #endif

  partition_t *part = ATL.part;

  // Record in the partition the size of the "fat divider"
  // If all these are constants, I expect these to be hoisted out of the loop
  part->mb = mb;
  part->nb = nb;
  part->subdiv_quad = quadrant; // which quadrant is being subdivided?

  // All the 9 new views are of the same partition
  // I expect this to be hoisted out of the loop since part does not change.
  // Regions that are never used should also be dead-code-eliminated.
  A00->part   = a01->part     = A02->part   = 
  a10t->part  = alpha11->part = a12t->part  =
  A20->part   = a21->part     = A22->part   = part;

  return SPA_Error_no_err;
}


// Actually move the column-pointers of the partition
SPA_Error SPA_Cont_with_3x3_to_2x2( obj_t *ATL, /**/ obj_t *ATR,  obj_t A00,  obj_t a01,     /**/ obj_t A02,
                                                                  obj_t a10t, obj_t alpha11, /**/ obj_t a12t,
                                    /* ********************** */  /* ************************************* */
                                    obj_t *ABL, /**/ obj_t *ABR,  obj_t A20,  obj_t a21,     /**/ obj_t A22,
                                
                                    int quadrant){
  // Todo: write safety-checks
  #ifdef SAFE
  if(ATL->type != CSR_VIEW || ATR->type != CSR_VIEW || ABL->type != CSR_VIEW || ABR->type != CSR_VIEW){
    return SPA_Error_err;
  }
  // The views should be of the same partition
  if( ATL->part != ATR->part || 
      ATL->part != ABL->part || 
      ATL->part != ABR->part ){
    return SPA_Error_err;
  }
  // Verify that the direction we're moving in is the same one which we were
  // subdivided by. E.g. if the bottom-right corner is subdivided, we should
  // be moving in that direction.
  if(ATL->part->subdiv_quad != quadrant){
    return SPA_Error_err;
  }

  #endif

  partition_t *part = ATL->part;

  // Move the row index
  // If quadrant is a constant, I expect the switch-case to be evaluated at
  // compile-time
  switch(quadrant){
    case SPA_TL:
    case SPA_TR:
      part->r_idx -= part->mb; // move the row upwards
      break;

    case SPA_BL:
    case SPA_BR:
      part->r_idx += part->mb; // move the row downwards
      break;

    default:
      return SPA_Error_err;
  }

  // Move the column pointers
  int *ia = part->csr->ia;
  int *ja = part->csr->ja;
  int *c_idx = part->c_idx;
  const int m = part->csr->m;
  const int n = part->csr->n;
  const int nb = part->nb;
  switch(quadrant){
    case SPA_TL:
    case SPA_BL:
      part->c_desired -= nb;
      for(int i=0;i<m;i++){
        while(ja[c_idx[i]] > part->c_desired && c_idx[i] > ia[i]) c_idx[i]--;

        // When moving left only. Required to maintain invariant that elements
        // of c_idx[] point to the column desired, or the first non-zero col to
        // the right.

        if(ja[c_idx[i]] < part->c_desired && c_idx[i] < ia[i+1]) c_idx[i]++;
      }
      break;

    case SPA_TR:
    case SPA_BR:
      part->c_desired += nb;
      for(int i=0;i<m;i++){
        while(ja[c_idx[i]] < part->c_desired && c_idx[i] < ia[i]) c_idx[i]++;
      }
      break;

    default:
      return SPA_Error_err;
  }


  return SPA_Error_no_err;
}