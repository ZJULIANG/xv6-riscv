#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void proccnmd(int argc, char *argv[], char *cmd)
{
    char *subargv[MAXARG];
    for (int i = 1; i < argc; i++)
    {
        subargv[i-1]=argv[i];
    }
    subargv[argc-1] = cmd;
    if(fork()==0){
        exec(subargv[0], subargv);
        printf("exec %s failed\n", subargv[0]);
        exit(1);
    }
    else{
        wait(0);
    }
}

int main(int argc, char *argv[])
{
    char buf[512];
    char *p = buf;
    while(0 != read(0, p, 1)){
        if(*p == '\n'){
            *p = 0;
            proccnmd(argc, argv, buf);
            memset(buf, 0, sizeof(buf));
            p = buf;
        }else{
            p=p+1;
        }
    }
    exit(0);
}