#include <stdio.h>
#include <stdlib.h>
 
struct fun_desc {
  char *name;
  char (*fun)(char);
};

char carray[100] = "/0";

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
  while((*array = f(*array))){
    array++;
  }
}
 
struct fun_desc menu[] = { { "To Lower Case", to_lower }, { "Censor", censor }, { "Print", cprt }, { "Get String", my_get }, { "Quit", quit } ,{ NULL, NULL } };
 
int main(int argc, char **argv){
  char option;
  int i=0;
  while(1){
    printf("Please Choose a function:\n");
    for(i = 0; i < sizeof(menu) && menu[i].name != NULL ; ++i){
      printf("%d) %s\n", i, menu[i].name);
    }
    printf("Option: ");
    option = fgetc(stdin);
    fgetc(stdin);
    if(option >= 48 && option <= 52){
      for_each(carray, menu[option - '0'].fun);
      printf("DONE.\n\n");
    }else{
      quit('c');
    }
      /*printf("%s. is carray.\n", carray);     */
  }
  return 0;
}