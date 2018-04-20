#include "proyecto.h"
#include "auxiliar.h"
//#define FIN_LECTORES_NODO 101

void *Receptor_Peticiones(void *arg) {
	int id_nodo_recibido, tipo_proceso_recibido;
	int id_cola = get_msgq("/bin/mv", id_nodo, 0777 | IPC_CREAT);
	vaciarCola(id_cola);
	id_cola = get_msgq("/bin/mv", id_nodo, 0777 | IPC_CREAT);
	printf("id_cola receptor peticiones %i\n", id_cola);
	struct msgbuf solicitud;

	while (1) {
		s_msgrcv(id_cola, (struct msgbuf *) &solicitud, sizeof(solicitud),
		REQUEST, 0);
		id_nodo_recibido = solicitud.id_nodo;
		tipo_proceso_recibido = solicitud.tipo_proceso;
		printf("[Nodo %d] He recibido una peticion de testigo del nodo %d y tipo_proceso %d\n", id_nodo, id_nodo_recibido, tipo_proceso_recibido);

		if (tipo_proceso_recibido == ESCRITORI) {
			sem_wait(&sem_pet_escritorI);
			pet_escritorI[id_nodo_recibido]++;
			sem_post(&sem_pet_escritorI);

			sem_wait(&SemTestigo);
			sem_wait(&sem_pet_escritorI);
			sem_wait(&sem_at_escritorI);
			if (testigo && !dentro()
					&& pet_escritorI[id_nodo_recibido]
							> at_escritorI[id_nodo_recibido]) {
				sem_post(&SemTestigo);
				sem_post(&sem_pet_escritorI);
				sem_post(&sem_at_escritorI);
				enviarTestigo(id_nodo_recibido);
			} else {
				sem_post(&SemTestigo);
				sem_post(&sem_pet_escritorI);
				sem_post(&sem_at_escritorI);
			}
		}

		if (tipo_proceso_recibido == ESCRITORII) {
			sem_wait(&sem_pet_escritorII);
			pet_escritorII[id_nodo_recibido]++;
			sem_post(&sem_pet_escritorII);

			sem_wait(&SemTestigo);
			sem_wait(&sem_pet_escritorII);
			sem_wait(&sem_at_escritorII);
			if (testigo && !dentro()
					&& pet_escritorII[id_nodo_recibido]
							> at_escritorII[id_nodo_recibido]) {
				sem_post(&SemTestigo);
				sem_post(&sem_pet_escritorII);
				sem_post(&sem_at_escritorII);
				enviarTestigo(id_nodo_recibido);
			} else {
				sem_post(&SemTestigo);
				sem_post(&sem_pet_escritorII);
				sem_post(&sem_at_escritorII);
			}
		}

		if (tipo_proceso_recibido == LECTOR) {
			sem_wait(&sem_pet_lectores);
			pet_lectores[id_nodo_recibido]++;
			sem_post(&sem_pet_lectores);

			sem_wait(&SemTestigo);
			sem_wait(&sem_pet_lectores);
			sem_wait(&sem_at_lectores);
			sem_wait(&semLectoresSC);
			if (testigo && lectoresSC > 0) {
				sem_post(&SemTestigo);
				sem_post(&sem_pet_lectores);
				sem_post(&sem_at_lectores);
				sem_post(&semLectoresSC);
				enviarTestigo(id_nodo_recibido);
			} else if (testigo && !dentro()
					&& pet_lectores[id_nodo_recibido]
							> at_lectores[id_nodo_recibido]) {
				sem_post(&SemTestigo);
				sem_post(&sem_pet_lectores);
				sem_post(&sem_at_lectores);
				sem_post(&semLectoresSC);
				enviarTestigo(id_nodo_recibido);
			} else {
				sem_post(&SemTestigo);
				sem_post(&sem_pet_lectores);
				sem_post(&sem_at_lectores);
				sem_post(&semLectoresSC);
			}
		}
	}

	return NULL;
}

void *Receptor_Testigo(void *arg) {
	int id_cola = get_msgq("/bin/mv", id_nodo, 0777 | IPC_CREAT);
	//msgctl(id_cola, IPC_RMID, NULL);
	vaciarCola(id_cola);
	id_cola = get_msgq("/bin/mv", id_nodo, 0777 | IPC_CREAT);
	struct msgbuf msg_testigo;
	while (1) {
		s_msgrcv(id_cola, (struct msgbuf *) &msg_testigo, sizeof(msg_testigo),
				TOKEN, 0);
		printf("[Nodo %d] He recibido el testigo.\n", id_nodo);
		sem_wait(&SemTestigo);
		testigo = 1;
		sem_post(&SemTestigo);
		adelantamiento = msg_testigo.num_max_adel;
		prioridad_lectores = msg_testigo.prio_lectores;
		actualizarVectoresAtendidas(msg_testigo.at_lectores,
				msg_testigo.at_escritoresII, msg_testigo.at_escritoresI);
		despertarProcesos();
	}

	return NULL;
}

void *Receptor_Lectores(void *arg) {
	int id_nodo_origen, id_cola2;
	int id_cola = get_msgq("/bin/mv", id_nodo, 0777 | IPC_CREAT);
	//msgctl(id_cola, IPC_RMID, NULL);
	vaciarCola(id_cola);
	id_cola = get_msgq("/bin/mv", id_nodo, 0777 | IPC_CREAT);
	struct msgbuf fin_lectores_nodo1, fin_lectores_nodo2;
	while (1) {
		s_msgrcv(id_cola, (struct msgbuf *) &fin_lectores_nodo1,
				sizeof(fin_lectores_nodo1), PASO_ESCRITOR, 0);
		id_nodo_origen = fin_lectores_nodo1.id_nodo;
		printf("[Nodo %d] He recibido una peticion para comprobar lectores en nodo del nodo %d.\n", id_nodo, id_nodo_origen);
		sem_wait(&semLectoresSC);
		if (lectoresSC != 0) {
			sem_post(&semLectoresSC);
			sem_wait(&fin_lectores);
			printf("SIGNAL fin_lectores\n");
		} else {
			sem_post(&semLectoresSC);
		}
		id_cola2 = get_msgq("/bin/mv", id_nodo_origen, 0777 | IPC_CREAT);
		fin_lectores_nodo2.id_nodo = id_nodo;
		fin_lectores_nodo2.mtype = FIN_LECTORES_NODO;
		printf("[Nodo %d] Envio un mensaje", id_nodo);
		s_msgsnd(id_cola2, (struct msgbuf *) &fin_lectores_nodo2,
				sizeof(fin_lectores_nodo2), 0);
	}
}
