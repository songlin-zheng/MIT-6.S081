#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char const *argv[])
{
    if (argc < 1)
    {
        fprintf(2, "Usage: pingpong");
        exit(1);
    }

    int p[2];   // suppose p[0] for parent, p[1] for child

    pipe(p);
    fprintf(2, "start forking\n");
    if (fork() == 0)
    {
        close(p[0]);
        read(p[1], 0, 1);
        fprintf(2, "%d: received ping\n", getpid());
        write(p[1], (void *)(' '), 1);
        close(p[1]);
        exit(0);
    }
    close(p[1]);
    write(p[0], (void *)(' '), 1);
    read(p[0], 0, 1);
    fprintf(2, "%d: received pong\n", getpid());
    close(p[0]);

    exit(0);
}
