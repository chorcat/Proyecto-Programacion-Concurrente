#include "auxiliar.h"
#include "proyecto.h"

int get_msgq(const char *pathname, int proj_id, int msgflg) {
	key_t key;
	if((key = ftok(pathname, proj_id)) == -1){
		perror("Error usando ftok()");
		exit(EXIT_FAILURE);
	}
	int qid;
	if((qid = msgget(key, msgflg)) == -1){
		perror("Error usando msgget()");
		exit(EXIT_FAILURE);
	}
	return qid;
}

void s_msgsnd(int msgid, const void *msgp, size_t msgsz, int msgflg) {
	if(msgsnd(msgid, msgp, msgsz, msgflg)){
		perror("Error usando msgsnd()");
		exit(EXIT_FAILURE);
	}
	return;
}

ssize_t s_msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg) {
	ssize_t bytes;
	if((bytes = msgrcv(msqid, msgp, msgsz, msgtyp, msgflg)) == -1){
		perror("Error usando msgrcv()");
		exit(EXIT_FAILURE);
	}
	return bytes;
}

void copiarArray(int from[],int to[]){
	int i;
	for(i=0; i<num_nodos; i++){
		to[i]=from[i];
	}
	return;
}
void vaciarCola(int qid){
	struct msgbuf solicitud;
	int recibido=0;
	do{
		recibido=msgrcv(qid,(struct msgbuf *) &solicitud, sizeof(struct msgbuf) - sizeof(long), 0, IPC_NOWAIT);
	}while(recibido>0);
}
