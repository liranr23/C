#include "util.h"

#define STDIN 0
#define STDOUT 1
#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141
#define EXIT_ERR 0x55
#define O_RDONLY 0
#define BUFF_SIZE 8192
#define DT_REG 8
#define MODE 0777

extern int system_call();

struct linux_dirent {
  unsigned long d_ino; /* Inode number */
  unsigned long d_off; /* Offset to next linux_dirent */
  unsigned short d_reclen; /* Length of this linux_dirent */
  char d_name[]; /* Filename (null-terminated) */
};

int main ( int argc , char* argv[] ) {
  int i;
  char suffix;
  int suf = 0;
  for(i=1; i<argc; i++){
    if(strncmp(argv[i], "-s", 2)==0){
      suf = 1;
      suffix = *argv[++i];
    }
  }
  int fd, nread;
  char buf[BUFF_SIZE];
  struct linux_dirent *d;
  int bpos;
  char d_type;
  char * dent_name;
  fd = system_call(SYS_OPEN, ".", O_RDONLY, MODE);
  if(fd < 0){
    system_call(SYS_WRITE, STDOUT, "Error open the current dir.", 28);
    system_call(SYS_EXIT, EXIT_ERR);
  }
  nread = system_call(SYS_GETDENTS, fd, buf, BUFF_SIZE);
  if(nread < 0){
    system_call(SYS_WRITE, STDOUT, "Error with getdents.", 20);
    system_call(SYS_EXIT, EXIT_ERR);
  }
  for(bpos = 0; bpos < nread; ){
    d = (struct linux_dirent *) (buf + bpos);
    d_type = *(buf + bpos + d->d_reclen - 1);
    dent_name = d->d_name;
    if(d_type == DT_REG && suf == 0){
      system_call(SYS_WRITE,STDOUT,dent_name, strlen(dent_name));
      system_call(SYS_WRITE,STDOUT, " ", 1);
    }else{
      if(d_type == DT_REG && suf == 1 && suffix == dent_name[strlen(dent_name)-1]){
	system_call(SYS_WRITE,STDOUT,dent_name, strlen(dent_name));
	system_call(SYS_WRITE,STDOUT, " ", 1);
      }
    }
    bpos += d->d_reclen;
  }
  system_call(SYS_CLOSE, fd);
  system_call(SYS_WRITE, STDOUT, "\n", 1);
  return 0;
}