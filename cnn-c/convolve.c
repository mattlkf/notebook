#include <stdio.h>
#include <stdlib.h>

// Sample input:
// 3 3 2 2
// 0.1 0.4 0.2
// 3 4 5
// 7 8 9
// 0.5 2
// 1 0

// Sample output:
// 3.85 4.6
// 16.5 20

struct layer{
  int m, n;
  float *elem;
};

struct conv_kernel{
  int m, n;
  float *val;
};

int main(){
  int i, j;
  conv_kernel k;
  layer l1, l2;

  scanf("%d %d %d %d\n", &l1.m, &l1.n, &k.m, &k.n);
  l1.elem = (float *) malloc(sizeof(float) * l1.m * l1.n);
  k.val = (float *) malloc(sizeof(float) * k.m * k.n);


  l2.m = l1.m - k.m + 1;
  l2.n = l1.n - k.n + 1;
  l2.elem = (float *) malloc(sizeof(float) * l2.m * l2.n);

  for(i=0;i<l1.m *l1.n;i++){
    scanf("%f", &l1.elem[i]);
  }

  for(i=0;i<k.m * k.n;i++){
    scanf("%f", &k.val[i]);
  }

  // Convolve k over l1 to get l2
  for(int l2i=0;l2i<l2.m;l2i++){
    for(int l2j=0;l2j<l2.n;l2j++){
      // Which element of l2 are we currently computing?
      float &cur = l2.elem[l2i * l2.n +l2j];
      cur = 0;
      for(int ki=0;ki<k.m;ki++){
        for(int kj=0;kj<k.n;kj++){
          int l1i = l2i + ki;
          int l1j = l2j + kj;
          cur += l1.elem[l1i * l1.n + l1j] * k.val[ki * k.n + kj];
        }
      }
    }
  }

  // Print the result...
  int p = 0;
  for(i=0;i<l2.m;i++){
    for(j=0;j<l2.n;j++){
      printf("%.2f ", l2.elem[p++]);
    }
    printf("\n");
  }

  free(l1.elem);
  free(l2.elem);
  free(k.val);

  return 0;
}