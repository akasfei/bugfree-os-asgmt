#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int _cp(const char *src, const char *des)
{
    int fd_des, fd_src;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_src = open(src, O_RDONLY);
    if (fd_src < 0)
        return -1;

    fd_des = open(des, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_des < 0)
        return -1;

    while (nread = read(fd_src, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        while (nread > 0)
        {
            nwritten = write(fd_des, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                saved_errno = errno;
                close(fd_src);
                if (fd_des >= 0)
                    close(fd_des);
                return -1;
            }
        }
    }

    if (nread == 0)
    {
        if (close(fd_des) < 0)
        {
            fd_des = -1;
            return -1;
        }
        close(fd_src);
        return 0;
    }
}

int main ( int argc, char *argv[] )
{
    if (argc != 3)
    {
        printf("Usage: cp src des\n");
    } 
    else {
        printf("%s - %s - %s\n", argv[0], argv[1], argv[2]);
        if (_cp(argv[1], argv[2]) < 0)
        {
            printf("Copy Error.\n");
            return -1;
        }
    }
    printf("Copy success.\n");
    return 0;
}