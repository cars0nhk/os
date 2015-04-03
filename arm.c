#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int num_child = argc-1;
    int i;
    int cid[num_child];
    int fd[num_child][2];
    int fdtwo[num_child][2];
    int ppid = getpid();
    int pid = ppid;
    int child_id = 0;
    //prepare pipe
    for(i=0;i<num_child;i++){
        if(ppid == pid){
            //pipe for parent to child
            if(pipe(fd[i])<0){
                printf("Fail to create pipe");
            }
            //pipe for child to parent
            if(pipe(fdtwo[i])<0){
                printf("Fail to create pipe");
            }
        }
    }
    
    //Fork process
    for(i=0;i<num_child;i++){
        if(ppid == pid){
            cid[i] = fork();
            pid=getpid();
            child_id=i;
        }
    }
    pid = getpid();
    if(ppid == pid){
        //parent process
        printf("Parent: all child id:");
        //print child id
        for (i=0;i<num_child;i++)
            printf("%d ",cid[i]);
        printf("\n");
        int quit=0;
        
        while(quit==0){
            
            char *s = (char*) malloc( 100 );
            scanf("%s",s);
            //call child
            if(strstr(s, "QUIT") != NULL){
                exit(0);
            }
            for (i=0; i<num_child; i++) {
                write(fd[i][1],s,100);
            }
        }
        
    }else{
        //Child process
        while(1){
            //read command
            char command[80];
            read(fd[child_id][0], command, 80);
            if(strstr(command, "PRINTHIHI") != NULL){
                printf("hihi,im child %d\n",pid);
            }
            if(strstr(command, "PRINTBYEBYE") != NULL){
                printf("byebye,im child %d\n",pid);
            }
        }
        
    }
}
