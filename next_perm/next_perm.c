#include <stdio.h>

int n = 5;
int a[] = {1,2,3,4,5};

// Permute the elements to produce the next permutation in lexicographical order
// If we are at the last permutation, "wrap around".
int next_perm(int *a, int n){
	int i, j, tmp;
	int wrap_around = 1;
	// Go from the back, checking for the first point where a[i] < a[i+1]
	for(i=n-1;i>0;i--){
		if(a[i-1] < a[i]) break;
	}
	// We found a point where a[i] < a[i+1]
	if(i > 0){
		wrap_around = 0;
		// Find the lowest element in the tail that's larger
		for(j=n-1;j>=i;j--){
			if(a[j] > a[i-1]) break;
		}

		// Swap i-1 and j
		tmp = a[i-1];
		a[i-1] = a[j];
		a[j] = tmp;
	} 

	// Invert the tail
	for(j=n-1;j>i;j--,i++){
		tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}

	return wrap_around;
}

int main(){
	while(1){
		for(int i=0;i<n;i++) printf("%d ", a[i]);
		printf("\n");
		if(next_perm(a,n)) break;
	}

	printf("---\n");
	for(int i=0;i<n;i++) printf("%d ", a[i]);
	printf("\n");


	return 0;
}