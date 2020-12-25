#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(2, "Usage: sleep time...");
    exit(1);
  }

  int time = atoi(argv[1]);

  //TO-DO
  while (time > 0)
  {
    if (sleep(time--) < 0)
    {
      fprintf(2, "sleep");
      exit(1);
    }
  }

  exit(0);
}