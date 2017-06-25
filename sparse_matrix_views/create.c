#include "create.h"
#include <stdlib.h>
#include <stdio.h>

SPA_Error SPA_Obj_create(int m, int n, obj_t *obj){
  if(m == 1 && n ==1){
    obj->type = SCALAR;
    obj->scalar = (scalar_t *) malloc(sizeof(scalar_t));
  }
  else if(m >= 0 && n >= 0 && m+n > 0){
    obj->type = CSR_OBJ;
    obj->csr = (csr_t *) malloc(sizeof(csr_t));
    obj->csr->m = m;
    obj->csr->n = n;
    obj->csr->data = NULL;
    obj->csr->ia = (int*) malloc((m+1)*sizeof(int));
    obj->csr->ja = NULL;
  }
  else{
    return SPA_Error_err;
  }

  return SPA_Error_no_err;
}

// Assumes that buf is in row-major order
SPA_Error SPA_Copy_buffer_to_object(int m, int n, SPA_Elem_t *buf, obj_t obj){
  int i, j;

  if (obj.type == SCALAR){
    printf("Creating a SCALAR\n");
    // If assigning to a scalar, buffer must be one element only
    if(m != 1 || n != 1){
      return SPA_Error_err;
    }
    *obj.scalar = buf[0];
  }
  else if(obj.type == CSR_OBJ){
    printf("Creating a CSR_OBJ\n");
    csr_t *mat = obj.csr;

    // dimensions of raw buffer must match csr matrix dimensions
    if(m != mat->m || n != mat->n){
      printf("Buffer does not match dimensions\n");
      return SPA_Error_err;
    }
    // Count the number of nonzero elements in the dense matrix
    int nz_count = 0;
    for(i=0;i<m*n;i++){
      if(buf[i] != 0) nz_count++;
    }

    // Free buffers if already allocated
    if(mat->data != NULL) free(mat->data);
    if(mat->ja != NULL) free(mat->ja);

    // Allocate buffers in the sparse matrix data structure
    mat->data = (SPA_Elem_t *) malloc(nz_count * sizeof(SPA_Elem_t));
    mat->ja = (int *) malloc(nz_count * sizeof(int));

    // Copy over the data, updating ia along the way
    if(mat->ia == NULL) return SPA_Error_err; // object must have been created
    
    int csr_idx = 0;
    int buf_idx = 0;

    for(i=0;i<m;i++){
      mat->ia[i] = csr_idx; // the start index of current row
      for(j=0;j<n;j++){
        if(buf[buf_idx] != 0){
          mat->data[csr_idx] = buf[buf_idx];
          mat->ja[csr_idx] = j;
          csr_idx++;
        }
        buf_idx++;
      }
    }
    mat->ia[m] = csr_idx; // the last element of iA is the # of nonzeros
  }
  else{
    return SPA_Error_err;
  }

  return SPA_Error_no_err;
}


SPA_Error SPA_Obj_free(obj_t *obj){
  if(obj->type == CSR_OBJ){
    csr_t *mat = obj->csr;
    if(mat != NULL){
      if(mat->data != NULL) free(mat->data);
      if(mat->ia != NULL) free(mat->ia);
      if(mat->ja != NULL) free(mat->ja);
      free(mat);
    }
  }
  else if(obj->type == SCALAR){
    if(obj->scalar != NULL) free(obj->scalar);
  }
}