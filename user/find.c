#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *pathname, char *filename)
{
  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;
  if ((fd = open(pathname, 0)) < 0)
  {
    fprintf(2, "Cannot open file %s\n", pathname);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", pathname);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    p = pathname + strlen(pathname);
    while (p >= pathname && *p != '/')
    {
      p--;
    }
    p++;
    if (!strcmp(p, filename))
    {
      fprintf(2, pathname);
      fprintf(2, "\n");
    }
    break;
  case T_DIR:
    if(strlen(pathname) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, pathname);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, filename);
    }
    break;
  default:
    break;
  }
  close(fd);
}

int
main(int argc, char **argv)
{
  if (argc < 3)
  {
    fprintf(2, "Usage: find pathname filename...\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  
  exit(0);
}