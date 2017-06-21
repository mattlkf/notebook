#include <stdio.h>
#include "typedefs.h"
#include "partitions.h"
#include "create.h"
#include "show.h"

int main(){
  int i;
  for(i=0;i<3;i++){
    printf("%d\n", SPA_Part_2x1(i));
  }

  // Create a matrix object
  obj_t A;
  SPA_Obj_create(2, 2, &A);

  // Initialize its contents from buffer
  double data[] = {1.0, 2.0, 0.0, 4.0};
  SPA_Copy_buffer_to_object(2, 2, data, A);

  // Show it
  SPA_Obj_show(A);

// SPA_Error SPA_Copy_buffer_to_object(int m, int n, SPA_Elem_t *buf, obj_t obj){
// SPA_Error SPA_Obj_show(obj_t obj);

  return 0;
}