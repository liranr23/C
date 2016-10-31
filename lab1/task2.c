#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
  int i, c = 0;
  int digit = 0;
  char ch;
  int s = 0, read = 0;
  int out = 0, out_num = 0, out_count = 0;
  FILE *outputs[5];
  FILE * input = stdin;
  int counter = 1, print = 0;
  for(i=1; i<argc; i++){
    if(strcmp(argv[i],"-s")==0){
	ch = *argv[++i];
	s = 1;
    }
    if(strcmp(argv[i],"-d")==0)
	digit=1;
    if(strcmp(argv[i],"-i")==0)
	input = fopen(argv[++i],"r");
	read = 1;
    if(strcmp(argv[i],"-o")==0){
	out_num = *argv[++i] - '0';
	out = 1;
    }
  }
  if(out == 1){
    printf("Number of output files: %d\n", out_num);
    int j;
    for(j=1; j<=out_num; j++){
      printf("Enter output file %d\n", j);
      char out_name[10];
      scanf("%s", out_name);
      outputs[j-1] = fopen(out_name, "w");
    }
  }
  while((c = getc(input)) != EOF){
    if(out == 0){
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
	    }
	    putc(c, stdout);
	  }
      }
    }
    else{
      if(counter == 1 && print == 0){
	fprintf(outputs[out_count], "%d:", counter);
	print = 1;
      }
      switch(c){
	case '@':
	case '*':
	  putc(c, outputs[out_count]);
	  putc('\n', outputs[out_count]);
	  ++counter;
	  ++out_count;
	  if(out_count>out_num-1){
	    out_count = 0;
	  }
	  fprintf(outputs[out_count], "%d:", counter);
	  break;
	default:
	  if(c >= 48 && c <= 57 && digit == 1){
	    putc(c, outputs[out_count]);
	    putc('\n', outputs[out_count]);
	    ++counter;
	    ++out_count;
	    if(out_count>out_num-1){
	      out_count = 0;
	    }
	    fprintf(outputs[out_count], "%d:", counter);
	    break;
	  }
	  if(s == 1 && c == ch){
	    putc(c, outputs[out_count]);
	    putc('\n', outputs[out_count]);
	    ++counter;
	    ++out_count;
	    if(out_count>out_num-1){
	      out_count = 0;
	    }
	    fprintf(outputs[out_count], "%d:", counter);
	    break;
	  }
	  putc(c, outputs[out_count]);
	}
      }
  }
  if(read == 1)
    fclose(input);
  if(out == 1){
    int i;
    for(i=0; i<out_num; i++){
      fclose(outputs[i]);
    }
  }
  return 0;
}