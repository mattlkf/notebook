#pragma once

// Include safety-checks or not?
#define SAFE

// Types
typedef int SPA_Error;
typedef double SPA_Elem_t;
typedef SPA_Elem_t scalar_t;
// Error codes
#define SPA_Error_err 1
#define SPA_Error_no_err 0

// The return value used in Spark
#define SPA_SUCCESS 0

// SPA Quadrants (used in API)
#define SPA_TL    0
#define SPA_TR    1
#define SPA_BL    2
#define SPA_BR    3

// Object types
#define CSR_VIEW  1
#define CSR_OBJ   2
#define COO_OBJ   3
#define SCALAR    4

// View 2x2 parts
#define VIEW_2x2_TL    0
#define VIEW_2x2_TR    1
#define VIEW_2x2_BL    2
#define VIEW_2x2_BR    3

// View 3x3 parts
#define VIEW_3x3_TL    1
#define VIEW_3x3_TM    2
#define VIEW_3x3_TR    3

#define VIEW_3x3_ML    4
#define VIEW_3x3_MM    5
#define VIEW_3x3_MR    6

#define VIEW_3x3_BL    7
#define VIEW_3x3_BM    8
#define VIEW_3x3_BR    9

// Forward-declaration of named struct to resolve circular dependency
struct obj_t_struct;

typedef struct{
  int m, n;  // the size of the encoded matrix

  // all these buffers should not be shared
  SPA_Elem_t *data;
  int *ia;
  int *ja;
} csr_t;

// This struct represents a 2x2/3x3 partition of a CSR matrix.
// It is the "backing" of the various views possible.
typedef struct{
  int r_idx;
  int c_desired;
  int *c_idx; // M indexes that keep track of the columns
  csr_t *csr;
  int mb, nb; // if the partition is subdivided, by how much?
  int subdiv_quad; // which quadrant is subdivided?
} partition_t;

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
  int subtype;
  union {
    scalar_t *scalar;
    partition_t* part;
    csr_t *csr;
  };
} obj_t;

// Syntactic sugar
typedef obj_t SPA_Obj;
