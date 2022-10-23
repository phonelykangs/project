#include"proto.h"
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

int main(){
    key_t key;
    int queue_id;
    struct msg rcvmsg;
    key = ftok(PATH_NAME,PROJECT_ID);
    if(key < 0){
        perror("ftok() ");
        exit(1);
    }

    queue_id = msgget(key,IPC_CREAT | 0600);
    if(queue_id < 0){
        perror("msgget(): ");
        exit(1);
    }

    while(1){
        if(msgrcv(queue_id,&rcvmsg,sizeof(rcvmsg) - sizeof(long),0,0) < 0){
            perror("msgrcv() ");
            exit(1);
        }
        printf("%s\n",rcvmsg.name);
        printf("%d\n",rcvmsg.chinese);
        printf("%d\n",rcvmsg.math);
    }
    msgctl(queue_id,IPC_RMID,NULL);
    exit(0);
}