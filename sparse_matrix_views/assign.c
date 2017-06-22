#include "assign.h"
#include <assert.h>
#include <stdlib.h>

void SPA_Obj_set_to_scalar(obj_t alpha, obj_t A){
  #ifdef SAFE
  // Alpha must be a scalar
  assert(alpha.type == SCALAR);
  assert(alpha.scalar != NULL);
  #endif

  switch(A.type){
    case SCALAR:
      *A.scalar = *alpha.scalar;
      break;
    default:
      // Todo: support setting all elements of a matrix to the same value
      assert(false);
      break;
  }

}