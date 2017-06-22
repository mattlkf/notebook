#include "show.h"
#include <stdio.h>

SPA_Error SPA_Obj_show(obj_t obj){

  if(obj.type == CSR_VIEW){
      // Todo: print the view of the sparse matrix
  }
  else {
      int i, j;
      csr_t *mat = obj.csr;
      const int &m = mat->m;
      const int &n = mat->n;
      printf("A %dx%d matrix\n", m, n);

      // Print the CSR representation
      printf(" A: ");
      for(i=0;i<mat->ia[m];i++) printf("%.1f ", mat->data[i]);
      printf("\n");

      printf("iA: ");
      for(i=0;i<=m;i++) printf("%3d ", mat->ia[i]);
      printf("\n");

      printf("jA: ");
      for(i=0;i<mat->ia[m];i++) printf("%3d ", mat->ja[i]);
      printf("\n");

      // Print the contents in full matrix form
      int csr_idx = 0;
      printf("Uncompressed form:\n");
      for(i=0;i<m;i++){
        for(j=0;j<n;j++){
          if(csr_idx == mat->ia[i+1] || j < mat->ja[csr_idx]){
            printf(" -  ");
            continue;
          }
          printf("%.1f ", mat->data[csr_idx++]);
        }
        printf("\n");
      }
  }
}