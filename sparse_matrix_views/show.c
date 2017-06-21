#include "show.h"
#include <stdio.h>

SPA_Error SPA_Obj_show(obj_t obj){

  if(obj.type == CSR_VIEW){
      // Todo: print the view of the sparse matrix
      view_t *view = obj.view;
      printf("A view: rows %d to %d, cols %d to %d\n",
              view->rs, view->re,
              view->cs, view->ce);
  }
  else {
      csr_t *mat = obj.csr;
      const int &m = mat->m;
      const int &n = mat->n;
      printf("A %dx%d matrix\n", m, n);

      // Print the contents
      int i, j;
      int csr_idx = 0;
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