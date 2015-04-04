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
        int quit=0;
        
        while(quit==0){
            char *s = (char*) malloc( 100 );
            printf("Please enter appointment:\n");
            fgets(s, 100, stdin);
            //call child
            if(strstr(s, "endProgram") != NULL){
                printf("Bye!");
                exit(0);
            }
            int target_child=-1;
            for (i=0;i<num_child;i++){
                if(strstr(s, argv[i+1]) != NULL){
                    target_child = i;
                }
            }
            write(fd[target_child][1],s,100);
            while (1) {
                char command[80];
                read(fdtwo[0][0], command, 80);
                if(strstr(command, "DONE") != NULL){
                    break;
                }
            }

        }
        
    }else{
        //Child process
        while(1){
            //read command
            char command[80];
            read(fd[child_id][0], command, 80);
            if(strstr(command, "addStudy") != NULL){
                printf("addStudy,im %s\n",argv[child_id+1]);
                write(fdtwo[0][1],"DONE",4);
            }
            if(strstr(command, "addAssignment") != NULL){
                printf("addAssignment,im %s\n",argv[child_id+1]);
                write(fdtwo[0][1],"DONE",4);
            }
            if(strstr(command, "addProject") != NULL){
                printf("addProject,im %s\n",argv[child_id+1]);
                write(fdtwo[0][1],"DONE",4);
            }
            if(strstr(command, "addGathering") != NULL){
                printf("addGathering,im %s\n",argv[child_id+1]);
                write(fdtwo[0][1],"DONE",4);
            }
            if(strstr(command, "addBatch") != NULL){
                printf("addBatch,im %s\n",argv[child_id+1]);
                write(fdtwo[0][1],"DONE",4);
            }
            if(strstr(command, "printSchd") != NULL){
                printf("printSchd,im %s\n",argv[child_id+1]);
                write(fdtwo[0][1],"DONE",4);
            }
        }
        
    }
}
