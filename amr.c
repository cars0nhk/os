#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char *argv[]){
    if (argc<4||argc>11){
        printf("Number of user between 3-10.\n");
        exit(1);
    }
    printf("~~WELCOME TO AMR~~\n");
    FILE* output;
    int num_child = argc-1;
    int i,j,k,cid[num_child],fd[num_child][2],fdtwo[num_child][2],fdthree[num_child][2];
    char bigtimetable[1000][100];
    int ppid = getpid();
    int pid = ppid;
    int tablepointer = 0;
    char command2[100];
    int readpointer;
    int child_id = 0;
    char *pch=(char*) malloc( 100 );
    char* data[1000][14];
    char* username[num_child];
    for (i=0; i<num_child; i++){
        username[i] = argv[i+1];
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
            //pipe for child to parent
            if(pipe(fdthree[i])<0){
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
        int num_part=0;
        int num_ok=0;
        while(quit==0){
            char *s = (char*) malloc( 100 );
            printf("Please enter appointment:\n");
            fgets(s, 100, stdin);
            //call child
            if(strstr(s, "endProgram") != NULL){
                printf("Bye!\n");
                exit(0);
            }
            if(strstr(s, "addStudy") != NULL||strstr(s, "addAssignment") != NULL||strstr(s, "addProject") != NULL||strstr(s, "addGathering") != NULL){
                i=0;
                if(strstr(s,"18:00")!=NULL){
                    i=1;
                }else if(strstr(s,"18:30")!=NULL){
                    i=2;
                }else if(strstr(s,"19:00")!=NULL){
                    i=3;
                }else if(strstr(s,"19:30")!=NULL){
                    i=4;
                }else if(strstr(s,"20:00")!=NULL){
                    i=5;
                }else if(strstr(s,"20:30")!=NULL){
                    i=6;
                }else if(strstr(s,"21:00")!=NULL){
                    i=7;
                }else if(strstr(s,"21:30")!=NULL){
                    i=8;
                }
                else{
                    i=10;
                }
                if(strstr(s,"0.5")!=NULL){
                    i+=1;
                }else if(strstr(s,"1.0")!=NULL){
                    i+=2;
                }else if(strstr(s,"1.5")!=NULL){
                    i+=3;
                }else if(strstr(s,"2.0")!=NULL){
                    i+=4;
                }else if(strstr(s,"2.5")!=NULL){
                    i+=5;
                }else if(strstr(s,"3.0")!=NULL){
                    i+=6;
                }else if(strstr(s,"3.5")!=NULL){
                    i+=7;
                }else if(strstr(s,"4.0")!=NULL){
                    i+=8;
                }
                if(i>9){
                    printf("[A appointment's time is out of 18:00-22:00]\n");
                }
                else{
                    strcpy(bigtimetable[tablepointer],s);
                    pch=strtok(s," "); //split using whitespace
                    k=1;
                    while(pch!=NULL){
                        data[tablepointer][k++]=pch; //store into data array
                        pch = strtok (NULL, " ");
                    }
                    tablepointer++;
                    printf("[Pending]\n");}
            }
            else if(strstr(s, "addBatch") != NULL){
                char str[100];
                char* infile=malloc(50);
                pch=strtok(s," ");
                while(pch!=NULL){
                    strcpy(infile,pch);
                    pch=strtok(NULL, "\n");
                }
                if(infile[0]=='-'){
                    for (i = 1; i < strlen(infile); ++i){
                        infile[i-1]=infile[i];
                    }
                    infile[strlen(infile)-1]='\0';
                }
                FILE *ptr_file;
                ptr_file =fopen(infile, "r");
                if (!ptr_file){ //open file error
                    printf("Error opening file\n");
                }
                while(fgets(str,100,ptr_file)!=NULL){ //read a line
                    if(strstr(str,"18:00")!=NULL){
                        i=1;
                    }else if(strstr(str,"18:30")!=NULL){
                        i=2;
                    }else if(strstr(str,"19:00")!=NULL){
                        i=3;
                    }else if(strstr(str,"19:30")!=NULL){
                        i=4;
                    }else if(strstr(str,"20:00")!=NULL){
                        i=5;
                    }else if(strstr(str,"20:30")!=NULL){
                        i=6;
                    }else if(strstr(str,"21:00")!=NULL){
                        i=7;
                    }else if(strstr(str,"21:30")!=NULL){
                        i=8;
                    }
                    else{
                        i=10;
                    }
                    if(strstr(str,"0.5")!=NULL){
                        i+=1;
                    }else if(strstr(str,"1.0")!=NULL){
                        i+=2;
                    }else if(strstr(str,"1.5")!=NULL){
                        i+=3;
                    }else if(strstr(str,"2.0")!=NULL){
                        i+=4;
                    }else if(strstr(str,"2.5")!=NULL){
                        i+=5;
                    }else if(strstr(str,"3.0")!=NULL){
                        i+=6;
                    }else if(strstr(str,"3.5")!=NULL){
                        i+=7;
                    }else if(strstr(str,"4.0")!=NULL){
                        i+=8;
                    }
                    if(i>9){
                        printf("[A appointment's time is out of 18:00-22:00]\n");
                    }
                    else{
                        strcpy(bigtimetable[tablepointer++],str); //store into card array
                    }
                }
                fclose(ptr_file);
                printf("[Pending]\n");
            }else if(strstr(s, "printSchd -fcfs") != NULL||strstr(s, "printSchd - fcfs") != NULL){
                j=0;
                char *okreason = malloc(100);
                char *noreason = malloc(100);
                for(readpointer=0;readpointer<tablepointer;readpointer++){
                    memset(okreason, 0, 100);
                    memset(noreason, 0, 100);
                    int player[num_child];
                    j=0;
                    for (i=0;i<num_child;i++){
                        if(strstr(bigtimetable[readpointer], username[i]) != NULL){
                            player[j++]=i;
                        }
                    }
                    
                    num_ok=0;
                    for (i=0;i<num_child;i++){
                        if(strstr(bigtimetable[readpointer], username[i]) != NULL){
                            char *ptr = malloc(100);
                            strcpy(ptr, bigtimetable[readpointer]);
                            strcat(ptr, "check");
                            write(fd[i][1],ptr,100);
                            read(fdtwo[0][0],command2,100);
                            num_part++;
                            if(strstr(command2, "ok") !=NULL){
                                num_ok++;
                                strcat(okreason,username[i]);
                                strcat(okreason," ");
                            }
                            else{
                                strcat(noreason,username[i]);
                                strcat(noreason," ");
                            }
                        }
                    }
                    int p,q;
                    if(num_ok==j){
                        for(p=0;p<j;p++){
                            q=player[p];
                            write(fdthree[q][1],okreason,100);
                            write(fd[q][1],bigtimetable[readpointer],100);
                            read(fdtwo[0][0],command2,100);
                            
                            
                        }
                    }else{
                        for(p=0;p<j;p++){
                            char *rejectptr = malloc(100);
                            q=player[p];
                            strcpy(rejectptr, "reject");
                            strcat(rejectptr, bigtimetable[readpointer]);
                            write(fdthree[q][1],noreason,100);
                            write(fd[q][1],rejectptr,100);
                            read(fdtwo[0][0],command2,100);
                            
                        }
                    }
                }
                output=fopen("FCFS_Schd.dat","w");
                fprintf(output,"***Appointment Schedule - ACCEPTED***\n\n");
                fclose(output);
                for(i=0;i<num_child;i++){
                    for (i=0;i<num_child;i++){
                        write(fd[i][1],"printOKfcfs",100);
                        read(fdtwo[0][0],command2,100);
                    }
                }
                if(num_ok<j){
                    output=fopen("FCFS_Schd.dat","a");
                    fprintf(output,"***Appointment Schedule - REJECTED***\n\n");
                    fclose(output);
                }
                for(i=0;i<num_child;i++){
                    for (i=0;i<num_child;i++){
                        write(fd[i][1],"printNOfcfs",100);
                        read(fdtwo[0][0],command2,100);
                    }
                }
                printf("[Done!]\n");
            }else if(strstr(s, "printSchd -prio") != NULL||strstr(s, "printSchd - prio") != NULL){
                char *okreason = malloc(100);
                char *noreason = malloc(100);
                char *joblist[] = {"addAssignment","addProject","addStudy","addGathering"};
                int pp;
                for(pp=0;pp<4;pp++){
                    for(readpointer=0;readpointer<tablepointer;readpointer++){
                        memset(okreason, 0, 100);
                        memset(noreason, 0, 100);
                        if(strstr(bigtimetable[readpointer],joblist[pp])!=NULL){
                            int player[num_child];
                            j=0;
                            for (i=0;i<num_child;i++){
                                if(strstr(bigtimetable[readpointer], username[i]) != NULL){
                                    player[j++]=i;
                                }
                            }
                            
                            num_ok=0;
                            for (i=0;i<num_child;i++){
                                if(strstr(bigtimetable[readpointer], username[i]) != NULL){
                                    char *ptr = malloc(100);
                                    strcpy(ptr, bigtimetable[readpointer]);
                                    strcat(ptr, "check");
                                    write(fd[i][1],ptr,100);
                                    read(fdtwo[0][0],command2,100);
                                    num_part++;
                                    //printf("%d\n", num_part);
                                    if(strstr(command2, "ok") !=NULL){
                                        num_ok++;
                                        strcat(okreason,username[i]);
                                        strcat(okreason," ");
                                    }
                                    else{
                                        strcat(noreason,username[i]);
                                        strcat(noreason," ");
                                    }
                                }
                            }
                            int p,q;
                            if(num_ok==j){
                                for(p=0;p<j;p++){
                                    q=player[p];
                                    write(fdthree[q][1],okreason,100);
                                    write(fd[q][1],bigtimetable[readpointer],100);
                                    read(fdtwo[0][0],command2,100);
                                }
                            }else{
                                for(p=0;p<j;p++){
                                    char *rejectptr = malloc(100);
                                    q=player[p];
                                    strcpy(rejectptr, "reject");
                                    strcat(rejectptr, bigtimetable[readpointer]);
                                    write(fdthree[q][1],noreason,100);
                                    write(fd[q][1],rejectptr,100);
                                    read(fdtwo[0][0],command2,100);
                                }
                            }
                        }//endif
                    }
                }
                output=fopen("PRIO_Schd.dat","w");
                fprintf(output,"***Appointment Schedule - ACCEPTED***\n\n");
                fclose(output);
                for(i=0;i<num_child;i++){
                    for (i=0;i<num_child;i++){
                        write(fd[i][1],"printOKprio",100);
                        read(fdtwo[0][0],command2,100);
                    }
                }
                if(num_ok<j){
                    output=fopen("PRIO_Schd.dat","a");
                    fprintf(output,"***Appointment Schedule - REJECTED***\n\n");
                    fclose(output);
                }
                
                for(i=0;i<num_child;i++){
                    for (i=0;i<num_child;i++){
                        write(fd[i][1],"printNOprio",100);
                        read(fdtwo[0][0],command2,100);
                    }
                }
                printf("[Done!]\n");
            }else{
                printf("Command not exist, please try again.\n");
            }
        }
    }else{
        //Child process
        char * record[1000][13];
        char * okre[1000];
        char * nore[1000];
        for(i=0;i<1000;i++){
            for(j=0;j<12;j++){
                record[i][j] = malloc(sizeof(char)*20);
            }
        }
        for(i=0;i<1000;i++){
            okre[i] = malloc(sizeof(char)*100);
        }
        for(i=0;i<1000;i++){
            nore[i] = malloc(sizeof(char)*100);
        }
        char * reject[1000][13];
        for(i=0;i<1000;i++){
            for(j=0;j<12;j++){
                reject[i][j] = malloc(sizeof(char)*20);
            }
        }
        int recordpointer=0;
        int rejectpointer=0;
        while(1){
            //read command
            char command[100];
            read(fd[child_id][0], command, 100);
            if(strstr(command, "check") != NULL){
                //printf("check\n");
                int checker=1;
                char* temp[13];
                for(j=0;j<12;j++){
                    temp[j] = malloc(sizeof(char)*13);
                }
                pch=strtok(command," ");
                pch=strtok(NULL," "); //split using whitespace
                for(i=1;i<5;i++){
                    strncpy(temp[i],pch,20); //store into data array
                    pch=strtok(NULL, " ");
                }
                for(i=5;i<=12;i++){ //store time
                    temp[i]="0";
                }
                username[child_id][0] = toupper(username[child_id][0]);
                strcpy(temp[1],username[child_id]);
                if(!strcmp(temp[3],"18:00")){
                    i=4;
                }else if(!strcmp(temp[3],"18:30")){
                    i=5;
                }else if(!strcmp(temp[3],"19:00")){
                    i=6;
                }else if(!strcmp(temp[3],"19:30")){
                    i=7;
                }else if(!strcmp(temp[3],"20:00")){
                    i=8;
                }else if(!strcmp(temp[3],"20:30")){
                    i=9;
                }else if(!strcmp(temp[3],"21:00")){
                    i=10;
                }else if(!strcmp(temp[3],"21:30")){
                    i=11;
                }
                int count=atoi(temp[4])*2;
                for(j=i+1;j<i+count+1&&j<13;j++){
                    temp[j]="1";
                }
                for(i=0;i<recordpointer;i++){
                    if(!strcmp(temp[2], record[i][2])){
                        for(j=5;j<13;j++){
                            if(!strcmp(temp[j],"1")&&!strcmp(record[i][j],"1"))
                                checker=0;
                        }
                    }
                }
                if(checker==1){
                    write(fdtwo[0][1], "ok", 2);
                }
                else{
                    write(fdtwo[0][1], "no", 2);
                }
            }else if(strstr(command, "reject")!=NULL){
                char command3[100];
                read(fdthree[child_id][0],command3,100);
                strcpy(nore[rejectpointer], command3);
                pch=strtok(command," ");
                if(strstr(command, "addAssignment") != NULL)
                    reject[rejectpointer][0]="Assignment";
                else if(strstr(command, "addProject") != NULL)
                    reject[rejectpointer][0]="Project";
                else if(strstr(command, "addStudy") != NULL)
                    reject[rejectpointer][0]="Study  ";
                else if(strstr(command, "addGathering") != NULL)
                    reject[rejectpointer][0]="Gathering";
                pch=strtok(NULL," "); //split using whitespace
                for(i=1;i<5;i++){
                    strcpy(reject[rejectpointer][i],pch); //store into data array
                    pch=strtok(NULL, " ");
                }
                for(i=5;i<=12;i++){ //store time
                    reject[rejectpointer][i]="0";
                }
                if(!strcmp(reject[rejectpointer][3],"18:00")){
                    i=4;
                }else if(!strcmp(reject[rejectpointer][3],"18:30")){
                    i=5;
                }else if(!strcmp(reject[rejectpointer][3],"19:00")){
                    i=6;
                }else if(!strcmp(reject[rejectpointer][3],"19:30")){
                    i=7;
                }else if(!strcmp(reject[rejectpointer][3],"20:00")){
                    i=8;
                }else if(!strcmp(reject[rejectpointer][3],"20:30")){
                    i=9;
                }else if(!strcmp(reject[rejectpointer][3],"21:00")){
                    i=10;
                }else if(!strcmp(reject[rejectpointer][3],"21:30")){
                    i=11;
                }
                int count=atof(reject[rejectpointer][4])*2;
                for(j=i+1;j<i+count+1&&j<13;j++){
                    reject[recordpointer][j]="1";
                }
                rejectpointer++;
                write(fdtwo[0][1], "done", 100);
            }else if(strstr(command, "printOK") != NULL){
                if(strstr(command, "fcfs") != NULL)
                    output=fopen("FCFS_Schd.dat","a");
                else{
                    output=fopen("PRIO_Schd.dat","a");
                }
                if(recordpointer!=0){
                    if(recordpointer==1)
                        fprintf(output,"%s, you have 1 appointment.\n\n",username[child_id]);
                    else
                        fprintf(output,"%s, you have %d appointments.\n\n",username[child_id],recordpointer);
                    fprintf(output,"Date\t\tStart\tEnd\t\tType\t\tPeople\n");
                    fprintf(output,"===========================================================================\n");
                    for(i=0;i<recordpointer;i++){
                        fprintf(output,"%s\t%s\t",record[i][2],record[i][3]);
                        int start=0;
                        int end=0;
                        if(!strcmp(record[i][3],"18:00")){
                            start=0;
                        }else if(!strcmp(record[i][3],"18:30")){
                            start=1;
                        }else if(!strcmp(record[i][3],"19:00")){
                            start=2;
                        }else if(!strcmp(record[i][3],"19:30")){
                            start=3;
                        }else if(!strcmp(record[i][3],"20:00")){
                            start=4;
                        }else if(!strcmp(record[i][3],"20:30")){
                            start=5;
                        }else if(!strcmp(record[i][3],"21:00")){
                            start=6;
                        }else if(!strcmp(record[i][3],"21:30")){
                            start=7;
                        }
                        int count=atof(record[i][4])*2;
                        end=start+count;
                        if(end==1)
                            fprintf(output,"18:30");
                        else if(end==2)
                            fprintf(output,"19:00");
                        else if(end==3)
                            fprintf(output,"19:30");
                        else if(end==4)
                            fprintf(output,"20:00");
                        else if(end==5)
                            fprintf(output,"20:30");
                        else if(end==6)
                            fprintf(output,"21:00");
                        else if(end==7)
                            fprintf(output,"21:30");
                        else if(end>=8)
                            fprintf(output,"22:00");
                        fprintf(output,"\t%s \t%s\n",record[i][0],okre[i]);
                    }
                    fprintf(output,"   - End -\n");
                    fprintf(output,"===========================================================================\n");
                    fprintf(output,"\n\n");
                }
                write(fdtwo[0][1],"done",100);
                recordpointer=0;
                fclose(output);
            }else if(strstr(command, "printNO") != NULL){
                if(strstr(command, "fcfs") != NULL)
                    output=fopen("FCFS_Schd.dat","a");
                else{
                    output=fopen("PRIO_Schd.dat","a");
                }
                if(rejectpointer!=0){
                    if(rejectpointer==1)
                        fprintf(output,"%s, there is 1 appointment rejected.\n\n",username[child_id]);
                    else
                        fprintf(output,"%s, there are %d appointments rejected.\n\n",username[child_id],rejectpointer);
                    fprintf(output,"Date\t\tStart\tEnd\t\tType\t\tReasons\n");
                    fprintf(output,"===========================================================================\n");
                    for(i=0;i<rejectpointer;i++){
                        fprintf(output,"%s\t%s\t",reject[i][2],reject[i][3]);
                        int start=0;
                        int end=0;
                        if(!strcmp(reject[i][3],"18:00")){
                            start=0;
                        }else if(!strcmp(reject[i][3],"18:30")){
                            start=1;
                        }else if(!strcmp(reject[i][3],"19:00")){
                            start=2;
                        }else if(!strcmp(reject[i][3],"19:30")){
                            start=3;
                        }else if(!strcmp(reject[i][3],"20:00")){
                            start=4;
                        }else if(!strcmp(reject[i][3],"20:30")){
                            start=5;
                        }else if(!strcmp(reject[i][3],"21:00")){
                            start=6;
                        }else if(!strcmp(reject[i][3],"21:30")){
                            start=7;
                        }
                        int count=atof(reject[i][4])*2;
                        end=start+count;
                        if(end==1)
                            fprintf(output,"18:30");
                        else if(end==2)
                            fprintf(output,"19:00");
                        else if(end==3)
                            fprintf(output,"19:30");
                        else if(end==4)
                            fprintf(output,"20:00");
                        else if(end==5)
                            fprintf(output,"20:30");
                        else if(end==6)
                            fprintf(output,"21:00");
                        else if(end==7)
                            fprintf(output,"21:30");
                        else if(end>=8)
                            fprintf(output,"22:00");
                        strcat(nore[i],"is unavailable.");
                        fprintf(output,"\t%s \t%s\n",reject[i][0],nore[i]);
                    }
                    fprintf(output,"   - End -\n");
                    fprintf(output,"===========================================================================\n");
                    fprintf(output,"\n\n");
                }
                write(fdtwo[0][1],"done",100);
                rejectpointer=0;
                fclose(output);
            }else if(strstr(command, "add") != NULL){
                char command3[100];
                read(fdthree[child_id][0],command3,100);
                strcpy(okre[recordpointer], command3);
                pch=strtok(command," ");
                if(strstr(command, "addAssignment") != NULL)
                    record[recordpointer][0]="Assignment";
                else if(strstr(command, "addProject") != NULL)
                    record[recordpointer][0]="Project";
                else if(strstr(command, "addStudy") != NULL)
                    record[recordpointer][0]="Study  ";
                else if(strstr(command, "addGathering") != NULL)
                    record[recordpointer][0]="Gathering";
                pch=strtok(NULL," "); //split using whitespace
                for(i=1;i<5;i++){
                    strcpy(record[recordpointer][i],pch); //store into data array
                    pch=strtok(NULL, " ");
                }
                for(i=5;i<=12;i++){ //store time
                    record[recordpointer][i]="0";
                }
                if(!strcmp(record[recordpointer][3],"18:00")){
                    i=4;
                }else if(!strcmp(record[recordpointer][3],"18:30")){
                    i=5;
                }else if(!strcmp(record[recordpointer][3],"19:00")){
                    i=6;
                }else if(!strcmp(record[recordpointer][3],"19:30")){
                    i=7;
                }else if(!strcmp(record[recordpointer][3],"20:00")){
                    i=8;
                }else if(!strcmp(record[recordpointer][3],"20:30")){
                    i=9;
                }else if(!strcmp(record[recordpointer][3],"21:00")){
                    i=10;
                }else if(!strcmp(record[recordpointer][3],"21:30")){
                    i=11;
                }
                int count=atof(record[recordpointer][4])*2;
                for(j=i+1;j<i+count+1;j++){
                    record[recordpointer][j]="1";
                }
                recordpointer++;
                write(fdtwo[0][1], "done", 100);
            }
        }
    }
}


