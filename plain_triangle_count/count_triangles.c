#include <stdio.h>

typedef struct{
	int *data;
	int *ia;
	int *ja;
} csr_t;

int get_int(FILE * f){
	// Read characters until you hit a number
	char ch;
	while(1){
		fscanf(f, "%c", &ch);
		// printf("%c", ch);
		if(ch >= '0' && ch <= '9') break;
	}

	int result = 0;
	while(ch >= '0' && ch <= '9'){
		result = result * 10 + ch - '0';
		fscanf(f, "%c", &ch);
		// printf("%c", ch);
	}
	return result;
}

void read_csr(FILE *f, csr_t * csr){
	int n = get_int(f);
	int m = get_int(f);
	printf("%d nodes, %d edges\n", n, m);

	for(int i=0;i<m*2;i++){
		get_int(f);
	}
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


	fclose(f);
	return 0;
}


