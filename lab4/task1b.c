#include "util.h"

#define SYS_WRITE 4
#define SYS_READ 3
#define EOF 0
#define STDIN 0
#define STDOUT 1
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_EXIT 1
#define O_RDONLY 0
#define O_WRONLY 1
#define MODE 0777
#define O_CREAT 64

extern int system_call(); /*for the warning*/

int main (int argc , char* argv[], char* envp[])
{
  char c;
  int line_count = 1, print = 0;
  char *line;
  int input = 0, output = 1, in = 0, out = 0;
  int i,j, num_of_files;
  int output_files[5];
  int place = 0;
  for(i = 1; i<argc; i++){
    if(strncmp(argv[i],"-i",2) == 0){
      in = 1;
      input = system_call(SYS_OPEN, argv[++i], O_RDONLY, MODE);
    }
    if(strncmp(argv[i],"-o",2) == 0){
      out = 1;
      num_of_files = *argv[++i] - '0';
      system_call(SYS_WRITE, STDOUT, "Number of output files: ",24);
      line = itoa(num_of_files);
      system_call(SYS_WRITE, STDOUT, line, strlen(line));
      system_call(SYS_WRITE, STDOUT, "\n", 1);
      for(j=1; j<=num_of_files; j++){
	char name[10];
	system_call(SYS_WRITE, STDOUT, "Enter output file ",18);
	line = itoa(j);
	system_call(SYS_WRITE, STDOUT, line, strlen(line));
	system_call(SYS_WRITE, STDOUT, ":", 1);
	system_call(SYS_WRITE, STDOUT, "\n", 1);
	system_call(SYS_READ, STDIN, name, 10);
	while(name[place] != '\n'){
	  place++;
	}
	name[place] = 0;
	system_call(SYS_OPEN, name, O_CREAT, MODE);
	output_files[j-1] = system_call(SYS_OPEN, name, O_WRONLY, MODE);
      }
      output = output_files[0];
    }
  }
  place = 0;
  while((system_call(SYS_READ, input, &c, 1)) != EOF){
    if(print == 0){
      line = itoa(line_count);
      system_call(SYS_WRITE, output, line, strlen(line));
      system_call(SYS_WRITE, output, ":", 1);
      print = 1;
    }
    switch(c){
      case '@':
      case '*':
	system_call(SYS_WRITE, output, &c, 1);
	system_call(SYS_WRITE, output, "\n", 1);
	line_count++;
	place++;
	if(place >= num_of_files){
	  place = 0;
	}
	output = output_files[place];
	print = 0;
	break;
      default:
	system_call(SYS_WRITE, output, &c, 1);
    }
  }
  if(in == 1){
    system_call(SYS_CLOSE, input);
  }
  if(out == 1){
    for(i=0; i<num_of_files; i++){
      system_call(SYS_CLOSE, output_files[i]);
    }
  }
  return 0;
}
