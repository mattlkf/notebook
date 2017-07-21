#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
using namespace std;

struct kernel_t{
  int x, y, z;
  float *val;
  float bias;
};

struct layer_t{
  int x, y, z;
  float *val;
};

struct dense_weights_t{
  int n_in; // the number of input nodes
  int n_out; // the number of output nodes
  float *val;
  float *bias;
};

#define MAX_BUF_SIZE 1352
float buf[2][MAX_BUF_SIZE];

layer_t define_layer(int x, int y, int z){
  static int tog = 1;
  tog = 1 - tog;
  return {x, y, z, buf[tog]};
}

float max_float_seen = 0;
float min_float_seen = 0;
float smallest_abs = 1.0;
void record_max_val(float x){
  if(x > max_float_seen) max_float_seen = x;
  if(x < min_float_seen) min_float_seen = x;

  // float absx = x < 0 ? -x : x;
  // if(absx != 0.0 && absx < smallest_abs) smallest_abs = absx; 
}

void read_kernels(kernel_t *k, int nk, int x, int y, int z){
  // Read kernel weights
  for(int i=0;i<nk;i++){
    k[i].val = (float*) malloc(sizeof(float) * x * y * z);
    k[i].x = x;
    k[i].y = y;
    k[i].z = z;
    for(int j=0;j<x*y*z;j++){
      scanf("%f", &k[i].val[j]);
      record_max_val(k[i].val[i]);
    }
  }

  // Read biases
  for(int i=0;i<nk;i++){
    scanf("%f", &k[i].bias);
    record_max_val(k[i].bias);
  }
}

void read_dense(dense_weights_t *d, int x, int y){
  d->val = (float *) malloc(sizeof(float) * x * y);
  d->bias = (float *) malloc(sizeof(float) * y);
  d->n_in = x;
  d->n_out = y;
  for(int i=0;i<x*y;i++){
    scanf("%f", &d->val[i]);
    record_max_val(d->val[i]);
  }
  // A bias for each output
  for(int i=0;i<y;i++){
    scanf("%f", &d->bias[i]);
    record_max_val(d->bias[i]);
  }
}

// Data structures

layer_t layer_0;
// Layer 1 (Conv)
kernel_t conv_1[2]; // two 3x3 kernels
layer_t layer_1;
// Layer 2 (Conv)
kernel_t conv_2[2]; // two 3x3 kernels
layer_t layer_2;
// Layer 3 (Pool)
layer_t layer_3; // after 2x2 pooling
// Layer 4 (Conv)
kernel_t conv_3[2]; // two 5x5 kernels
layer_t layer_4;
// Layer 5 (Pool)
layer_t layer_5; // after 2x2 pooling
// Layer 6 (Dense)
dense_weights_t dense_1;
layer_t layer_6;
// Layer 7 (Dense)
dense_weights_t dense_2;
layer_t layer_7;

void convolve(layer_t *in, kernel_t *kernels, layer_t *out){
  // Generate each output layer by convolving the correct kernel..
  int out_xy = out->x * out->y;
  int in_xy = in->x * in->y;
  int in_y = in->y;

  float reg[8];

  float *out_ptr = out->val;
  for(int ok=0;ok<out->z;ok++){

    int i, j, k;
    int oi, oj;

    kernel_t *kernel = &kernels[ok];

    float *in_ptr = in->val;

    // For each element of output..
    for(oi=0;oi<out->x;oi++){
      int ri;
      
      // The general case, when there are more than 8 cycles left
      for(oj=out->y & (~7);oj>0;oj -= 8){
        // Do this efficiently by loading into one reg and copy into the others
        for(ri=0;ri<8;ri++) reg[ri] = kernel->bias; 

        // Reset k_ptr to start of kernel
        float *k_ptr = kernel->val;

        // Apply the kernel to 8 consecutive input elements
        for(k=0;k<kernel->z;k++){ // For each layer
          for(i=0;i<kernel->x;i++){
            for(j=0;j<kernel->y;j++){

              float elem_k = *(k_ptr++);

              for(ri=0;ri<8;ri++){
                float elem_in = *(in_ptr++);
                
                reg[ri] += elem_in * elem_k;
                record_max_val(reg[ri]);
              }
              in_ptr -= 7;
            }

            in_ptr -= kernel->y;
            in_ptr += in_y;
          }
          in_ptr -= kernel->x * in_y;
          in_ptr += in_xy;
        }

        for(ri=0;ri<8;ri++){
          *(out_ptr++) = reg[ri];
        }

        in_ptr -= kernel->z * in_xy;
        in_ptr += 8;
      }


      // The tail case, when there are less than 8 cycles left

      for(oj = out->y & 7;oj>0;oj--){

        reg[0] = kernel->bias; 

        // Reset k_ptr to start of kernel
        float *k_ptr = kernel->val;

        // Apply the kernel to 8 consecutive input elements
        for(k=0;k<kernel->z;k++){ // For each layer
          for(i=0;i<kernel->x;i++){
            for(j=0;j<kernel->y;j++){

              float elem_k = *(k_ptr++);

              float elem_in = *(in_ptr++);
              
              reg[0] += elem_in * elem_k;
              record_max_val(reg[0]);
            }

            in_ptr -= kernel->y;
            in_ptr += in_y;
          }
          in_ptr -= kernel->x * in_y;
          in_ptr += in_xy;
        }

        *(out_ptr++) = reg[0];

        in_ptr -= kernel->z * in_xy;
        in_ptr++;
      }

      // End of tail case

      in_ptr -= out->y;
      in_ptr += in->y;
    }
  }
}

void relu(layer_t *in){
  for(int i=0;i<in->z * in->x * in->y;i++){
    if(in->val[i] < 0) in->val[i] = 0;
  }
}

void pool_2x2(layer_t *in, layer_t *out){
  assert(in->z == out->z);
  assert(in->x == out->x * 2);
  assert(in->y == out->y * 2);

  int in_xy =  in->x * in->y;
  int out_xy = out->x * out->y;

  for(int k=0;k<out->z;k++){
    for(int i=0;i<out->x;i++){
      for(int j=0;j<out->y;j++){
        float max = 0;

        if(in->val[k * in_xy + i*2*in->y + j*2] > max){
          max = in->val[k * in_xy + i*2*in->y + j*2];
        }        
        if(in->val[k * in_xy + i*2*in->y + j*2 + 1] > max){
          max = in->val[k * in_xy + i*2*in->y + j*2 + 1];
        }        
        if(in->val[k * in_xy + (i*2+1)*in->y + j*2] > max){
          max = in->val[k * in_xy + (i*2+1)*in->y + j*2];
        }        
        if(in->val[k * in_xy + (i*2+1)*in->y + j*2+1] > max){
          max = in->val[k * in_xy + (i*2+1)*in->y + j*2+1];
        }        

        out->val[k * out_xy + i * out->y + j] = max;
      }
    }
  }
}

void dense(layer_t *in, dense_weights_t *d, layer_t *out){
  assert(d->n_out == out->x * out->y * out->z);
  assert(d->n_in == in->x * in->y * in->z);
  // Loop over output values
  for(int i=0;i<d->n_out;i++){
    float mac = d->bias[i];
    for(int j=0;j<d->n_in;j++){
      mac += d->val[i * d->n_in + j] * in->val[j];
      record_max_val(mac);
    }
    out->val[i] = mac;
  }
}

int16_t argmax(layer_t *in){
  int ret = 0;
  for(int i=0;i<in->x * in->y * in->z;i++){
    if(in->val[i] > in->val[ret]) ret = i; 
  }
  return ret;
}

void read_layer(layer_t *in){
  int len = in->x * in->y * in->z;
  for(int i=0;i<len;i++){
    scanf("%f", &in->val[i]);
    // record_max_val(in->val[i]);
  }  
}

void print_layer(layer_t *in){
  int p=0;

  for(int k=0;k<in->z;k++){
    for(int i=0;i<in->x;i++){
      for(int j=0;j<in->y;j++){
        printf("%.3f ", in->val[p++]);
      }
      printf("\n");
    }
    printf("-------\n");
  }
}

void print_layer_sorted(layer_t *in){
  int len = in->x * in->y * in->z;
  float *sorted = (float*) malloc(sizeof(float) * len);
  for(int i=0;i<len;i++){
    sorted[i] = in->val[i];
  }

  sort(sorted, sorted+len);
  for(int i=0;i<len;i++){
    printf("%.2f ", sorted[i]);
  }
  printf("\n");

  free(sorted);
}


void free_kernel(kernel_t *k){
  free(k->val);
}

void free_dense(dense_weights_t *d){
  free(d->val);
  free(d->bias);
}

int main(){
  // Init layers
  layer_0 = define_layer(28, 28, 1);
  layer_1 = define_layer(26, 26, 2); // after 3x3 conv
  layer_2 = define_layer(24, 24, 2); // after 3x3 conv
  layer_3 = define_layer(12, 12, 2); // after 2x2 pool
  layer_4 = define_layer(8, 8, 2); // after 5x5 conv
  layer_5 = define_layer(4, 4, 2); // after 2x2 pool
  layer_6 = define_layer(10, 1, 1); // after dense 1
  layer_7 = define_layer(10, 1, 1); // after dense 2

  // Init weights
  read_kernels(conv_1, 2, 3, 3, 1); // two 3x3x1 kernels
  read_kernels(conv_2, 2, 3, 3, 2); // two 3x3x2 kernels
  read_kernels(conv_3, 2, 5, 5, 2); // two 5x5x2 kernels
  read_dense(&dense_1, 32, 10);
  read_dense(&dense_2, 10, 10);


  int n_same_as_keras = 0;
  int n_imgs;
  scanf("%d", &n_imgs);  
  printf("%d images\n", n_imgs);
  while(n_imgs--){
    // Read in the input layer
    read_layer(&layer_0);

    // Run it..
    convolve(&layer_0, conv_1, &layer_1); 
 
    // print_layer(&layer_1);

    relu(&layer_1);
    convolve(&layer_1, conv_2, &layer_2); 
    relu(&layer_2);
    pool_2x2(&layer_2, &layer_3);
    convolve(&layer_3, conv_3, &layer_4);
    relu(&layer_4);
    pool_2x2(&layer_4, &layer_5);
    dense(&layer_5, &dense_1, &layer_6);
    relu(&layer_6);
    dense(&layer_6, &dense_2, &layer_7);
    // Take the argmax of the last layer
    int prediction = argmax(&layer_7);
    // printf("Prediction: %d\n", prediction);
 
    int ref_prediction, ref_actual;
    scanf("%d %d", &ref_prediction, &ref_actual);
    // printf("Keras predicted %d (actually %d)\n", ref_prediction, ref_actual);
    if(ref_prediction == prediction){
      n_same_as_keras++;
    }
  }

  printf("Same as keras: %d\n", n_same_as_keras);

  printf("Max float seen: %f\n", max_float_seen);
  printf("Min float seen: %f\n", min_float_seen);
  // printf("Closest to 0: %f\n", smallest_abs);

  free_kernel(conv_1);
  free_kernel(conv_2);
  free_kernel(conv_3);
  free_dense(&dense_1);
  free_dense(&dense_2);
  return 0;
}