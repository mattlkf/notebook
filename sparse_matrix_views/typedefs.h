#pragma once

typedef int SPA_Error;
typedef double SPA_Elem_t;


#define SPA_Error_err 1
#define SPA_Error_no_err 0

#define CSR_VIEW  1
#define CSR_OBJ   2
#define COO_OBJ   3

// Forward-declaration of named struct to resolve circular dependency
struct obj_t_struct;

typedef struct{
  struct obj_t_struct* obj;  // which csr matrix is this a view of?
  int cs, ce; // column start, column end
  int rs, re; // row start, row end
  int *fs, *fe; // finger starts and finger ends
} view_t;

typedef struct{
  int m, n;  // the size of the encoded matrix

  // all these buffers should not be shared
  SPA_Elem_t *data;
  int *ia;
  int *ja;
} csr_t;

typedef struct obj_t_struct {
  int type;
  union {
    view_t *view;
    csr_t *csr;
  };
} obj_t;


typedef struct {
  SPA_Elem_t v;
  int x, y;
} coo_elem_t;