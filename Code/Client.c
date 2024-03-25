#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
typedef struct {
    long mtype;        
    char filename[100]; 
    char content[1024]; 
} message_t;
#define MSG_TYPE_REQUEST 1  
#define MSG_TYPE_RESPONSE 2 

#define MSG_QUEUE_KEY 4000  
#define MSG_QUEUE_KEY_RESPONSE (MSG_QUEUE_KEY + 1)  

int main(int argc, char* argv[]) {
char a[20],b[5] = "quit", pass[30] , j[30]; 
int m;
 int msgqid, response_qid;
	    message_t msg;
	msgqid = msgget(MSG_QUEUE_KEY, 0);  
	    if (msgqid == -1) {
		perror("msgget (request)");
		exit(1);
	    }
	    response_qid = msgget(MSG_QUEUE_KEY_RESPONSE, 0);  
	    if (response_qid == -1) {
		perror("msgget (response)");
		exit(1);
	    }
	    while(1){
		printf("Enter <Filename> <password>: ");
		scanf("%s %s",a,pass);
		int i =0;
		if(!strcmp(a,b)){
			return 0;
		}
		msg.mtype = MSG_TYPE_RESPONSE;
		strcpy(msg.filename,pass);
		if (msgsnd(response_qid, &msg, sizeof(msg.filename), 0) == -1) {
		    perror("msgsnd");
		    }
		if (msgrcv(response_qid, &msg, sizeof(msg.filename), MSG_TYPE_RESPONSE, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}
		strcpy(j,msg.filename);
		while(strcmp(j,"Success")){
			printf("Wrong Password!\nEnter right Password : ");
			scanf("%s",pass);
			strcpy(msg.filename,pass);
		if (msgsnd(response_qid, &msg, sizeof(msg.filename), 0) == -1) {
		    perror("msgsnd");
		    }
		if (msgrcv(response_qid, &msg, sizeof(msg.filename), MSG_TYPE_RESPONSE, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}
		strcpy(j,msg.filename);
		}
	    msg.mtype = MSG_TYPE_REQUEST;
	    strcpy(msg.filename, a);
	    if (msgsnd(msgqid, &msg, sizeof(msg.filename), 0) == -1) {
		perror("msgsnd");
		exit(1);
	    }
	    strcpy(msg.filename, argv[1]);
	    if (msgsnd(msgqid, &msg, sizeof(msg.filename), 0) == -1) {
		perror("msgsnd");
		exit(1);
	    }
	    printf("\nEnter any Integer to svae the file in Server: ");
	    scanf("%d", &m);
	    if (msgsnd(msgqid, &msg, sizeof(msg.filename), 0) == -1) {
		perror("msgsnd");
		exit(1);
	    }
	}
    return 0;
}

