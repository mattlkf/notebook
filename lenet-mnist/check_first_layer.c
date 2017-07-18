#include <stdio.h>

float in[28][28];
float k[2][5][5];
float bias[2];
float output[28][28];
float ref_out[28][28];

#define EPSILON 0.01

int main(){
  
  int i, j;

  for(i=0;i<5;i++){
    for(j=0;j<5;j++){
      scanf("%f %f", &k[0][i][j], &k[1][i][j]);
    }
  }

  scanf("%f %f", &bias[0], &bias[1]);

  // Read an image
  for(i=0;i<28;i++){
    for(j=0;j<28;j++){
      scanf("%f", &in[i][j]);
    }
  }

  // Read sample output
  for(i=0;i<28;i++){
    for(j=0;j<28;j++){
      scanf("%f", &ref_out[i][j]);
    }
  }


  // Convolve
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

          mac += in[ai][aj] * k[0][4-ki][4-kj];
        }
      }

      output[i][j] = mac + bias[0];
    }
  }

  // Apply ReLU
  for(i=0;i<28;i++){
    for(j=0;j<28;j++){
      if(output[i][j] < 0) output[i][j] = 0;
    }
  }


  // Compare (strict)
  // for(i=0;i<28;i++){
  //   for(j=0;j<28;j++){
  //     if(ref_out[i][j] != output[i][j]){
  //       printf("!\n");
  //       printf("%f %f\n", ref_out[i][j], output[i][j]);
  //     }
  //   }
  // }

  // Compare (with tolerance)
  for(i=0;i<28;i++){
    for(j=0;j<28;j++){
      if(ref_out[i][j] < (output[i][j] - EPSILON) 
       ||ref_out[i][j] > (output[i][j] + EPSILON)){
        printf("!\n");
        printf("%f %f\n", ref_out[i][j], output[i][j]);
        printf("%f %f %f\n", ref_out[i][j], output[i][j] + EPSILON, output[i][j] - EPSILON);
      }
    }
  }


  // Print
  // for(i=0;i<28;i++){
  //   for(j=0;j<28;j++){
  //     printf("%.2f ", output[i][j]);
  //   }
  //   printf("\n");
  // }

  return 0;
}