#include <stdio.h> 

int countDigit(char *input){
  int counter=0;
  int i=0;
  while(input[i] != 0){
    if(input[i] >= '0' && input[i] <= '9'){
      counter++;
    }
    i++;
  }
  return counter;
}

int main(int argc, char **argv){
  char *input = argv[1];
  int ans = countDigit(input);
  printf("The number of digits in the string is: %d\n", ans);
  return 0;
}