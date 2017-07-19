#include <stdio.h>
#include <stdint.h>
float in[28][28];
float conv_kernel[2][5][5];
float bias[2];
float conv_activate_out[2][28][28];
float pool_out[2][14][14];
float ref_conv_activate[2][28][28];
float ref_pool[2][28][28];

// Second conv-pool
float kernel_2[2][2][5][5];
float ca_out_2[2][14][14];
float pool_out_2[2][7][7];
float ref_ca_2[2][14][14];
float ref_pool_2[2][7][7];
float bias_2[2];

float flattened[98];

float dense_weights_1[98][10];
float dense_bias_1[10];
float dense_out_1[10];
float ref_dense_1[10];

float dense_weights_2[10][10];
float dense_bias_2[10];
float dense_out_2[10];
float ref_dense_2[10];


#define EPSILON 0.01

float q8_to_float(int16_t x){
  return (float)(x) / (float)(1 << 8);
}

int16_t float_to_q8_quick(float x){
  return (int16_t)(x * (float)(1 << 8));
}

int16_t mul(int16_t x, int16_t y){
  int32_t res = (int32_t) x * (int32_t) y;
  return (int16_t)(res >> 8);
}

int main(){
  
  int i, j, k;

  // Read conv kernel weights
  for(i=0;i<5;i++){
    for(j=0;j<5;j++){
      scanf("%f %f", &conv_kernel[0][i][j], &conv_kernel[1][i][j]);
    }
  }

  // Read bias
  scanf("%f %f", &bias[0], &bias[1]);

  // Read second conv kernel weights
  for(i=0;i<5;i++){
    for(j=0;j<5;j++){
      for(k=0;k<2;k++){
        scanf("%f %f", &kernel_2[0][k][i][j], &kernel_2[1][k][i][j]);
      }
    }
  }

  // Read bias
  scanf("%f %f", &bias_2[0], &bias_2[1]);

  // Read dense weights
  for(i=0;i<98;i++){
    for(j=0;j<10;j++){
      scanf("%f", &dense_weights_1[i][j]);
    }
  }

  // Read dense layer biases
  for(i=0;i<10;i++){
    scanf("%f", &dense_bias_1[i]);
  }

  // Read dense weights
  for(i=0;i<10;i++){
    for(j=0;j<10;j++){
      scanf("%f", &dense_weights_2[i][j]);
    }
  }

  // Read dense layer biases
  for(i=0;i<10;i++){
    scanf("%f", &dense_bias_2[i]);
  }


  // Read an image
  for(i=0;i<28;i++){
    for(j=0;j<28;j++){
      scanf("%f", &in[i][j]);
    }
  }

  // Read sample conv_activate_out (Layer 1 - after conv and ReLU)
  for(k=0;k<2;k++){
    for(i=0;i<28;i++){
      for(j=0;j<28;j++){
        scanf("%f", &ref_conv_activate[k][i][j]);
      }
    }
  }

  // Read sample conv_activate_out (Layer 1 - after pooling)
  for(k=0;k<2;k++){
    for(i=0;i<14;i++){
      for(j=0;j<14;j++){
        scanf("%f", &ref_pool[k][i][j]);
      }
    }
  }

  // Read reference output..
  for(k=0;k<2;k++){
    for(i=0;i<14;i++){
      for(j=0;j<14;j++){
        scanf("%f", &ref_ca_2[k][i][j]);
      }
    }
  }

  // Read pool 2 output
  for(k=0;k<2;k++){
    for(i=0;i<7;i++){
      for(j=0;j<7;j++){
        scanf("%f", &ref_pool_2[k][i][j]);
      }
    }
  }

  // Read flattened output
  for(i=0;i<98;i++) scanf("%f", &flattened[i]);

// Dense Layer 1

  // Read dense layer output (first dense)
  for(i=0;i<10;i++){
    scanf("%f", &ref_dense_1[i]);
  }

// Dense Layer 2

  // Read dense layer output (first dense)
  for(i=0;i<10;i++){
    scanf("%f", &ref_dense_2[i]);
  }

  // First conv-activate layer
  for(k=0;k<2;k++){
    for(i=0;i<28;i++){
      for(j=0;j<28;j++){
        float mac = 0;
        // Apply the kernel with the center at (i,j)
        for(int ki=0;ki<5;ki++){
          for(int kj=0;kj<5;kj++){
            int ai = i - 2 + ki;
            int aj = j - 2 + kj;
            if(ai < 0 || aj < 0 || ai >= 28 || aj >= 28){
              continue;
            }

            mac += in[ai][aj] * conv_kernel[k][4-ki][4-kj];
          }
        }
        conv_activate_out[k][i][j] = mac + bias[k];

        // Apply ReLU
        if(conv_activate_out[k][i][j] < 0) conv_activate_out[k][i][j] = 0;
      }
    }
  }

  // 2 x 2 max-pool
  for(k=0;k<2;k++){
    for(i=0;i<14;i++){
      for(j=0;j<14;j++){
        float max_val = 0;
        for(int ki=0;ki<2;ki++){
          for(int kj=0;kj<2;kj++){
            int ai = i * 2 + ki;
            int aj = j * 2 + kj;
            if(max_val < conv_activate_out[k][ai][aj]) max_val = conv_activate_out[k][ai][aj];
          }
        }

        pool_out[k][i][j] = max_val;
      }
    }
  }

  // Second conv-activate layer
  for(k=0;k<2;k++){
    for(i=0;i<14;i++){
      for(j=0;j<14;j++){
        float mac = 0;
        for(int layer=0;layer<2;layer++){
          // Apply the kernel with the center at (i,j)
          for(int ki=0;ki<5;ki++){
            for(int kj=0;kj<5;kj++){
              int ai = i - 2 + ki;
              int aj = j - 2 + kj;
              if(ai < 0 || aj < 0 || ai >= 14 || aj >= 14){
                continue;
              }

              mac += pool_out[layer][ai][aj] * kernel_2[k][layer][4-ki][4-kj];
            }
          }

        }
        ca_out_2[k][i][j] = mac + bias_2[k];
        // Apply ReLU
        if(ca_out_2[k][i][j] < 0) ca_out_2[k][i][j] = 0;
      }
    }
  }

  // Second pooling layer
  for(k=0;k<2;k++){
    for(i=0;i<7;i++){
      for(j=0;j<7;j++){
        float max_val = 0;
        for(int ki=0;ki<2;ki++){
          for(int kj=0;kj<2;kj++){
            int ai = i * 2 + ki;
            int aj = j * 2 + kj;
            if(max_val < ca_out_2[k][ai][aj]) max_val = ca_out_2[k][ai][aj];
          }
        }

        pool_out_2[k][i][j] = max_val;
      }
    }
  }

  // Flatten
  int p = 0;
  for(i=0;i<2;i++){
    for(j=0;j<7;j++){
      for(k=0;k<7;k++){
        flattened[p++] = pool_out_2[i][j][k];
      }
    }
  }


  // Compute first dense layer output (with ReLU)
  for(i=0;i<10;i++){
    float mac = dense_bias_1[i];
    for(j=0;j<98;j++){
      mac += flattened[j] * dense_weights_1[j][i];
    }
    dense_out_1[i] = mac > 0 ? mac : 0;
  }

  // Compute second dense layer output (without activation)
  for(i=0;i<10;i++){
    float mac = dense_bias_2[i];
    for(j=0;j<10;j++){
      mac += dense_out_1[j] * dense_weights_2[j][i];
    }
    // dense_out_2[i] = mac > 0 ? mac : 0;
    dense_out_2[i] = mac;
  }

  // Compare (with tolerance)
  for(k=0;k<2;k++){
    for(i=0;i<28;i++){
      for(j=0;j<28;j++){
        if(ref_conv_activate[k][i][j] < (conv_activate_out[k][i][j] - EPSILON) 
         ||ref_conv_activate[k][i][j] > (conv_activate_out[k][i][j] + EPSILON)){
          printf("!\n");
          printf("%f %f\n", ref_conv_activate[k][i][j], conv_activate_out[k][i][j]);
          printf("%f %f %f\n", ref_conv_activate[k][i][j], conv_activate_out[k][i][j] + EPSILON, conv_activate_out[k][i][j] - EPSILON);
        }
      }
    }
  }

  // Compare the pooled output (with tolerance)
  for(k=0;k<2;k++){
    for(i=0;i<14;i++){
      for(j=0;j<14;j++){
        if(ref_pool[k][i][j] < (pool_out[k][i][j] - EPSILON) 
         ||ref_pool[k][i][j] > (pool_out[k][i][j] + EPSILON)){
          printf("!\n");
          printf("%f %f\n", ref_pool[k][i][j], pool_out[k][i][j]);
          printf("%f %f %f\n", ref_pool[k][i][j], pool_out[k][i][j] + EPSILON, pool_out[k][i][j] - EPSILON);
        }
      }
    }
  }

  // Compare the second conv layer (with tolerance)
  for(k=0;k<2;k++){
    for(i=0;i<14;i++){
      for(j=0;j<14;j++){
        if(ref_ca_2[k][i][j] < (ca_out_2[k][i][j] - EPSILON) 
         ||ref_ca_2[k][i][j] > (ca_out_2[k][i][j] + EPSILON)){
          printf("!\n");
          printf("%f %f\n", ref_ca_2[k][i][j], ca_out_2[k][i][j]);
          printf("%f %f %f\n", ref_ca_2[k][i][j], ca_out_2[k][i][j] + EPSILON, ca_out_2[k][i][j] - EPSILON);
        }
      }
    }
  }

  // Compare the second pool layer (with tolerance)
  for(k=0;k<2;k++){
    for(i=0;i<7;i++){
      for(j=0;j<7;j++){
        if(ref_pool_2[k][i][j] < (pool_out_2[k][i][j] - EPSILON) 
         ||ref_pool_2[k][i][j] > (pool_out_2[k][i][j] + EPSILON)){
          printf("!\n");
          printf("%f %f\n", ref_pool_2[k][i][j], pool_out_2[k][i][j]);
          printf("%f %f %f\n", ref_pool_2[k][i][j], pool_out_2[k][i][j] + EPSILON, pool_out_2[k][i][j] - EPSILON);
        }
      }
    }
  }
  printf("Pool 2 ok\n");

  // Check the first dense layer output
  for(i=0;i<10;i++){
    if(ref_dense_1[i] < dense_out_1[i] - EPSILON
     ||ref_dense_1[i] > dense_out_1[i] + EPSILON){
      printf("Dense 1!\n");
      printf("%f %f\n", ref_dense_1[i], dense_out_1[i]);
    }
  }

  printf("Dense 1 ok\n");

  // Check the second dense layer output
  for(i=0;i<10;i++){
    if(ref_dense_2[i] < dense_out_2[i] - EPSILON
     ||ref_dense_2[i] > dense_out_2[i] + EPSILON){
      printf("Dense 2!\n");
      printf("%f %f\n", ref_dense_2[i], dense_out_2[i]);
    }
  }

  printf("Dense 2 ok\n");

  int argmax = 0;
  for(i=0;i<10;i++){
    if(dense_out_2[i] > dense_out_2[argmax]) argmax = i;
  }

  printf("Prediction: %d\n", argmax);

  // Print
  // for(k=0;k<2;k++){
  //   for(i=0;i<28;i++){
  //     for(j=0;j<28;j++){
  //       printf("%.3f ", conv_activate_out[k][i][j]);
  //     }
  //     printf("\n");
  //   }
  // }

  return 0;
}