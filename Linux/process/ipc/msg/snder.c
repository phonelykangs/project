#include"proto.h"
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

int main(){
    key_t key;
    int queue_id;
    int flag;
    struct msg sndmsg;

    key = ftok(PATH_NAME,PROJECT_ID);
    if(key < 0){
        perror("ftok(): ");
        exit(1);
    }

    queue_id = msgget(key,0);
    if(queue_id < 0){
        perror("msgget(): ");
        exit(1);
    }
    
    sndmsg.mtype = 1;
    strcpy(sndmsg.name,"Hello");
    sndmsg.chinese = 80;
    sndmsg.math = 90;

    if(msgsnd(queue_id,&sndmsg,sizeof(sndmsg) - sizeof(long),0) < 0){
        perror("msgsnd() ");
        exit(1);
    }

    puts("OK!");
    msgctl(queue_id,IPC_RMID,NULL);
    exit(0);
}
