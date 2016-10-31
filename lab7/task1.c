#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define BUFFER_SIZE 4096

struct fun_desc {
  char *name;
  void (*fun)(char*,int,void*);
};

char buffer[BUFFER_SIZE] = "/0";

void quit(char *filename, int size, void *mem_buffer){
  exit(0);
}
void mem_display(char *filename, int size, void *mem_buffer){
  unsigned int address;
  int length;
  char str[100];
  printf("Please enter <address> <length> \n");
  if((fgets(str, 100, stdin)) != 0){
    sscanf(str, "%x %d", &address, &length);
  }
  if(address == 0){
    address = (unsigned int)mem_buffer;
  }
  int i=0,j;
  int temp;
  for(i=0; i<size*length;i++){
    unsigned char *pos;
    temp = i;
    for(j=0; j<size ; j++){
      pos = (unsigned char*) (address+temp);
      printf("%.2X", *pos);
      temp++;
    }
    i = i+size -1;
    printf(" ");
  }
  printf("\n");
}
void load_to_memory(char *filename, int size, void *mem_buffer){
  unsigned int address;
  unsigned int location;
  int length;
  char source[100];
  char str[100];
  printf("Please enter <mem-address> <source-file> <location> <length>\n");
  if((fgets(str, 100, stdin)) != 0){
    sscanf(str, "%x %s %x %d", &address,source,&location, &length);
  }
  if(address == 0){
    address = (unsigned int)mem_buffer;
  }
  FILE *file = fopen(source,"r");
  if(file <= 0){
    printf("error opening the file \n");
    exit(1);
  }
  fseek(file, location, SEEK_SET);
  fread((void*)address, length, 1, file);
  fclose(file);
  printf("Loaded %d units into 0x%X \n", length, address);
}
void save_to_file(char *filename, int size, void *mem_buffer){
  unsigned int address;
  int length;
  unsigned int location;
  char str[100];
  char target[100];
  FILE *file;
  printf("Please enter <filename> to save into(0 for default)\n");
  if((fgets(str, 100, stdin)) != 0){
    sscanf(str, "%s", target);
  }
  if((strcmp(target, "0")) == 0){
    file = fopen(filename, "r+");
  }else{
    file = fopen(target, "r+");
  }
  if(file <= 0){
    printf("error opening the file \n");
  }
  printf("Please enter <s-address> <t-location> <length>\n");
  if((fgets(str, 100, stdin)) != 0){
    sscanf(str, "%x %x %d", &address, &location, &length);
  } 
  if(address == 0){
    address = (unsigned int)mem_buffer;
  }
  fseek(file, 0, SEEK_END);
  int lengthOfFile = ftell(file);
  if(lengthOfFile < location){
    printf("ERROR: <t-location> is greater than the size of <target-file> \n");
    fclose(file);
    exit(1);
  }
  fseek(file, location, SEEK_SET);
  fwrite((void*)address, length, size, file);
  fclose(file);
}

int getByteSize(unsigned int val){
  int i=0;
  while(val > 0){
    val>>=8;
    i++;
  }
  return i;
}

void mem_mod(char *filename, int size, void *mem_buffer){
  unsigned int address;
  unsigned int val;
  char str[100];
  printf("Please enter <address><val>\n");
  if((fgets(str, 100, stdin)) != 0){
    sscanf(str, "%x %x", &address, &val);
  } 
  if(address == 0){
    address = (unsigned int)mem_buffer;
  }
  int times = getByteSize(val);
  unsigned int val_byte = val;
  int i=0;
  unsigned char *pos;
  int jumps = (times-1)*8;
  for(i=0; i<times;i++){
    pos = (unsigned char*) (address+i);
    val_byte = (val >> jumps) & 0xff;
    jumps -= 8;
    *pos = val_byte;
  }
}

struct fun_desc menu[] = { { "Mem Display", mem_display }, { "Load Into Memory", load_to_memory }, { "Save Into File", save_to_file }, { "Mem Modify", mem_mod }, { "Quit", quit } ,{ NULL, NULL } };
 
int main(int argc, char **argv){
  char *filename = argv[1];
  int size = 1;
  if(argc == 3){
    size = *argv[2] - '0';
  }
  char option;
  int j=1,i=0;
  while(1){
    printf("File: %s, buffer location: %p, choose action:\n", filename, buffer);
    for(i = 0; i < sizeof(menu) && menu[i].name != NULL ; ++i){
      printf("%d-%s\n", j, menu[i].name);
      j++;
    }
    printf("Option: ");
    option = fgetc(stdin);
    fgetc(stdin);
    menu[option - '0' - 1].fun(filename, size, buffer);
    printf("DONE.\n");
    j = 1;
  }
  return 0;
}