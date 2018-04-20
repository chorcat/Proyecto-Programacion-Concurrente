#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PASO_ESCRITOR 100
#define REQUEST 300
#define TOKEN 301
#define ESCRITORI 1
#define ESCRITORII 2
#define LECTOR 3


key_t key;

struct msg_escritor {
  long mtype;
  int id_nodo;
};

struct msgbuf {
  long mtype;
  int id_nodo;
  int tipo_proceso;
  int num_peticion;
  int at_escritoresI[3];
  int at_escritoresII[3];
  int at_lectores[3];
  int num_max_adel;
  int prio_lectores;
};

sem_t fin_lectores, paso_sol_escritorI, paso_sol_escritorII, paso_sol_lector;
sem_t SemEscritoresI;
sem_t SemEscritoresII;
sem_t SemEscritoresEsperandoII;
sem_t EscritoresMutex1;
sem_t EscritoresMutex2;
sem_t sem_lectores2;
sem_t SemLectoresVar;
sem_t paso_esc2;
sem_t SemTestigo;
sem_t paso_esc2;
sem_t SemTestigo;
sem_t sem_pet_escritorI;
sem_t sem_pet_escritorII;
sem_t sem_pet_lectores;
sem_t sem_at_escritorI;
sem_t sem_at_escritorII;
sem_t sem_at_lectores;
sem_t finalizar_pago;
sem_t finalizar_anulacion;
sem_t finalizar_prerreserva;
sem_t finalizar_grada;
sem_t finalizar_evento;
sem_t sem_lectores;
sem_t semLectoresSC;
sem_t adelantamientos;
sem_t semNumeroMaximo;

pthread_t thread_receptor_lectores;
pthread_t thread_receptor;
pthread_t thread_escritoresI;
pthread_t thread_escritoresII;
pthread_t thread_lectores;
pthread_t thread_receptor_token;

int escritoresI;
int escritoresII;
int lectores;
int lectoresSC;
int escritoresEsperandoII;
int escritoresIEsperandoConTestigo;
int id_nodo;
int testigo;
int escritoresIIEsperando;
int escritoresIIEsperandoConTestigo;
int adelantamiento;
int max;
int prioridad_lectores;

int pet_escritorI[3];
int pet_escritorII[3];
int pet_lectores[3];

int at_escritorI[3];
int at_escritorII[3];
int at_lectores[3];

int comp_pet_escritorI();
int comp_pet_escritorII();
int comp_pet_lectores();
void solicitud_testigo(int);
void *receptor_lectores();
void actualizarVectoresAtendidas(int[3], int[3], int[3]);
void despertarProcesos();
int dentro();
void enviarTestigo(int);
void actualizarVectoresAtendidas(int[3], int[3], int[3]);
int calcular_nodo_sig();
void  solicitud(int tipo);
void solicitud_especial();
void *EscritorI(void *arg);
void *EscritorII(void *arg);
void *Lector(void *arg);
void *receptor_token();
void *receptor();
void adel();
void finLectores();
void actualizar_atendidas(int);

#define FIN_LECTORES_NODO 101
void *Receptor_Peticiones(void *arg);
void *Receptor_Testigo(void *arg);
void *Receptor_Lectores(void *arg);
void actualizar_atendidas_lector();
