#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
	int p[2];
    char buf;
    pipe(p);
    if(fork() == 0){
        if(read(p[0], &buf, 1) == 1){
            printf("%d: received ping\n", getpid());
        }
        close(p[0]);
        write(p[1], "b", 1);
        close(p[1]);
    }else{
        write(p[1], "a", 1);
        close(p[1]);
        if(read(p[0], &buf, 1) == 1){
            printf("%d: received pong\n", getpid());
        }
        close(p[0]);
    }
	exit(0);
}