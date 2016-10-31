#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>  

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
      printf("Error the file isn't ELF\n");
      exit(1);
    }
  }
}

int main(int argc, char **argv){
  int fd;
  void *map_start; /* will point to the start of the memory mapped file */
  struct stat fd_stat; /* this is needed to  the size of the file */
  Elf32_Ehdr *header; /* this will point to the header structure */

  if( (fd = open(argv[1], O_RDWR)) < 0 ) {
     perror("error in open");
     exit(-1);
  }

  if( fstat(fd, &fd_stat) != 0 ) {
     perror("stat failed");
     exit(-1);
  }

  if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0)) <0 ) {
     perror("mmap failed");
     exit(-4);
  }
  header = (Elf32_Ehdr *) map_start;
  int num_of_section_headers;
  int header_table_offset;
  Elf32_Shdr * section_header;
  header = (Elf32_Ehdr *) map_start;
  check_elf(header);
  int i;
  header_table_offset = header->e_shoff;
  num_of_section_headers = header->e_shnum;
  section_header = (Elf32_Shdr *)(map_start + header_table_offset);
  Elf32_Shdr *sh_strtab = &section_header[header->e_shstrndx];
  Elf32_Sym *symbols; 
  for (i=0; i < num_of_section_headers; i++){
    if(section_header[i].sh_type == SHT_SYMTAB || section_header[i].sh_type == SHT_DYNSYM){
      char* sym_name;
      if(section_header[i].sh_type == SHT_SYMTAB){
	sym_name = ".symtab";
      }else{
	sym_name = ".dynsym";
      }
      symbols = (Elf32_Sym *)((char *)map_start + section_header[i].sh_offset);
      int size;
      size = (section_header[i].sh_size)/(sizeof(Elf32_Sym));
      printf("Symbol table '%s' contains %d entries:\n",sym_name, size);
      printf("  [Num] %-15s %-8s %-8s %-6s\n","Value", "Ndx", "Section Name", "Symbol Name");
      int j;
      for(j=0; j<size;j++){
	printf("  [%02d] %08x", j, symbols[j].st_value);
	Elf32_Shdr *cur_sec = &section_header[symbols[j].st_shndx];
	if(symbols[j].st_shndx == 0){
	  printf("         %-8s", "UNS");
	}
	else if(symbols[j].st_shndx == 0xfff1){
	  printf("         %-8s", "ABS");
	}
	else{
	  printf("         %d",symbols[j].st_shndx);
	}
	printf("        %s",(char*)(map_start + sh_strtab->sh_offset + cur_sec->sh_name));
	Elf32_Shdr *linked_sec = &section_header[section_header[i].sh_link];
	printf("            %s", (char*)(map_start + linked_sec->sh_offset + symbols[j].st_name));
	printf("\n");
      }
    }
  }

  munmap(map_start, fd_stat.st_size);
  return 0;
}