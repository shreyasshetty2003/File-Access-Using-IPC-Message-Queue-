#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define BUF_SIZE 8192
#define MSG_TYPE_REQUEST 1  
#define MSG_TYPE_RESPONSE 2
#define MSG_QUEUE_KEY 4000
typedef struct {
    long mtype;       
    char filename[100]; 
    char content[1024]; 
} message_t;
int main() {
	int input_fd, output_fd;
	ssize_t ret_in, ret_out;
	char buffer[BUF_SIZE];
	char a[150] = "/home/shreyas/Documents/" , c[150] = "/home/shreyas/Documents/", pass[50] = " ";
    	int msgqid, request_qid, response_qid;
    	message_t msg;
    while (1) {
    char b[20];
    strcpy(a, "/home/shreyas/Documents/") ;strcpy(c,"/home/shreyas/Documents/"); strcpy(pass, " ");
    request_qid = msgget(MSG_QUEUE_KEY, IPC_CREAT | 0666);
    if (request_qid == -1) {
        perror("msgget (request)");
        exit(1);
    }
    response_qid = msgget(MSG_QUEUE_KEY + 1, IPC_CREAT | 0666);
    if (response_qid == -1) {
        perror("msgget (response)");
        exit(1);
    }
	msg.mtype = MSG_TYPE_RESPONSE;
	if (msgrcv(response_qid, &msg, sizeof(msg.filename), MSG_TYPE_RESPONSE, 0) == -1) {
		perror("msgrcv");
		exit(1);
	}
	strcpy(pass,msg.filename);
    	while(strcmp("PASSWORD", pass)){
		strcpy(msg.filename, "Failure");
		if (msgsnd(response_qid, &msg, sizeof(msg.filename), 0) == -1) {
		    perror("msgsnd");
		    exit(1);
		}
		if (msgrcv(response_qid, &msg, sizeof(msg.filename), MSG_TYPE_RESPONSE, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}
		strcpy(pass,msg.filename);
	}
	strcpy(msg.filename, "Success");
	if (msgsnd(response_qid, &msg, sizeof(msg.filename), 0) == -1) {
	    perror("msgsnd");
	    exit(1);
	}
	msg.mtype = MSG_TYPE_REQUEST;
        if (msgrcv(request_qid, &msg, sizeof(msg.filename), MSG_TYPE_REQUEST, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("Client requested file: %s\n", msg.filename);
        strcat(a,msg.filename);
        strcpy(b,msg.filename);
        if (msgrcv(request_qid, &msg, sizeof(msg.filename), MSG_TYPE_REQUEST, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("file copied to: %s\n", msg.filename);
        strcat(c,msg.filename);
        input_fd = open (a, O_RDONLY);
	if (input_fd == -1) {
		perror ("open");
		return 2;
	}
	strcat(c,"/");
	strcat(c,b);
	output_fd = open(c, O_WRONLY | O_CREAT, 0644);
	if(output_fd == -1){
		perror("open");
		return 3;
	}
	while((ret_in = read (input_fd, &buffer, BUF_SIZE)) > 0){
		ret_out = write (output_fd, &buffer, (ssize_t) ret_in);
		if(ret_out != ret_in){
			perror("write");
			return 4;
		}
	}
	close (input_fd);
	close (output_fd);
	if (msgrcv(request_qid, &msg, sizeof(msg.filename), MSG_TYPE_REQUEST, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        input_fd = open (c, O_RDONLY);
	if (input_fd == -1) {
		perror ("open");
		return 2;
	}
	output_fd = open(a, O_WRONLY | O_CREAT, 0644);
	if(output_fd == -1){
		perror("open");
		return 3;
	}
	while((ret_in = read (input_fd, &buffer, BUF_SIZE)) > 0){
		ret_out = write (output_fd, &buffer, (ssize_t) ret_in);
		if(ret_out != ret_in){
			perror("write");
			return 4;
		}
	}
	close (input_fd);
	close (output_fd);
	int output_fd = unlink(c);
	if(output_fd == -1){
		perror("unlink error");
		return 3;
	}
    }
    return 0;
}
/*FILE *fp = fopen(msg.filename, "rb");
        if (fp == NULL) {
            perror("fopen");
            msg.mtype = MSG_TYPE_RESPONSE;
            strcpy(msg.content, "Error: File not found");
            msgsnd(response_qid, &msg, sizeof(msg.content), 0);
            continue;
        }
        size_t bytes_read = fread(msg.content, 1, sizeof(msg.content) - 1, fp);
        msg.content[bytes_read] = '\0';
        fclose(fp);
        msg.mtype = MSG_TYPE_RESPONSE;
        if (msgsnd(response_qid, &msg, sizeof(msg.content), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }*/

