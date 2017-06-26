#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct{
	int m, n;
	int *data;
	int *ia;
	int *ja;
} csr_t;

int get_int(FILE * f){
	// Read characters until you hit a number
	char ch;
	while(1){

		// fscanf(f, "%c", &ch);

		// ch = fgetc(f);
		ch = getc(f);

		if(ch >= '0' && ch <= '9') break;
	}

	int result = 0;
	while(ch >= '0' && ch <= '9'){
		result = result * 10 + ch - '0';
		// fscanf(f, "%c", &ch);
		// ch = fgetc(f);
		ch = getc(f);
	}
	return result;
}

void read_csr(FILE *f, csr_t * csr){
	int n = get_int(f);
	int m = get_int(f);
	printf("%d nodes, %d edges\n", n, m);

	// It's a square matrix
	csr->m = csr->n = n;

	// Allocate buffers for the CSR matrix
	csr->data = (int *) malloc(sizeof(int) * m);
	printf("%p\n", csr->data);
	csr->ia = (int *) malloc(sizeof(int) * (n+1));
	printf("%p\n", csr->ia);
	csr->ja = (int *) malloc(sizeof(int) * m);
	printf("%p\n", csr->ja);

	assert(csr->data != NULL);
	assert(csr->ia != NULL);
	assert(csr->ja != NULL);

	// Populate the CSR matrix
	int pi = -1;
	int i;
	for(i=0;i<m;i++){
		// Figure out the source node
		int u = get_int(f);

		if(u >= n) printf("%d\n", u);

		assert(u >= 0);
		assert(u < n);



		while(pi < u){
			assert(pi + 1 < m);
			assert(pi >= -1);
			csr->ia[++pi] = i;
		}
		assert(i < m);
		assert(i >= 0);
		csr->ja[i] = get_int(f);
		csr->data[i] = 1;
	}

	// printf("Hi");

	// while(pi < n){
	// 	csr->ia[++pi] = i;		
	// }
}

void print_csr(csr_t *csr){
	int i, j;
    const int &m = csr->m;
    const int &n = csr->n;
    printf("A %dx%d matrix\n", m, n);

    // Print the CSR representation
    printf(" A: ");
    for(i=0;i<csr->ia[m];i++) printf("%d ", csr->data[i]);
    printf("\n");

    printf("iA: ");
    for(i=0;i<=m;i++) printf("%d ", csr->ia[i]);
    printf("\n");

    printf("jA: ");
    for(i=0;i<csr->ia[m];i++) printf("%d ", csr->ja[i]);
    printf("\n");

    // Print the contents in full csrrix form
    int csr_idx = 0;
    printf("Uncompressed form:\n");
    for(i=0;i<m;i++){
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
	printf("%p\n", csr.data);
	fflush(stdout);
	if(csr.data != NULL) free(csr.data);
	printf("%p\n", csr.ia);
	fflush(stdout);
	if(csr.ja != NULL) free(csr.ja);
	printf("%p\n", csr.ja);
	fflush(stdout);
	if(csr.ia != NULL) free(csr.ia);
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

	read_csr(f, &csr);

	printf("Done reading\n");
	fflush(stdout);

	// print_csr(&csr);


	free_csr(csr);

	fclose(f);
	return 0;
}

