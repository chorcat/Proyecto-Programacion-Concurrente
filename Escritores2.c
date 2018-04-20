#include "proyecto.h"
void * EscritorII(void *arg) {
	//int testigo=1, escritoresII=0, escritoresI=0, escritoresIIEsperando, escritoresIIEsperandoConTestigo=0;
	//sem_t EscritoresMutex1, EscritoresMutex2, paso_esc2, SemTestigo, SemLectoresVar, SemEscritoresI, SemEscritoresII, SemEscritoresIIEsperando;
	int tipo = *(int*) arg;

	escritoresII++;

	sem_wait(&SemTestigo);
	if (testigo == 0) {
		sem_post(&SemTestigo);
		//printf("No tengo el testigo\n");
		sem_wait(&EscritoresMutex2);
		//printf("Despues del mutex2\n");
		sem_wait(&SemEscritoresII);
		if (escritoresII == 1 && !testigo) {
			sem_post(&SemEscritoresII);
			solicitud_testigo(2);
			sem_wait(&SemEscritoresI);
			if (escritoresI != 0) {
				sem_post(&SemEscritoresI);
				escritoresIIEsperando++;
				sem_wait(&paso_esc2);
				escritoresIIEsperando--;
				sem_wait(&EscritoresMutex1);
				if (tipo == 1) {
					printf("\nNodo[%i] Proceso de anulaciones en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_anulacion);
					printf(
							"\nNodo[%i] Proceso de anulaciones saliendo de la SC",
							id_nodo);
					fflush(stdout);

				} else {
					printf("\nNodo[%i] Proceso de prerreserva en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_prerreserva);
					printf(
							"\nNodo[%i] Proceso de prerreserva saliendo de la SC",
							id_nodo);
					fflush(stdout);

				}
				//SC
				escritoresII--;
			} else {
				sem_post(&SemEscritoresI);
				if(prioridad_lectores==1){
					escritoresIIEsperandoConTestigo++;
					sem_wait(&EscritoresMutex2);
					escritoresIIEsperandoConTestigo--;
				}

				sem_wait(&EscritoresMutex1);
				if (tipo == 1) {
					printf("\nNodo[%i] Proceso de anulaciones en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_anulacion);
					printf(
							"\nNodo[%i] Proceso de anulaciones saliendo de la SC",
							id_nodo);
					fflush(stdout);

				} else {
					printf("\nNodo[%i] Proceso de prerreserva en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_prerreserva);

					printf(
							"\nNodo[%i] Proceso de prerreserva saliendo de la SC",
							id_nodo);
					fflush(stdout);

				}

				//SC
				escritoresII--;
			}
		} else {

			sem_post(&SemEscritoresII);
			sem_wait(&SemEscritoresI);
			if (escritoresI != 0) {
				sem_post(&SemEscritoresI);
				escritoresIIEsperando++;
				sem_wait(&paso_esc2);
				escritoresIIEsperando--;
				sem_wait(&EscritoresMutex1);
				if (tipo == 1) {
					printf("\nNodo[%i] Proceso de anulaciones en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_anulacion);

					printf(
							"\nNodo[%i] Proceso de anulaciones saliendo de la SC",
							id_nodo);
					fflush(stdout);

				} else {
					printf("\nNodo[%i] Proceso de prerreserva en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_prerreserva);

					printf(
							"\nNodo[%i] Proceso de prerreserva saliendo de la SC",
							id_nodo);
					fflush(stdout);

				}

				//SC
				escritoresII--;
			} else {



				sem_post(&SemEscritoresI);
				printf("No hay escritoresI \n");
				sem_wait(&EscritoresMutex1);
				if (tipo == 1) {
					printf("\nNodo[%i] Proceso de anulaciones en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_anulacion);

					printf(
							"\nNodo[%i] Proceso de anulaciones saliendo de la SC",
							id_nodo);
					fflush(stdout);

				} else {
					printf("\nNodo[%i] Proceso de prerreserva en SC", id_nodo);
					fflush(stdout);
					sem_wait(&finalizar_prerreserva);

					printf(
							"\nNodo[%i] Proceso de prerreserva saliendo de la SC",
							id_nodo);
					fflush(stdout);

				}
				escritoresII--;
			}

		}

	} else {

		sem_post(&SemTestigo);
		escritoresIIEsperandoConTestigo++;
		sem_wait(&SemEscritoresII);
		if (escritoresII == 1) {
			sem_post(&SemEscritoresII);
			int i;
			sem_wait(&SemEscritoresI);

			if (escritoresI > 0) {
				sem_post(&SemEscritoresI);

				i = escritoresI;

			} else
				sem_post(&SemEscritoresI);

			//printf("wait a mutex2\n");
			sem_wait(&EscritoresMutex2);

			escritoresIIEsperandoConTestigo--;
			sem_wait(&SemEscritoresI);
			if (escritoresI != 0) {

				sem_post(&SemEscritoresI);
				escritoresIIEsperando++;
				sem_wait(&paso_esc2);
				escritoresIIEsperando--;
				sem_post(&SemEscritoresI);
				sem_wait(&EscritoresMutex1);
			} else {
				sem_post(&SemEscritoresI);
				if (i == 0)
					sem_wait(&EscritoresMutex1);

			}
			if (tipo == 1) {

				printf("\nNodo[%i] Proceso de anulaciones en SC", id_nodo);
				fflush(stdout);
				sem_wait(&finalizar_anulacion);

				printf("\nNodo[%i] Proceso de anulaciones saliendo de la SC",
						id_nodo);
				fflush(stdout);

			} else {
				printf("\nNodo[%i] Proceso de prerreserva en SC", id_nodo);
				fflush(stdout);
				sem_wait(&finalizar_prerreserva);

				printf("\nNodo[%i] Proceso de prerreserva saliendo de la SC",
						id_nodo);
				fflush(stdout);
			}
			//SC
			escritoresII--;

		} else {
			sem_post(&SemEscritoresII);
			//printf("wait mutex2\n");
			escritoresIIEsperandoConTestigo++;
			sem_wait(&EscritoresMutex2);
			escritoresIIEsperandoConTestigo--;
			sem_wait(&SemEscritoresI);
			if (escritoresI != 0) {

				sem_post(&SemEscritoresI);
				escritoresIIEsperando++;
				sem_wait(&paso_esc2);
				escritoresIIEsperando--;
				sem_post(&SemEscritoresI);
				sem_wait(&EscritoresMutex1);
			} else {
				sem_post(&SemEscritoresI);
			}

			if (tipo == 1) {
				printf("\nNodo[%i] Proceso de anulaciones en SC", id_nodo);
				fflush(stdout);
				sem_wait(&finalizar_anulacion);
				printf("\nNodo[%i] Proceso de anulaciones saliendo de la SC",
						id_nodo);
				fflush(stdout);
			} else {
				printf("\nNodo[%i] Proceso de prerreserva en SC", id_nodo);
				fflush(stdout);
				sem_wait(&finalizar_prerreserva);
				printf("\nNodo[%i] Proceso de prerreserva saliendo de la SC",
						id_nodo);
				fflush(stdout);

			}
			//SC
			escritoresII--;
		}
	}
	actualizar_atendidas(ESCRITORII);
	sem_wait(&SemEscritoresI);
	if (escritoresI != 0) {
		sem_post(&SemEscritoresI);
		sem_post(&EscritoresMutex1);
	} else {
		sem_post(&SemEscritoresI);
		if (comp_pet_escritorI() >= 0) {
			enviarTestigo(calcular_nodo_sig());
			sem_post(&EscritoresMutex1);
			sem_wait(&SemEscritoresII);
			if(escritoresII==0){
				sem_post(&SemEscritoresII);
				sem_post(&EscritoresMutex2);
			}else{
				sem_post(&SemEscritoresII);
			}

		} else {
			sem_wait(&SemEscritoresII);
			if (escritoresII != 0) {
				sem_post(&SemEscritoresII);
				sem_post(&EscritoresMutex1);
				printf("Despertando a otro escritorII del mismo nodo\n");
				sem_post(&EscritoresMutex2);
			} else {
				sem_post(&SemEscritoresII);
				if (comp_pet_escritorII() >= 0) {
					printf("Hay peticiones de escritoresII\n");
					enviarTestigo(calcular_nodo_sig());
					if(escritoresII==0){
						//printf("No hay escritores 2 -> post mutex2");
						sem_post(&EscritoresMutex2);

						sem_post(&EscritoresMutex1);
					}else{
						sem_post(&EscritoresMutex1);
					}

				} else {
					sem_wait(&adelantamientos);

					if (adelantamiento >= max ) {
						if (lectores>0){
							sem_post(&semNumeroMaximo);
							sem_post(&adelantamientos);
						}  else{
							enviarTestigo(calcular_nodo_sig());
							sem_post(&adelantamientos);
							sem_post(&EscritoresMutex1);
							sem_post(&EscritoresMutex2);
						}

					} else {
						sem_post(&adelantamientos);
						sem_wait(&SemLectoresVar);
						if (lectores != 0) {
							sem_post(&SemLectoresVar);
							sem_post(&sem_lectores2);
						} else {
							sem_post(&SemLectoresVar);
							if (comp_pet_lectores() >= 0) {
								enviarTestigo(calcular_nodo_sig());
								sem_post(&EscritoresMutex1);
								sem_post(&EscritoresMutex2);

							} else {
								//printf("\npost mutex1\n");
								sem_post(&EscritoresMutex1);
								//printf("post mutex2\n");
								sem_post(&EscritoresMutex2);

							}
						}
					}
				}
			}

		}

	}
}
