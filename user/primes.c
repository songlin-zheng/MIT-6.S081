#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

#define R 0
#define W 1

void
primes(int *fd_prev)
{
  int fd_next[2];
  pipe(fd_next);

  int prime;
  int m = read(fd_prev[R], &prime, sizeof(prime));
  if (m == 0)
  {
    return;
  }
  fprintf(2, "prime %d\n", prime);
  if (fork() == 0)
  {
    close(fd_prev[R]);
    close(fd_next[W]);
    primes(fd_next);
  }
  else
  {
    int next;
    close(fd_next[R]);
    while (read(fd_prev[R], &next, sizeof(next)) != 0)
    {
      if (next % prime != 0)
      {
        write(fd_next[W], &next, sizeof(next));
      }
    }
    close(fd_prev[R]);
    close(fd_next[W]);
    wait(0);
  }
  
  return;
}

int
main(int argc, char **argv)
{
  int fd[2];
  pipe(fd);
  int limit = 35;
  if (argc > 1 && atoi(argv[1]))
  {
    limit = atoi(argv[1]);
  }

  if (fork() == 0)
  {
    close(fd[W]);
    primes(fd);
  }
  else
  {
    close(fd[R]);
    for (int i = 2; i <= limit; i++)
    {
      write(fd[W], &i, sizeof(i));
    }
    close(fd[W]);
    wait(0);
  }
  exit(0);
}

// another solution from https://github.com/skyzh/6.S081-xv6-riscv-fall19/blob/master/user/primes.c
// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"

// #define R 0
// #define W 1

// int
// main(int argc, char *argv[])
// {
//   int numbers[100], cnt = 0, i;
//   int fd[2];
//   for (i = 2; i <= 35; i++) {
//     numbers[cnt++] = i;
//   }
//   while (cnt > 0) {
//     pipe(fd);
//     if (fork() == 0) {
//       int prime, this_prime = 0;
//       close(fd[W]);
//       cnt = -1;
//       while (read(fd[R], &prime, sizeof(prime)) != 0) {
//         if (cnt == -1) {
//           this_prime = prime;
//           cnt = 0;
//         } else {
//           if (prime % this_prime != 0) numbers[cnt++] = prime;
//         }
//       }
//       printf("prime %d\n", this_prime);
//       close(fd[R]);
//     } else {
//       close(fd[R]);
//       for (i = 0; i < cnt; i++) {
//         write(fd[W], &numbers[i], sizeof(numbers[0]));
//       }
//       close(fd[W]);
//       wait();
//       break;
//     }
//   }
//   exit();
// }