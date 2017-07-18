#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
using namespace std;

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

// This is for "valid" mode in Keras - no zero-padding around input
void conv_relu_2x2_maxpool(layer l1, conv_kernel k, layer pool){
  // Set the dimensions for l2
  int l2m = l1.m - k.m + 1;
  int l2n = l1.n - k.n + 1;

  assert(l2m % 2 == 0);
  assert(l2n % 2 == 0);

  // A 2x2 max-pool - half the height and width
  pool.m = l2m >> 1;
  pool.n = l2n >> 1;

  // 2x2 pooling
  pool.elem = (float *) calloc(pool.m * pool.n, sizeof(float));

  // Convolve k over l1 to get l2. Iterate according to the indices of l2
  for(int l2i=0;l2i<l2m;l2i++){
    for(int l2j=0;l2j<l2n;l2j++){

      float l2_cur = 0;
      for(int ki=0;ki<k.m;ki++){
        for(int kj=0;kj<k.n;kj++){
          int l1i = l2i + ki;
          int l1j = l2j + kj;
          l2_cur += l1.elem[l1i * l1.n + l1j] * k.val[ki * k.n + kj];
        }
      }

      // Which element of pool are we currently computing?
      int pool_i = l2i >> 1;
      int pool_j = l2j >> 1;

      float &pool_e = pool.elem[pool_i * pool.n + pool_j];

      pool_e = max(pool_e, l2_cur);
    }
  }

  // Assume that the previous layer is no longer needed
  free(l1.elem);
}

int main(){
  int i, j;
  conv_kernel k1, k2;
  layer image, conv_pool_1_output, conv_pool_2_output, dense_1_output, dense_2_output;

  // Read the model weights
  // TODO: read the weights that Keras gives us

  // Read the input image 
  // TODO: read the MNIST dataset format

  conv_relu_2x2_maxpool(image, k1, conv_pool_1_output);
  conv_relu_2x2_maxpool(conv_pool_1_output, k2, conv_pool_2_output);




  return 0;
}