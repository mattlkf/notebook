#include "typedefs.h"

SPA_Error SPA_Obj_create(int m, int n, obj_t *obj);

SPA_Error SPA_Copy_buffer_to_object(int m, int n, SPA_Elem_t *buf, obj_t obj);

SPA_Error SPA_Obj_free(obj_t *obj);
