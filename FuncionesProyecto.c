#include "proyecto.h"
#include "auxiliar.h"

void actualizarVectoresAtendidas(int at_lectores_recibido[3],
		int at_escritorII_recibido[3], int at_escritorI_recibido[3]) {
	sem_wait(&sem_at_escritorI);
	copiarArray(at_escritorI_recibido,at_escritorI);
	sem_post(&sem_at_escritorI);
	sem_wait(&sem_at_escritorII);
	copiarArray(at_escritorII_recibido,at_escritorII);
	sem_post(&sem_at_escritorII);
	sem_wait(&sem_at_lectores);
	copiarArray(at_lectores_recibido,at_lectores);
	sem_post(&sem_at_lectores);
	return;
}

void despertarProcesos() {
	printf("Me llego el testigo voy a despertar procesos \n");
	fflush(stdout);
	sem_wait(&SemLectoresVar);
	if (lectores > 0) {
		sem_post(&SemLectoresVar);
		if(adelantamiento>=max&&!escritoresI&&!escritoresII){
			sem_post(&semNumeroMaximo);
			sem_post(&sem_lectores);
		}
		else{
		sem_post(&paso_sol_lector);
		sem_wait(&semLectoresSC);
		if (lectoresSC > 0) {
			sem_post(&sem_lectores);
			sem_post(&semLectoresSC);
		} else {
			sem_post(&semLectoresSC);
		}
		}
	} else {
		sem_post(&SemLectoresVar);
	}
	sem_wait(&SemEscritoresII);
	if (escritoresII > 0) {
		sem_post(&SemEscritoresII);
		if (escritoresIIEsperandoConTestigo > 0) {
			sem_post(&EscritoresMutex2);
		} else {
			sem_post(&paso_sol_escritorII);
		}
	} else {
		sem_post(&SemEscritoresII);
	}
	sem_wait(&SemEscritoresI);
	if (escritoresI > 0) {
		sem_post(&SemEscritoresI);
		if (escritoresIEsperandoConTestigo > 0) {
			sem_post(&EscritoresMutex1);
		} else {
			sem_post(&paso_sol_escritorI);
		}
	} else {
		sem_post(&SemEscritoresI);
	}
	return;
}


int dentro() {
	sem_wait(&SemEscritoresI);
	sem_wait(&SemEscritoresII);
	sem_wait(&SemLectoresVar);
	if (escritoresI > 0 || escritoresII > 0 || lectores > 0) {
		sem_post(&SemEscritoresI);
		sem_post(&SemEscritoresII);
		sem_post(&SemLectoresVar);
		return 1;
	}
	sem_post(&SemEscritoresI);
	sem_post(&SemEscritoresII);
	sem_post(&SemLectoresVar);
	return 0;
}

void enviarTestigo(int id_nodo_sig) {
	struct msgbuf mensaje;
	solicitud_especial();
	sem_wait(&semLectoresSC);
	if (lectoresSC > 0) {
		sem_post(&semLectoresSC);
		mensaje.prio_lectores = 1;
		sem_wait(&sem_lectores);
	} else {
		sem_post(&semLectoresSC);
	}
	testigo = 0;
	mensaje.mtype = TOKEN;
	mensaje.id_nodo = id_nodo;
	mensaje.num_max_adel = adelantamiento;

	copiarArray(at_escritorI, mensaje.at_escritoresI);
	copiarArray(at_escritorII, mensaje.at_escritoresII);
	copiarArray(at_lectores, mensaje.at_lectores);
	int queue_id = get_msgq("/bin/mv", id_nodo_sig, 0777 || IPC_CREAT);
	s_msgsnd(queue_id, (struct msgbuf*) &mensaje,
			sizeof(struct msgbuf) - sizeof(long), 0);
	return;
}

int comp_pet_escritorI() {
	sem_wait(&sem_pet_escritorI);
	sem_wait(&sem_at_escritorI);
	if (pet_escritorI[0] > at_escritorI[0]) {
		sem_post(&sem_pet_escritorI);
		sem_post(&sem_at_escritorI);
		return 0;
	}
	if (pet_escritorI[1] > at_escritorI[1]) {
		sem_post(&sem_pet_escritorI);
		sem_post(&sem_at_escritorI);
		return 1;
	}
	if (pet_escritorI[2] > at_escritorI[2]) {
		sem_post(&sem_pet_escritorI);
		sem_post(&sem_at_escritorI);
		return 2;
	}
	sem_post(&sem_pet_escritorI);
	sem_post(&sem_at_escritorI);
	return -1;

}

int comp_pet_escritorII() {
	sem_wait(&sem_pet_escritorII);
	sem_wait(&sem_at_escritorII);
	if (pet_escritorII[0] > at_escritorII[0]) {
		sem_post(&sem_pet_escritorII);
		sem_post(&sem_at_escritorII);
		return 0;
	}
	if (pet_escritorII[1] > at_escritorII[1]) {
		sem_post(&sem_pet_escritorII);
		sem_post(&sem_at_escritorII);
		return 1;
	}
	if (pet_escritorII[2] > at_escritorII[2]) {
		sem_post(&sem_pet_escritorII);
		sem_post(&sem_at_escritorII);
		return 2;
	}
	sem_post(&sem_pet_escritorII);
	sem_post(&sem_at_escritorII);
	return -1;

}

int comp_pet_lectores() {
	sem_wait(&sem_pet_lectores);
	sem_wait(&sem_at_lectores);
	if (pet_lectores[0] > at_lectores[0]) {
		sem_post(&sem_pet_lectores);
		sem_post(&sem_at_lectores);
		return 0;
	}
	if (pet_lectores[1] > at_lectores[1]) {
		sem_post(&sem_pet_lectores);
		sem_post(&sem_at_lectores);
		return 1;
	}
	if (pet_lectores[2] > at_lectores[2]) {
		sem_post(&sem_pet_lectores);
		sem_post(&sem_at_lectores);
		return 2;
	}
	sem_post(&sem_pet_lectores);
	sem_post(&sem_at_lectores);
	return -1;

}

void actualizar_peticiones(int tipo) {
	if (tipo == ESCRITORI) {
		sem_wait(&sem_pet_escritorI);
		pet_escritorI[id_nodo]++;
		sem_post(&sem_pet_escritorI);
	} else if (tipo == ESCRITORII) {
		sem_wait(&sem_pet_escritorII);
		pet_escritorII[id_nodo]++;
		sem_post(&sem_pet_escritorII);
	} else if (tipo == LECTOR) {
		sem_wait(&sem_pet_lectores);
		pet_lectores[id_nodo]++;
		sem_post(&sem_pet_lectores);
	}
}

int calcular_nodo_sig() {
	int id_nodo_sig;
	id_nodo_sig = comp_pet_escritorI();
	if (id_nodo_sig != -1)
		return id_nodo_sig;
	id_nodo_sig = comp_pet_escritorII();
	if (id_nodo_sig != -1)
		return id_nodo_sig;
	id_nodo_sig = comp_pet_lectores();
	if (id_nodo_sig != -1)
		return id_nodo_sig;

	return -1;
}

void solicitud_testigo(int tipo) {
	actualizar_peticiones(tipo);
	printf("Solicitando testigo para tipo %i\n", tipo);
	int i;
	int queue_id;
	struct msgbuf mensaje;
	mensaje.mtype = REQUEST;
	mensaje.id_nodo = id_nodo;
	mensaje.tipo_proceso = tipo;
	//printf("antes del for \n");
	for (i = 0; i < 3; i++) {
		if (i != id_nodo) {
			printf("calculando queue_id \n");
			queue_id = get_msgq("/bin/mv", i, 0777 || IPC_CREAT);
			printf("queue_id %i\n", queue_id);
			s_msgsnd(queue_id, (struct msgbuf*) &mensaje,
					sizeof(struct msgbuf) - sizeof(long), 0);
			printf("i = %i", i);
			//msgsnd(msgid, msgp, msgsz, msgflg); // Modificar con los valores correctos
		}
	}
	switch (tipo) {
	case 1:
		sem_wait(&paso_sol_escritorI);
		break;
	case 2:
		sem_wait(&paso_sol_escritorII);
		break;
	case 3:
		sem_wait(&paso_sol_lector);
		break;
	}
	return;
}

void solicitud_especial() {

	int i, queue_id;
	struct msgbuf mensaje;
	sem_wait(&SemEscritoresI);
	if (escritoresI) {
		actualizar_peticiones(ESCRITORI);
		sem_post(&SemEscritoresI);
		mensaje.mtype = REQUEST;
		mensaje.id_nodo = id_nodo;
		mensaje.tipo_proceso = ESCRITORI;
		for (i = 0; i < 3; i++) {
			if (id_nodo != i) {
				queue_id = get_msgq("/bin/mv", i, 0777 || IPC_CREAT);
				s_msgsnd(queue_id, (struct msgbuf*) &mensaje,
						sizeof(struct msgbuf) - sizeof(long), 0);
			}
		}
	} else {
		sem_post(&SemEscritoresI);

	}
	sem_wait(&SemEscritoresII);
	if (escritoresII) {
		actualizar_peticiones(ESCRITORII);
		sem_post(&SemEscritoresII);
		mensaje.mtype = REQUEST;
		mensaje.id_nodo = id_nodo;
		mensaje.tipo_proceso = ESCRITORII;
		for (i = 0; i < 3; i++) {
			if (id_nodo != i) {
				queue_id = get_msgq("/bin/mv", i, 0777 || IPC_CREAT);
				s_msgsnd(queue_id, (struct msgbuf*) &mensaje,
						sizeof(struct msgbuf) - sizeof(long), 0);
			}
		}
	} else {
		sem_post(&SemEscritoresII);

	}
	sem_wait(&SemLectoresVar);
	if (lectores) {
		actualizar_peticiones(LECTOR);
		sem_post(&SemLectoresVar);
		mensaje.mtype = REQUEST;
		mensaje.id_nodo = id_nodo;
		mensaje.tipo_proceso = LECTOR;
		for (i = 0; i < 3; i++) {
			if (id_nodo != i) {
				queue_id = get_msgq("/bin/mv", i, 0777 || IPC_CREAT);
				s_msgsnd(queue_id, (struct msgbuf*) &mensaje,
						sizeof(struct msgbuf) - sizeof(long), 0);
			}
		}
	} else {
		sem_post(&SemLectoresVar);
	}
	return;
}

void adel() {
	sem_wait(&adelantamientos);
	sem_wait(&SemEscritoresI);
	sem_wait(&SemEscritoresII);
	if (comp_pet_escritorI() >= 0 || comp_pet_escritorII() >= 0 || escritoresI
			|| escritoresII) {
		adelantamiento++;
	}
	sem_post(&SemEscritoresI);
	sem_post(&SemEscritoresII);

	if (adelantamiento >= max) {

		sem_post(&adelantamientos);
		sem_wait(&semNumeroMaximo);
		sem_wait(&adelantamientos);
		adelantamiento = 0;
		sem_post(&adelantamientos);
		sem_post(&sem_lectores);

	} else {
		sem_post(&adelantamientos);
	}

}

void finLectores() {
	printf("[Nodo %d] Realizando funcion fin_lectores.\n", id_nodo);
	int i;
	struct msgbuf mensaje, mensaje_fin_lectores;
	int queue_id;
	sem_wait(&SemTestigo);
	sem_wait(&semLectoresSC);
	if (!testigo && lectoresSC == 0) {
		sem_post(&SemTestigo);
		sem_post(&semLectoresSC);
		printf("[Nodo %d] Signal fin_lectores\n", id_nodo);
		sem_post(&fin_lectores);
	} else {

		sem_post(&SemTestigo);
		sem_post(&semLectoresSC);
		mensaje.mtype = PASO_ESCRITOR;
		mensaje.id_nodo = id_nodo;
		printf("[Nodo %d] Envio un mensaje a todos los nodos preguntandoles si han terminado sus lectores.\n", id_nodo);
		for (i = 0; i < 3; i++) {
			if (i != id_nodo) {
				queue_id = get_msgq("/bin/mv", i, 0777 || IPC_CREAT);
				s_msgsnd(queue_id, (struct msgbuf*) &mensaje,
						sizeof(struct msgbuf), 0);
				// send(PASO_ESCRITOR, id_nodo[i]);
			}
		}
		printf("[Nodo %d] Me quedo esperando a recibir un mensaje de todos los nodos.\n", id_nodo);
		queue_id = get_msgq("/bin/mv", id_nodo, 0777 | IPC_CREAT);
		for (i = 0; i < 3; i++) {
			if (i != id_nodo) {
				s_msgrcv(queue_id, (struct msgbuf *) &mensaje_fin_lectores,
						sizeof(struct msgbuf), FIN_LECTORES_NODO,
						0);
				printf("[Nodo %d] He recibido el mensaje de que han terminado los lectores en el nodo %d\n", id_nodo, mensaje_fin_lectores.id_nodo);
			}

		}
		actualizar_atendidas_lector();
		prioridad_lectores = 0;
	}
}

void actualizar_atendidas(int tipo) {
	if (tipo == ESCRITORI) {
		sem_wait(&sem_pet_escritorI);
		if (pet_escritorI[id_nodo] > at_escritorI[id_nodo]) {
			at_escritorI[id_nodo]++;
		}
		sem_post(&sem_pet_escritorI);
	} else if (tipo == ESCRITORII) {
		sem_wait(&sem_pet_escritorII);
		if (pet_escritorII[id_nodo] > at_escritorII[id_nodo]) {
			at_escritorII[id_nodo]++;
		}
		sem_post(&sem_pet_escritorII);
	} else if (tipo == LECTOR) {
		sem_wait(&sem_pet_lectores);
		if (pet_lectores[id_nodo] > at_lectores[id_nodo]) {
			at_lectores[id_nodo]++;
		}
		sem_post(&sem_pet_lectores);
	}
}



void actualizar_atendidas_lector() {
		sem_wait(&sem_pet_lectores);
		int i;
		for (i=0;i<3;i++){
		if (pet_lectores[i] > at_lectores[i]) {
			at_lectores[i]=pet_lectores[i];
		}
		}
		sem_post(&sem_pet_lectores);
	
}

