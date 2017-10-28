#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
    int fdOriginal;
    int fdNew;
    char buf[512];

    if(argc != 3)
    {
        printf(2, "Usage cp original new");
        return 1;
    }

    fdOriginal = open(argv[1], O_RDONLY);
    fdNew = open(argv[2], O_CREATE | O_WRONLY);

    int n;
    while((n = read(fdOriginal, buf, sizeof(buf))) > 0)
    {
        write(fdNew, buf, n);
    }

    close(fdOriginal);
    close(fdNew);

    exit();
}