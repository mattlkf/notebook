#include <stdio.h>

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

#define EPSILON 0.01

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

  // Read reference output..
  for(k=0;k<2;k++){
    for(i=0;i<14;i++){
      for(j=0;j<14;j++){
        scanf("%f", &ref_ca_2[k][i][j]);
      }
    }
  }

  for(k=0;k<2;k++){
    for(i=0;i<7;i++){
      for(j=0;j<7;j++){
        scanf("%f", &ref_pool_2[k][i][j]);
      }
    }
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