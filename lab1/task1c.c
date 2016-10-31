#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
  int i, c = 0;
  int digit = 0;
  char ch;
  int s = 0, read = 0;;
  FILE * input = stdin;
  int counter = 1, print = 0;
  for(i=1; i<argc; i++){
    if(strcmp(argv[i],"-s")==0)
	ch = *argv[++i];
	s = 1;
    if(strcmp(argv[i],"-d")==0)
	digit=1;
    if(strcmp(argv[i],"-i")==0)
	input = fopen(argv[++i],"r");
	read = 1;
  }
  while((c = getc(input)) != EOF){
    switch(c){
      case '@':
      case '*':
	if(counter == 1 && print == 0){
	  printf("%d:", counter);
	  print = 1;
	}
	putc(c, stdout);
	putc('\n', stdout);
	++counter;
	printf("%d:", counter);
	break;
      default:
	if(c >= 48 && c <= 57 && digit == 1){
	  if(counter == 1 && print == 0){
	    printf("%d:", counter);
	    print = 1;
	  }
	  putc(c, stdout);
	  putc('\n', stdout);
	  ++counter;
	  printf("%d:", counter);
	  break;
	}
	if(s == 1 && c == ch){
	  if(counter == 1 && print == 0){
	    printf("%d:", counter);
	    print = 1;
	  }
	  putc(c, stdout);
	  putc('\n', stdout);
	  ++counter;
	  printf("%d:", counter);
	  break;
	}else{
	  if(counter == 1 && print == 0){
	    printf("%d:", counter);
	    print = 1;
	    ++counter;
	  }
	  putc(c, stdout);
	}
      }
  }
  if(read == 1)
    fclose(input);
  return 0;
}