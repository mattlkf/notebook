#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// Recall: first bit is sign (1 for negative, 0 for positive).
// Most negative number is 0x8000h (-1.0)
// Most positive number is 0x7fffh (0.999969482)
int16_t float_to_q15(float x){
  assert(x >= -1.0 && x < 1.0);
  float c = 0.5;
  int16_t result = 0;

  if(x < 0){
    result |= 1 << 15; // set the sign bit
    x += 1; // shift the value upwards into the [0,1) range
  }

  for(int i=14;i>=0;i--){
    if(x >= c){
      result |= (1 << i);
      x -= c;
    }
    c/=2.0;
  }

  return result;
}

float q15_to_float(int16_t x){
  return (float)(x) / (float)(1 << 15);
}

int16_t float_to_q15_quick(float x){
  return (int16_t)(x * (float)(1 << 15));
}

void print_bits(int16_t q){
  for(int i=15;i>=0;i--){
    printf("%c", q & (1 << i) ? '1' : '0');
  }
  printf("\n");
}

int main(){
  float a, b, c;

  while(1){
    scanf("%f %f", &a, &b);
    c = a * b;

    printf("%f * %f = %f\n", a, b, c);

    // printf("%hu * %hu = %hu\n", float_to_q15_quick(a), float_to_q15_quick(b), float_to_q15_quick(c));
    // printf("%hu * %hu = %hu\n", float_to_q15(a), float_to_q15(b), float_to_q15(c));

    printf("%hx * %hx = %hx\n", float_to_q15_quick(a), float_to_q15_quick(b), float_to_q15_quick(c));
    printf("%hx * %hx = %hx\n", float_to_q15(a), float_to_q15(b), float_to_q15(c));

  }



  return 0;
}