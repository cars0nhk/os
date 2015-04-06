#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char *argv[])
{
    if (argc<4||argc>11)
    {
        printf("Number of user between 3-10.\n");
        exit(1);
    }
    printf("~~WELCOME TO AMR~~\n");
    int num_child = argc-1;
    int i,j,k=0,cid[num_child],fd[num_child][2],fdtwo[num_child][2];
    char bigtimetable[8000][100];
    int ppid = getpid();
    int pid = ppid;
    int tablepointer = 0;
    int readpointer;
    int child_id = 0;
	char* pch;
	char* data[1000][20];
    char *username[argc];
	char* spilt[100];
    for (i=0; i<num_child; i++) {
        username[i] = argv[i+1];
        for(j = 0; username[i][j]; j++){
            username[i][j] = tolower(username[i][j]);
        }
        //username[i][0] = toupper(username[i][0]);
    }
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
                printf("Bye!\n");
                exit(0);
            }
            int target_child=-1;
            //printf("%s",s);
            if(strstr(s, "addStudy") != NULL||strstr(s, "addAssignment") != NULL||strstr(s, "addProject") != NULL||strstr(s, "addGathering") != NULL){
                strcpy(bigtimetable[tablepointer],s);
                pch=strtok(bigtimetable[tablepointer]," "); //split using whitespace
                k=0;
                while(pch!=NULL){
                    data[tablepointer][k++]=pch; //store into data array
                    //printf("%s\n", pch);
                    pch = strtok (NULL, " ");
                }
                tablepointer++;
                for (i=0;i<num_child;i++){
                    if(strstr(s, argv[i+1]) != NULL){
                        target_child = i;
                    }
                }
                if (target_child == -1){
                    printf("User not exist, please try again.\n");
                }
                else{
                    write(fd[target_child][1],s,100);
                    while (1) {
                        char command[80];
                        read(fdtwo[0][0], command, 80);
                        if(strstr(command, "DONE") != NULL){
                            break;
                        }
                    }
                }
            }
            else if(strstr(s, "addBatch") != NULL){
                printf("AddBatch\n");
                //strcpy(bigtimetable[tablepointer++],s);
                /*for (i=0;i<num_child;i++){
                    if(strstr(s, argv[i+1]) != NULL){
                        target_child = i;
                        write(fd[target_child][1],s,100);
                        while (1) {
                            char command[80];
                            read(fdtwo[0][0], command, 80);
                            if(strstr(command, "DONE") != NULL){
                                break;
                            }
                        }
                    }
                }*/
            }
            else if(strstr(s, "printSchd") != NULL){
                printf("%d\n",tablepointer );
                for(readpointer=0;readpointer<tablepointer;readpointer++){
                    for (i = 0; i < sizeof(data[readpointer]); ++i){
                        if(data[readpointer][i]==NULL)
                            break;
                        printf("%s ",data[readpointer][i]);
                    }
                    printf("\n");
                }
                /*for (i=0;i<num_child;i++){
                    target_child = i;
                    write(fd[target_child][1],s,100);
                    while (1) {
                        char command[80];
                        read(fdtwo[0][0], command, 80);
                        if(strstr(command, "DONE") != NULL){
                            break;
                        }
                    }
                }*/
            }
            else{
                printf("Command not exist, please try again.\n");
            }
        }
        
    }else{
        //Child process
        while(1){
            //read command
            char command[80];
            read(fd[child_id][0], command, 80);
            if(strstr(command, "addStudy") != NULL){
            	printf("-> [Pending]\n");
                printf("addStudy,im %s\n",username[child_id]);
                write(fdtwo[0][1],"DONE",4);
            }
            else if(strstr(command, "addAssignment") != NULL){
            	printf("-> [Pending]\n");
                printf("addAssignment,im %s\n",username[child_id]);
                write(fdtwo[0][1],"DONE",4);
            }
            else if(strstr(command, "addProject") != NULL){
            	printf("-> [Pending]\n");
                printf("addProject,im %s\n",username[child_id]);
                write(fdtwo[0][1],"DONE",4);
            }
            else if(strstr(command, "addGathering") != NULL){
            	printf("-> [Pending]\n");
                printf("addGathering,im %s\n",username[child_id]);
                write(fdtwo[0][1],"DONE",4);
            }
            else if(strstr(command, "addBatch") != NULL){
            	printf("-> [Pending]\n");
                printf("addBatch,im %s\n",username[child_id]);
                write(fdtwo[0][1],"DONE",4);
            }
            else if(strstr(command, "printSchd") != NULL){
            	printf("-> [Done!]\n");
                printf("printSchd,im %s\n",username[child_id]);
                write(fdtwo[0][1],"DONE",4);
            }
            else{
                write(fdtwo[0][1],"ERROR",4);
            }
        }
        
    }
}
