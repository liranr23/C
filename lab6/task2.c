#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

#define STDIN 0
#define STDOUT 1

int main(){
  char* args1[3];
  args1[0] = "ls";
  args1[1] = "-l";
  args1[2] = 0;
  char* args2[4];
  args2[0] = "tail";
  args2[1] = "-n";
  args2[2] = "2";
  args2[3] = 0;
  
  int pipepf[2];
  if(pipe(pipepf) == -1){
    perror("pipe");
    exit(1);
  }
  int pid;
  if((pid = fork()) == -1){
    perror("fork");
    exit(1);
  }
  if(pid == 0){
    close(STDOUT);
    dup(pipepf[1]);
    close(pipepf[1]);
    if((execvp(args1[0], args1)) < 0){
      perror("Error executing the command: ");
      _exit(0);
    }
  }else{
    close(pipepf[1]); 
    int pid2;
    if((pid2 = fork()) == -1){
      perror("fork");
      exit(1);
    }
    if(pid2 == 0){
      close(STDIN);
      dup(pipepf[0]);
      close(pipepf[0]);
      if((execvp(args2[0], args2)) < 0){
	perror("Error executing the command: ");
	_exit(0);
      }
    }
    else{
      close(pipepf[0]);
      waitpid(pid, 0, 0);
      waitpid(pid2, 0, 0);
    }
  }
  return 0;
}