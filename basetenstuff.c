#include <stdio.h>




int main(int argc, char* argv[]){

  // how to process a single digit at a time from a base 10 number from right to left

  int n = 12345;

  int q; // quotient 
  int r; // remainder
 

  for(q=n;q;q/=10){
    r = q % 10;
    printf("%d\n",r);

  }
  return 0;
}
