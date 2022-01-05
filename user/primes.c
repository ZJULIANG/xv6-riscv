#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define INT_LEN 4
#define MAX_NUM 35

void prime(int *p0)
{
    int p[2];
    pipe(p);
    int base = 0;
    int num = 0;
    int subpid = 0;
    read(*p0, &base, INT_LEN);
    printf("prime %d\n", base);
    if(base==MAX_NUM) return;
    while(0 != read(*p0, &num, INT_LEN)){
        if(num % base != 0){
            if(subpid == 0){
                subpid = fork();
                if(subpid == 0){
                    close(p[1]);
                    prime(&p[0]);
                    close(p[0]);
                    exit(0);
                }
            }
            write(p[1], &num, INT_LEN);
        }
    }
    close(p[0]);
    close(p[1]);
    wait(0);
    return;
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
   if(fork()==0){
       close(p[1]);
       prime(&p[0]);
       close(p[0]);
       exit(0);
   }
   close(p[0]);
   for (int i = 2; i <= 35; i++){
       write(p[1], &i, INT_LEN);
    }
   close(p[1]);
   wait(0);
   exit(0);
}