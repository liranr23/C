#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct virus virus;
 
struct virus {
        int length;
        char *signature;
        char *name;
        virus *next;
};

void PrintHex(char buffer[], int length){
  int i;
  for(i = 0; i<length; i++){
    printf("%02X ", (unsigned char)buffer[i]);
  }
  printf("\n");
}

virus *list_append(virus *virus_list, virus *data){
  virus * pointer = virus_list;
  if(pointer == 0){
    pointer = data;
    return pointer;
  }
  while(pointer->next != 0){
    pointer = pointer->next;
  }
  pointer->next = data;
  return virus_list;
}

void list_print(virus *virus_list){
  while(virus_list != 0){
    printf("Virus name: %s\n",virus_list->name);
    printf("Virus size: %d\n",virus_list->length);
    printf("signature:\n");
    PrintHex(virus_list->signature, virus_list->length);
    virus_list = virus_list->next;
  }
}

void list_free(virus *virus_list){
  virus *pointer = virus_list;
  while(virus_list != 0){
    free(virus_list->name);
    free(virus_list->signature);
    virus_list = virus_list->next;
    free(pointer);
    pointer = virus_list;
  }
}

void detect_virus(char *buffer, virus *virus_list, unsigned int size){
  int i=0, ret;
  while(virus_list != 0){
    for(i=0;i<size;++i){
      ret = memcmp(&buffer[i],virus_list->signature, 1);
      if(ret == 0 && (i+virus_list->length)<size){
	ret = memcmp(&buffer[i],virus_list->signature, virus_list->length);
	if(ret == 0){
	  printf("The starting byte location in the suspected file is: %d. Which is: 0x%x \n", i, i);
	  printf("The Virus name is: %s\n", virus_list->name);
	  printf("The Virus size is: %d\n", virus_list->length);
	}
      }
    }
    virus_list = virus_list->next;
  }
}
  
int main(int argc, char **argv){
  FILE * input = stdin;
  input = fopen(argv[1],"rb");
  fseek(input, 0, SEEK_END); 
  int length = ftell(input); 
  fseek(input, 0, SEEK_SET); 
  char *buffer = (char *)malloc((length+1)*sizeof(char));
  fread(buffer, length, 1, input);
  FILE * susfile = fopen(argv[2],"rb");
  fseek(susfile, 0, SEEK_END); 
  int viruslen = ftell(susfile); 
  fseek(susfile, 0, SEEK_SET); 
  char *suspect_buffer = (char *)malloc((viruslen+1)*sizeof(char));
  fread(suspect_buffer, viruslen, 1, susfile);
  int i = 0;
  virus *head = 0;
  while(i<length){
    int virus_size = 0;
    int stop = i+4;
    int time = 0;
    while(i<stop){
      virus_size = virus_size + (int)buffer[i]*(int)pow(16,time);
      ++i;
      ++time;
    }
    stop = i+virus_size;
    char *virus_name = (char *)calloc(100,sizeof(char));
    char *sign = (char *)malloc((virus_size+1)*sizeof(char));
    int j=0;
    while(i<stop){
      sign[j] = buffer[i];
      ++i;
      ++j;
    }
    j=0;
    while((int)buffer[i] != 0){
      virus_name[j] = buffer[i];
      ++i;
      ++j;
    }
    ++i;
    virus *newNode = malloc(sizeof(virus));
    newNode->name = virus_name;
    newNode->signature = sign;
    newNode->length = virus_size;
    newNode->next = 0;
    head = list_append(head, newNode);
 }
  detect_virus(suspect_buffer, head, viruslen);
  list_free(head);
  free(buffer);
  free(suspect_buffer);
  fclose(susfile);
  fclose(input);
  return 0;
}

