#include <unistd.h>
#include <stdio.h>
#include "LineParser.h"
#include <errno.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

#define STDIN 0
#define STDOUT 1
#define LINE_LENGTH 2048

typedef struct linked_list linked_list;

typedef struct var_list var_list;
 
struct linked_list {
        char *command;
        linked_list *next;
};

struct var_list {
	char *name;
	char *value;
	var_list *next;
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

var_list *setValue(var_list *list, var_list *data){
  var_list * pointer = list;
  if(pointer == 0){
    var_list * newHead = data;
    return newHead;
  }
  while(pointer->next != 0){
    if(strcmp(pointer->name, data->name) == 0){
      pointer->value = data->value;
      return list;
    }
    pointer = pointer->next;
  }
  pointer->next = data;
  return list;
}

void print_var(var_list *list){
  int counter = 0;
  while(list != 0){
    printf("#%d ",counter);
    printf("name: %s ", list->name);
    printf("variable: %s \n", list->value);
    list = list->next;
    counter++;
  }
}

void list_print(linked_list *list){
  int counter = 0;
  while(list != 0){
    printf("#%d ",counter);
    printf("%s", list->command);
    list = list->next;
    counter++;
  }
}

void rename_var(var_list *list, char* cur_name, char *fut_name){
  int changed = 0;
  int time = 0;
  if(list == 0){
    perror("the list is empty");
  }else{
    while(list != 0 || time == 0){
      time = 1;
      if(strcmp(list->name, cur_name) == 0){
	char *str = calloc(strlen(fut_name)+1, sizeof(char));
	strcpy(str, fut_name);
	free(list->name);
	list->name = str;
	changed = 1;
	break;
      }
      list = list->next;
    }
  }
  if(changed == 0){
    perror("the variable isn't found!");
  }
}  

var_list* delete_var(var_list *list, char* name){
  var_list * pointer = list;
  var_list * backtrace = list;
  int deleted = 0;
  if(list == 0){
    perror("the list is empty");
  }else
  if(strcmp(pointer->name, name) == 0){
    deleted = 1;
    list = backtrace->next;
    free(pointer->name);
    free(pointer->value);
    free(pointer);
  }
  else if(pointer->next != 0){
    int time = 0;
    pointer = pointer->next;
    while(pointer != 0 || time == 0){
      time = 1;
      if(strcmp(pointer->name, name) == 0){
	backtrace->next = pointer->next;
	free(pointer->name);
	free(pointer->value);
	free(pointer);
	deleted = 1;
	break;
      }
      pointer = pointer->next;
      backtrace = backtrace->next;
    }
  }
  if(deleted == 0){
    perror("the variable isn't found!");
  }
  return list;
}	

char* getArgu(var_list *list, char* name){
  int found = 0;
  int time = 0;
  if(list == 0){
    perror("the list is empty");
    return "";
  }
  else{
    while(list != 0 || time == 0){
      time = 1;
      if(strcmp(list->name, name) == 0){
	found = 1;
	return list->value;
      }
      list = list->next;
    }
  }
  if(found == 0){
    perror("the variable isn't found!");
  }
  return "";
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

void var_free(var_list *list){
  var_list *pointer = list;
  while(list != 0){
    free(list->name);
    free(list->value);
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
  int input, output;
  int fd[2];
  int piped = 0;
  if(pCmdLine->next != 0){
    if (pipe(fd) == -1){
      perror("pipe");
      exit(EXIT_FAILURE);
    }
    piped = 1;
  }
  int pid = fork();
  if(pid < 0){
    perror("fork");
    exit(1);
  }
  if(pid == 0){ /*child*/
    if(piped == 1){
      close(STDOUT);
      dup(fd[1]);
      close(fd[1]);
      close(fd[0]);
    }
    if(pCmdLine->inputRedirect != 0){
      input = open(pCmdLine->inputRedirect, O_RDONLY);
      dup2(input, 0);
      close(input);
    }
    if(pCmdLine->outputRedirect != 0){
      output = open(pCmdLine->outputRedirect,O_WRONLY | O_CREAT, 0777);
      dup2(output, 1);
      close(output);
    }
    if((execvp(pCmdLine->arguments[0], pCmdLine->arguments)) < 0){
      perror("Error executing the command: ");
      _exit(0);
    }
    if(piped == 1){
      close(fd[1]);
      _exit(EXIT_SUCCESS);
    }
  } /*end of child now the parent*/
  if(piped == 1){
    cmdLine *next_Line = pCmdLine->next;
    close(fd[1]);
    int pid2 = fork();
    if (pid2 == -1){
      perror("fork");
      exit(EXIT_FAILURE);
    }
    if (pid2 == 0){
      close(STDIN);
      dup(fd[0]);
      close(fd[0]);
      close(fd[1]);
      if(next_Line->inputRedirect != 0){
	input = open(next_Line->inputRedirect, O_RDONLY);
	dup2(input, 0);
	close(input);
      }
      if(next_Line->outputRedirect != 0){
	output = open(next_Line->outputRedirect,O_WRONLY | O_CREAT, 0777);
	dup2(output, 1);
	close(output);
      }
      if((execvp(next_Line->arguments[0], next_Line->arguments)) < 0){
	perror("Error executing the command: ");
	_exit(0);
      }
      close(fd[1]);
      _exit(EXIT_SUCCESS);
    }
    else{
      close(fd[0]);
      if((pCmdLine->blocking) == 1){
	waitpid(pid, 0, 0);
      }
      if((next_Line->blocking) == 1){
	waitpid(pid2, 0, 0);
      }
    }
  }
  if((pCmdLine->blocking) == 1){
    waitpid(pid, 0, 0);
  }
} 

int choose_action(cmdLine *pCmdLine, linked_list *head, var_list *var_head){
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
  else if(strcmp(pCmdLine->arguments[0], "env") == 0){
    print_var(var_head);
    return 0;
  }
  else if(strcmp(pCmdLine->arguments[0], "rename") == 0){
    rename_var(var_head, pCmdLine->arguments[1], pCmdLine->arguments[2]);
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
  var_list *var_head = 0;
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
    int i;
    for(i = 0; i<command->argCount; i++){
      if(strncmp(command->arguments[i], "$", 1) == 0){
	char *change = &command->arguments[i][1];
	char *fut_change = getArgu(var_head, change);
	replaceCmdArg(command, i, fut_change);
      }
    }
    if(strcmp(command->arguments[0], "set") == 0){
      var_list *newVar = malloc(sizeof(var_list));
      char *str1 = calloc(strlen(command->arguments[1])+1, sizeof(char));
      char *str2 = calloc(strlen(command->arguments[2])+1, sizeof(char));
      strcpy(str1, command->arguments[1]);
      strcpy(str2, command->arguments[2]);
      newVar->name = str1;
      newVar->value = str2;
      newVar->next = 0;
      var_head = setValue(var_head, newVar);
    }
    else if(strcmp(command->arguments[0], "delete") == 0){
      var_head = delete_var(var_head, command->arguments[1]);
    }
    else if((choose_action(command, head, var_head)) == 1){
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
  var_free(var_head);
  return 0;
}