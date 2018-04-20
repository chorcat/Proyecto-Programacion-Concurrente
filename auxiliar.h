#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <semaphore.h>

#define num_nodos 3

int get_msgq(const char*, int, int);
void s_msgsnd(int, const void*, size_t, int);
ssize_t s_msgrcv(int, void*, size_t, long, int);
void copiarArray(int*,int*);
void vaciarCola(int qid);
