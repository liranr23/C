#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>  
#include <string.h>

int start = 0;
int symsize = 0;


void check_elf(Elf32_Ehdr *header){
  unsigned int elf[5];
  elf[0] = 0x7f;
  elf[1] = 0x45;
  elf[2] = 0x4c;
  elf[3] = 0x46;
  elf[4] = 1;
  int i;
  for(i=0; i<5;i++){
    if(elf[i] != header->e_ident[i]){
      printf("Error the file isn't ELF32\n");
      exit(1);
    }
  }
}

void get_the_info(Elf32_Ehdr *header, Elf32_Sym* symbols, int i,char** arr,int *undarr){
  Elf32_Shdr* sections = (Elf32_Shdr*)((char*)header+(header->e_shoff));
  int size,index_sym_string;
  size = sections[i].sh_size/sizeof(Elf32_Sym);
  char * off= ((char *)header)+(sections[header->e_shstrndx].sh_offset);
  while(strcmp((off+sections[i].sh_name),".strtab" )!=0){
    index_sym_string=i;
    i++;
  }
  index_sym_string++;
  char * symbol_string= (char *)(((char *)header)+sections[index_sym_string].sh_offset);
  int j=0;
  for(i=0; i<size; i++){
    if(strcmp((symbol_string+symbols[i].st_name), "") != 0){
      if (symbols[i].st_shndx == 0){
	undarr[j] = 1;
      }
      else{
	undarr[j] = 0;
      }
      arr[j] = symbol_string+symbols[i].st_name;
      symsize++;
      if(strcmp((symbol_string+symbols[i].st_name), "_start") == 0){
	start++;
      }
      j++;
    }
  }
}

void pass_on_symbols(void *map_start, Elf32_Ehdr *header, int num_of_section_headers, Elf32_Shdr* sections, char **arr,int *undarr){
  int i;
  Elf32_Sym* symbols;
  for (i=0; i < num_of_section_headers; i++){
    if(sections[i].sh_type== SHT_SYMTAB || sections[i].sh_type==SHT_DYNSYM){
      symbols = (Elf32_Sym*)((char*)map_start+sections[i].sh_offset);
      get_the_info(header, symbols, i, arr,undarr);
    }
  }
}

int main(int argc, char **argv){
  int fd;
  int fd2;
  void *map_start; /* will point to the start of the memory mapped file */
  void *map_start2;
  struct stat fd_stat; /* this is needed to  the size of the file */
  struct stat fd_stat2;
  Elf32_Ehdr *header; /* this will point to the header structure */
  Elf32_Ehdr *header2;

  if((fd = open(argv[1], O_RDWR)) < 0){
     perror("error in open");
     exit(-1);
  }
  if(fstat(fd, &fd_stat) != 0 ){
     perror("stat failed");
     exit(-1);
  }
  if((map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0)) <0){
     perror("mmap failed");
     exit(-4);
  } 
  if((fd2 = open(argv[2], O_RDWR)) < 0){
     perror("error in open");
     exit(-1);
  }
  if(fstat(fd2, &fd_stat2) != 0 ){
     perror("stat failed");
     exit(-1);
  }
  if((map_start2 = mmap(0, fd_stat2.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd2, 0)) <0){
     perror("mmap failed");
     exit(-4);
  } 
  header = (Elf32_Ehdr *) map_start;
  header2 = (Elf32_Ehdr *) map_start2;
  check_elf(header);
  check_elf(header2);
  char *symarr[10000];
  char *symarr2[10000];
  int undarr[10000];
  int undarr2[10000];
  int size1=0, size2=0;
  Elf32_Shdr* sections = (Elf32_Shdr*)((char*)header+(header->e_shoff));
  Elf32_Shdr* sections2 = (Elf32_Shdr*)((char*)header2+(header2->e_shoff));
  int num_of_section_headers = header->e_shnum;
  int num_of_section_headers2 = header2->e_shnum;
  pass_on_symbols(map_start, header,num_of_section_headers,sections,symarr,undarr);
  size1 = symsize;
  symsize = 0;
  pass_on_symbols(map_start2,header2,num_of_section_headers2,sections2,symarr2,undarr2);
  size2 = symsize;
  /*now we have the info we want*/
  if(start <= 0){
    printf("_start check: FAILED\n");
    exit(-1);
  }
  printf("_start check: PASSED\n");
  int i,j;
  for(i=0;i<size1;++i){
    for(j=0;j<size2;++j){
      if(strcmp(symarr[i],symarr2[j])==0){
	if(undarr[i] == undarr2[j]){		/*both undefined or defined*/
	  printf("duplicate check: FAILED (%s)\n",symarr[i]);
	  exit(-1);
	}
	undarr[i] = 0;
	undarr2[j] = 0;
      }
    }
  }
  printf("duplicate check: PASSED\n");
  for(i=0; i<size1;++i){
    if(undarr[i] == 1){
      printf("no missing symbols: FAILED (%s)\n",symarr[i]);
      exit(-1);
    }
  }
  for(i=0; i<size2;++i){
    if(undarr2[i] == 1){
      printf("no missing symbols: FAILED (%s)\n",symarr2[i]);
      exit(-1);
    }
  }
  printf("no missing symbols: PASSED\n");
	
  munmap(map_start, fd_stat.st_size);
  munmap(map_start2, fd_stat2.st_size);
  return 0;
}