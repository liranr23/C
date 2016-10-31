#include <unistd.h>
#include <stdio.h>
#include "LineParser.h"
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <stdlib.h>
#define LINE_LENGTH 2048

typedef struct linked_list linked_list;
 
struct linked_list {
        char *command;
        linked_list *next;
};

linked_list *list_append(linked_list *list, linked_list *data){
  linked_list * pointer = list;
  if(pointer == 0){
    linked_list * newHead = data;
    return newHead;
  }
  while(pointer->next != 0){
    pointer = pointer->next;
  }
  pointer->next = data;
  return list;
}

void list_print(linked_list *list){
  int counter = 0;
  while(list != 0){
    printf("#%d ", counter);
    printf("%s", list->command);
    list = list->next;
    counter++;
  }
}

void list_free(linked_list *list){
  linked_list *pointer = list;
  while(list != 0){
    free(list->command);
    list = list->next;
    free(pointer);
    pointer = list;
  }
}

char * getCommand(linked_list *list, int place){
  while(place > 0){
    list = list->next;
    place--;
  }
  return list->command;
}

void execute(cmdLine *pCmdLine){
  int pid = fork();
  if(pid < 0){
    perror("fork");
    exit(1);
  }
  if(pid == 0){ /*child*/
    if((execvp(pCmdLine->arguments[0], pCmdLine->arguments)) < 0){
      perror("Error executing the command: ");
      _exit(0);
    }
  } /*end of child now the parent*/
  if((pCmdLine->blocking) == 1){
    waitpid(pid, 0, 0);
  }
} 

int choose_action(cmdLine *pCmdLine, linked_list *head){
  if(strcmp(pCmdLine->arguments[0], "quit") == 0){
    return 1;
  }
  else if(strcmp(pCmdLine->arguments[0], "cd") == 0){
    if((chdir(pCmdLine->arguments[1])) < 0){
      perror("Error changing a dir: ");
    }
    return 0;
  }
  else if(strcmp(pCmdLine->arguments[0], "history") == 0){
    if(head == 0){
      perror("Error-no history");
      return 0;
    }
    list_print(head);
    return 0;
  }
  else{
    execute(pCmdLine);
    return 0;
  }
}

int main (int argc , char* argv[]){
  char dir[PATH_MAX];
  char BUFFER[LINE_LENGTH];
  int commands_counter = 0;
  linked_list *head = 0;
  cmdLine *command;
  while(1){
    getcwd(dir, PATH_MAX);
    printf("The current dir is: %s \n", dir);
    fgets(BUFFER, LINE_LENGTH, stdin);
    command = parseCmdLines(BUFFER);
    if(strncmp(command->arguments[0], "!", 1) == 0){
      int place = atoi(&command->arguments[0][1]);
      if(place < 0 || place > commands_counter){
	perror("Error, out of bounds");
      }else{
	freeCmdLines(command);
	char *repeat = getCommand(head, place);
	command = parseCmdLines(repeat);
	strcpy(BUFFER, repeat);
      }
    }
    if((choose_action(command, head)) == 1){
      freeCmdLines(command);
      break;
    }
    linked_list *newNode = malloc(sizeof(linked_list));
    char *str = calloc(strlen(BUFFER)+1, sizeof(char));
    strcpy(str, BUFFER);
    newNode->command = str;
    newNode->next = 0;
    head = list_append(head, newNode);
    freeCmdLines(command);
    commands_counter++;
  }
  list_free(head);
  return 0;
}