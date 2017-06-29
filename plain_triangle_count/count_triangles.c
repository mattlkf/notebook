#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
using namespace std;

typedef struct{
	int m, n;
	int *data;
	int *ia;
	int *ja;
} csr_t;

struct edge{
  int x, y;
};

bool cmp(const edge &a, const edge &b){
  if(a.x == b.x) return a.y < b.y;
  return a.x < b.x;
}

int get_int(FILE * f){
	// Read characters until you hit a number
	char ch;
	while(1){
		ch = getc(f);
		if(ch >= '0' && ch <= '9') break;
	}
	int result = 0;
	while(ch >= '0' && ch <= '9'){
		result = result * 10 + ch - '0';
		ch = getc(f);
	}
	return result;
}

void check_graph(edge *a, int n){
  for(int i=0;i<n;i++){
    if(a[i].x > a[i].y){
      printf("Not upper triangular");
    }

    if(a[i].x == a[i].y){
      printf("Self-loop detected\n");
    }
  }
}

void inspect_csr(csr_t csr){
  // Check how many rows are blank
  int empty_row_count = 0;
  for(int i=0;i<csr.n;i++){
    if(csr.ia[i] == csr.ia[i+1]){
      empty_row_count++;
    }
  }

  printf("There are %d empty rows (out of %d)\n", empty_row_count, csr.n);
}

// Treat it as a list of edges
void duplicate_graph(edge * a, int n){
  // Create a copy..
  int i;
  for(i=0;i<n;i++){
    a[i+n].x = a[i].y;
    a[i+n].y = a[i].x;
  }

  n *= 2;

  sort(a, a+n, cmp);
}

void read_csr(FILE *f, csr_t * csr, int duplicate){
	int n = get_int(f);
	int m = get_int(f);
	printf("%d unique nodes, %d edges\n", n, m);

  // Allocate enough space to hold the input data (it is not a CSR yet)
  int pii = 0;
  int *input_ints = (int *) malloc(sizeof(int) * m * (duplicate ? 4 : 2));
  assert(input_ints != NULL);

  int i;
  for(i=0;i<2*m;i++){
    input_ints[i] = get_int(f);
    n = max(input_ints[i], n);
  }

  printf("Read input file\n");

  check_graph((edge*) input_ints, m);

  if(duplicate){
    duplicate_graph((edge*)input_ints, m);
    m = m*2;
  }

  csr->m = m;
  csr->n = n+1; // n is 0-indexed

  // Allocate buffers for the CSR matrix
  csr->data = (int *) malloc(sizeof(int) * m);
  csr->ia = (int *) malloc(sizeof(int) * (csr->n + 1));
  csr->ja = (int *) malloc(sizeof(int) * m);

  assert(csr->data != NULL);
  assert(csr->ia != NULL);
  assert(csr->ja != NULL);

  // Populate the CSR matrix
  int pi = -1;
  for(i=0;i<m;i++){
    // Get the source node
    int u = input_ints[pii++];

    while(pi < u){
      csr->ia[++pi] = i;
    }
    csr->ja[i] = input_ints[pii++];
    csr->data[i] = 1;
  }

  while(pi <= csr->n){
    csr->ia[++pi] = i;    
  }

  free(input_ints);
}

void print_csr(csr_t *csr){
  int i, j;
    const int &m = csr->m;
    const int &n = csr->n;
    printf("A %dx%d matrix with %d edges\n", n, n, m);

    // Print the CSR representation
    printf(" A: ");
    for(i=0;i<csr->ia[n];i++) printf("%d ", csr->data[i]);
    printf("\n");

    printf("iA: ");
    for(i=0;i<=m;i++) printf("%d ", csr->ia[i]);
    printf("\n");

    printf("jA: ");
    for(i=0;i<csr->ia[n];i++) printf("%d ", csr->ja[i]);
    printf("\n");

    printf("id: ");
    for(i=0;i<csr->ia[n];i++) printf("%d ", i);
    printf("\n");


    // Print the contents in full matrix form
    int csr_idx = 0;
    printf("Uncompressed form:\n");
    for(i=0;i<n;i++){
      for(j=0;j<n;j++){
        if(csr_idx == csr->ia[i+1] || j < csr->ja[csr_idx]){
          printf("- ");
          continue;
        }
        printf("%d ", csr->data[csr_idx++]);
      }
      printf("\n");
    }
 
}

void free_csr(csr_t csr){
	if(csr.data != NULL) free(csr.data);
	if(csr.ja != NULL) free(csr.ja);
	if(csr.ia != NULL) free(csr.ia);
}

int count_triangles(csr_t *csr){
  int sum = 0;
  int i, j, k;
  int n = csr->n;

  // Allocate n indices, initially at the start of each row
  int *idx = (int *) malloc(sizeof(int) * n);

  for(int i=0;i<n;i++){
    idx[i] = csr->ia[i];
  }

  // Stride through matrix
  for(i=0;i<n;i++){
    printf("Hi\n");
    // Iterate through x (the vertical strip above current element)
    for(j=0;j<i;j++){
      printf("Find A%d %d\n", i, j);
      // Find A[i][j]
      while(idx[j] < csr->ia[j+1] && csr->ja[idx[j]] < i){
        idx[j]++;
      }

      printf("idx[%d] = %d aj %d\n", j, idx[j], csr->ja[idx[j]]);

      // If it's a one
      if(idx[j] != csr->ia[j+1] && csr->ja[idx[j]] == i){
        // Do the two-pointer trick on the rows
        int pa = idx[j];
        int pb = csr->ia[i];

        while(pa < csr->ia[j+1] && pb < csr->ia[i+1]){
          if(csr->ja[pa] == csr->ja[pb]){
            printf("Found a triangle! %d %d %d\n", i, j, csr->ja[pa]);
            sum++;
            pa++;
            pb++;
          }
          else {
            if(csr->ja[pa] < csr->ja[pb]) pa++;
            else pb++;
          }
        }
      }
    }
  }

  free(idx);

  return sum;
}

int main(int argc, char **argv){
	// Parse cmd-line arguments
	if(argc != 2){
		printf("Usage: %s <file>\n", argv[0]);
		return 1;
	}

	FILE * f = fopen(argv[1], "r");
	if(f == NULL){
		printf("Error opening file\n");
		return 1;
	}

	// Convert the input file into a sparse matrix
	csr_t csr;

	read_csr(f, &csr, 0);

  inspect_csr(csr);

	printf("Done reading\n");
	fflush(stdout);

	print_csr(&csr);

  printf("%d triangles\n", count_triangles(&csr));

	// free_csr(csr);

	fclose(f);
	return 0;
}




















