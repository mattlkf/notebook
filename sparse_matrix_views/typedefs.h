#pragma once

// Include safety-checks or not?
#define SAFE

typedef int SPA_Error;
typedef double SPA_Elem_t;


#define SPA_Error_err 1
#define SPA_Error_no_err 0

#define CSR_VIEW  1
#define CSR_OBJ   2
#define COO_OBJ   3

// View quadrants
#define SPA_TL    0
#define SPA_TR    1
#define SPA_BL    2
#define SPA_BR    3

// Subview quadrants
#define SPA_SV_TL 4
#define SPA_SV_TR 5
#define SPA_SV_BL 6
#define SPA_SV_BR 7

// Subview halves
#define SPA_SV_T  8
#define SPA_SV_B  9
#define SPA_SV_L  10
#define SPA_SV_R  11

// Forward-declaration of named struct to resolve circular dependency
struct obj_t_struct;

/*
typedef struct{
  struct obj_t_struct* obj;  // which csr matrix is this a view of?
  int cs, ce; // column start, column end
  int rs, re; // row start, row end
  int *fs, *fe; // finger starts and finger ends
} view_t;
*/

typedef struct{
  int m, n;  // the size of the encoded matrix

  // all these buffers should not be shared
  SPA_Elem_t *data;
  int *ia;
  int *ja;
} csr_t;

typedef struct{
  int r_idx;
  int *c_idx; // M indexes that keep track of the columns
  csr_t *csr;
  int mb, nb; // if the partition is subdivided, by how much?
} partition_t;

typedef struct view_t {
  int type;
  partition_t *part; // which partition is this a view into?
} view_t;

typedef struct {
  SPA_Elem_t v;
  int x, y;
} coo_elem_t;

typedef struct {
  int m, n, nz;
  coo_elem_t *data;
} coo_t;

typedef struct obj_t_struct {
  int type;
  union {
    view_t *view;
    csr_t *csr;
  };
} obj_t;


