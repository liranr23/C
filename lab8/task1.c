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

  /* now, the file is mapped starting at map_start.
   * all we need to do is tell *header to point at the same address:
   */

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
  printf("  [Nr] %-15s %-8s %-8s %-6s\n","Section name", "Addr", "Off", "Size");
  for (i=0; i < num_of_section_headers; i++){
    printf("  [%02d] %-15s %08x %08x %06x\n", i, (char*)(map_start + sh_strtab->sh_offset + section_header[i].sh_name) ,section_header[i].sh_addr, section_header[i].sh_offset , section_header[i].sh_size);
  }

  munmap(map_start, fd_stat.st_size);
  return 0;
}