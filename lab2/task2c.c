#include <stdio.h>
#include <stdlib.h>
 
char censor(char c) {
  if(c == '!')
    return '.';
  else if(c == 0)
    return 0;
  
  return c;
}

char to_lower(char c) {
  if(c >= 'A' && c<= 'Z'){
    c = c + 32;
  }
  return c;
}

char cprt(char c){
  printf("%c\n", c);
  return c;
}

char my_get(char c){
  char input;
  input = fgetc(stdin);
  if(input == '\n'){
    return 0;
  }else
    return input;
}

char quit(char c){
  exit(0);
}

void for_each(char *array, char (*f) (char)){
  char* pointer = array;
  char temp = 1;
  while(temp != 0){
   *pointer = f(*pointer);
    pointer++;
    temp = *pointer;
  }
}
 
int main(int argc, char **argv){
  char c[5];
  for_each(c, my_get);
  for_each(c, cprt);
  for_each(c, to_lower);
  for_each(c, censor);
  for_each(c, cprt);
  return 0;
}