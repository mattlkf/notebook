#include <stdio.h>

int main(){
  while(1){
    double f;
    int ret = scanf("%lf", &f);
    printf("%lf\n", f);
    if(ret == 0) break;
  }

  return 0;
}