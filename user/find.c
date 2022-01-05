#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void findfile(char *path, char *filename){
    int fd;
    char buf[512], *p;
    struct stat st;
    struct dirent de;
    struct stat subst;
    if((fd = open(path, 0)) < 0){
        printf("cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){
        printf("cannot stat %s\n", path);
        close(fd);
        return;
    }
    if(st.type == T_DIR){
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0 || strcmp(de.name, ".")==0 || strcmp(de.name, "..")==0) continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &subst) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if(subst.type == T_DIR){
                findfile(buf, filename);
            }else if(subst.type == T_FILE){
                if(strcmp(de.name, filename)==0){
                    printf("%s\n", buf);
                }
            }
        }
    }else{
        printf("%s is no a dir\n", path);
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3) exit(1);
    char *path = argv[1];
    char *filename = argv[2];
   
    findfile(path, filename);

    exit(0);
}