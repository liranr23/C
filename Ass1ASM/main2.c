#include <stdio.h>

extern int calc_func(int x);

int check(int x){
  if(x<1 || x>31){
    return 0;
  }
  return 1;
}

int main(int argc, char** argv)
{
  int x;
	
  printf("Enter a Decimal number: ");
  fflush(stdout);

  scanf("%d", &x);

  calc_func(x);

  return 0;
}
