#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char  buf[80], buf2[80];

    int handcard=atoi(argv[2]);
    char charArray[104];
    char card[52][2];
    int i,j;
    for (i = 0; i < 104; i++)
    {
        scanf( "%1s", &charArray[i]);
    }
    j=0;
    for (i = 0; i < 104;i++)
    {
        card[j][0]=charArray[i++];
        card[j][1]=charArray[i];
        j++;
    }
    int num_child = atoi(argv[1]);
    int k;
    int cid[num_child];
    int fd[num_child][2];
    int fdtwo[num_child][2];
    int ppid = getpid();
    int pid = ppid;
    int child_id = 0;
    
    //prepare pipe
    for(k=0;k<num_child;k++){
        if(ppid == pid){
            if(pipe(fd[k])<0){
                printf("Fail to create pipe");
            }
            if(pipe(fdtwo[k])<0){
                printf("Fail to create pipe");
            }
        }
    }

    
    //Fork process
    for(k=0;k<num_child;k++){
        if(ppid == pid){
            cid[k] = fork();
            pid=getpid();
            child_id=k+1;
        }
    }
    pid = getpid();
    if(ppid == pid){
        //parent process
        printf("Parent: the child players are ");
        int u;
        for (u=0;u<num_child;u++)
            printf("%d ",cid[u]);
        printf("\n");
        //call child
        int nw;
        for (nw=0; nw<num_child; nw++) {
            write(fd[nw][1],"childPrint",sizeof("childPrint"));
        }
        int count=0;
        for (nw=0; nw<num_child; nw++) {
            //read command
            char command[80];
            read(fdtwo[nw][0], command, 80);
            if(strcmp(command, "parentPrint")==0){
                count++;
            }
        if(count==num_child)
            printf("Parent: trump suit %c\n",card[atoi(argv[1])*atoi(argv[2])][0] );
       }
        printf("Parent: child %d to play first\n",handcard%num_child);
        for (nw=0; nw<num_child; nw++) {
            //ask to show
            write(fd[nw][1],"childShow",sizeof("childShow"));
            
            //waiting children
            char c_cardBuf[10];
            read(fdtwo[nw][0],c_cardBuf,10);
            printf("Parent: child %d plays %s\n",nw+1,c_cardBuf);
        }
        int n;
        for (n = 0; n<num_child-1; n++) {
            wait(NULL);
        }
        int x;
        for(n=0; n<num_child-1;n++){
            for(x=0;x<2;x++){
                close(fd[n][x]);
                close(fdtwo[n][x]);
            }
        }
    }else{
        int m=child_id-1;
        int l;
        int cid=getpid();
        while(1){
            //read command
            char command[80];
            read(fd[child_id-1][0], command, 80);
            if(strcmp(command, "childPrint")==0){
                printf("Child %d, pid %d: ",child_id,cid);
                for(l=0;l<handcard;l++)
                {
                    printf("%c%c ",card[m][0],card[m][1]);
                    m=m+num_child;
                }
                printf("\n");
                write(fdtwo[child_id-1][1],"parentPrint",sizeof("parentPrint"));
            }
            break;
        }
        m=child_id-1;
        while(1){
            //read command
            char command[80];
            read(fd[child_id-1][0], command, 80);
            if(strcmp(command, "childShow")==0){
                printf("Child %d: play ",child_id);
                l=0;
                    printf("%c%c ",card[m][0],card[m][1]);
                
                printf("\n");
                write(fdtwo[child_id-1][1],card[m],2);
                 m=m+num_child;
            }
            break;
        }
        
    }
}