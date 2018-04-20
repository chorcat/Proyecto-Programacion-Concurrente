#include "proyecto.h"

void* Lector(void* arg) {
	int tipo = *(int*) arg;

	printf("Nodo [%i] Lector iniciandose \n", id_nodo);
	fflush(stdout);
	sem_wait(&SemTestigo);

	// NO TIENES EL TESTIGO
	if (!testigo) {
		sem_post(&SemTestigo);
		//printf("Lector iniciandose 2 ---------- sin testigo \n", id_nodo);
		fflush(stdout);
		sem_wait(&SemLectoresVar);
		//printf("Lector iniciandose 3 \n", id_nodo);
		fflush(stdout);

		// SI YA SOLICITARON EL TESTIGO
		if (lectores) {
			printf("Oh vaya no soy el primer lector \n ");
			fflush(stdout);
			lectores++;
			sem_post(&SemLectoresVar);
			solicitud_testigo(3);//  ¿Se deberia solicitar aqui?
			sem_wait(&sem_lectores);
			adel();
			sem_post(&sem_lectores);
		}

		// PRIMER LECTOR EN SOLICITAR EL TESTIGO
		else {
			printf("Soy el primer lector :) \n");
			fflush(stdout);
			lectores++;
			sem_post(&SemLectoresVar);
			//sem_wait(&sem_lectores); // No hay que cerrar el semaforo aqui  No descomentar
			solicitud_testigo(LECTOR);
			//printf("Recibi el testigo (primero)\n");
			fflush(stdout);
			sem_wait(&SemEscritoresI);
			sem_wait(&SemEscritoresII);
			//printf("Pase los semaforos variables \n");
			fflush(stdout);
			if ((escritoresI || escritoresII)&&prioridad_lectores==0) {
				sem_post(&SemEscritoresI);
				sem_post(&SemEscritoresII);
				//printf("me quedo en sem_lectores \n");
				sem_wait(&sem_lectores2);
			} else {
				//printf("Recibi el testigo 2\n");
				fflush(stdout);
				sem_post(&SemEscritoresI);
				sem_post(&SemEscritoresII);
				if(!(escritoresI&&prioridad_lectores) ){
					//printf("Me bloqueo aqui \n");
				sem_wait(&EscritoresMutex1);

				}
				if(!(escritoresII&&prioridad_lectores&&!escritoresI)){
				
				sem_wait(&EscritoresMutex2);
				}
				adel();
				//printf("Paso los mutex escritores\n");
				fflush(stdout);
			}

			sem_post(&sem_lectores);
		}
		//printf("Antes de semLEctoresSC\n");
		fflush(stdout);
		sem_wait(&semLectoresSC);
		lectoresSC++;
		sem_post(&semLectoresSC);
		//printf("Pase semLectoresSC\n");
		fflush(stdout);
		if (tipo == 1) {
			printf("\nNodo[%i] Proceso de gradas en SC", id_nodo);
			fflush(stdout);
			sem_wait(&finalizar_grada);
		} else {
			printf("\nNodo[%i] Proceso de evento en SC", id_nodo);
			fflush(stdout);
			sem_wait(&finalizar_evento);

		}
		sem_wait(&semLectoresSC);
		lectoresSC--;
		sem_post(&semLectoresSC);

		sem_wait(&SemLectoresVar);
		lectores--;
		sem_post(&SemLectoresVar);
	} else {
		//Si no tengo el testigo lo mismo pero sin solicitarlo 
		sem_post(&SemTestigo);
		sem_wait(&SemLectoresVar);
		printf("Lector iniciandose con testigo\n");
		fflush(stdout);
		if (lectores) {
			lectores++;
			sem_post(&SemLectoresVar);
			sem_wait(&sem_lectores);

			adel();
			sem_post(&sem_lectores);
		} else {
			printf("Oh vaya soy el primer lector \n", id_nodo);
			fflush(stdout);
			lectores++;
			sem_post(&SemLectoresVar);
			//  sem_wait(&sem_lectores);  // No hay que cerrar el semaforo aqui 
			sem_wait(&SemEscritoresI);
			sem_wait(&SemEscritoresII);
			//printf("paso los sem escritores \n");
				fflush(stdout);
			if (escritoresI || escritoresII) {
				sem_post(&SemEscritoresI);
				sem_post(&SemEscritoresII);
				//printf("me quedo en sem lec 2 1 \n");
				fflush(stdout);
				sem_wait(&sem_lectores2);
			} else {

				sem_post(&SemEscritoresI);
				sem_post(&SemEscritoresII);
				//printf("voy a mirar adel  \n");
				fflush(stdout);
				adel();
				//printf("me quedo en mutex 1 \n");
				fflush(stdout);
				sem_wait(&EscritoresMutex1);
				//printf("me quedo en mutex 2 \n");
				fflush(stdout);
				sem_wait(&EscritoresMutex2);
			}

			sem_post(&sem_lectores);
		}
		sem_wait(&semLectoresSC);
		lectoresSC++;
		sem_post(&semLectoresSC);
		if (tipo == 1) {
			printf("\nNodo[%i] Proceso de gradas en SC", id_nodo);
			fflush(stdout);
			sem_wait(&finalizar_grada);
		} else {
			printf("\nNodo[%i] Proceso de evento en SC", id_nodo);
			fflush(stdout);
			sem_wait(&finalizar_evento);

		}
		sem_wait(&semLectoresSC);
		lectoresSC--;
		sem_post(&semLectoresSC);

		sem_wait(&SemLectoresVar);
		lectores--;
		sem_post(&SemLectoresVar);

	}

	// SALIDA LECTORES

	sem_wait(&SemLectoresVar);
	sem_wait(&adelantamientos);
	if (!lectores && testigo) {

		sem_wait(&sem_lectores); // Ya cierra el primero  el semaforo¿? No , si hay que cerrar el semaforo por numero maximo de adelantamientos 
		sem_post(&SemLectoresVar);
		sem_post(&adelantamientos);
		//sem_post(&EscritoresMutex1);
		//sem_post(&EscritoresMutex2);  // ojo aqui con estos post

		if (escritoresI != 0) {
			finLectores();
			sem_post(&EscritoresMutex1);
			if(!escritoresII){
				sem_post(&EscritoresMutex2);
			}
		}
		else{
			if (comp_pet_escritorI() >= 0) {
				//printf("estoy debajo de comp_pet_escritor()\n");
				fflush(stdout);
				finLectores();
				enviarTestigo(calcular_nodo_sig());
				//printf("Envio el testigo \n", id_nodo);
				fflush(stdout);
				sem_post(&EscritoresMutex1);
				sem_post(&EscritoresMutex2);
			}
			else{
				if (escritoresII != 0) {
					finLectores();
					sem_post(&EscritoresMutex2);
					if(!escritoresI){
					sem_post(&EscritoresMutex1);
					}
				}
				else{
					if (comp_pet_escritorII() >= 0) {
						finLectores();
						enviarTestigo(calcular_nodo_sig());
						//printf("Envio testigo---------------------\n");
						fflush(stdout);
						sem_post(&EscritoresMutex1);
						sem_post(&EscritoresMutex2);
					}
					else {
						actualizar_atendidas(LECTOR);
						sem_post(&EscritoresMutex1);
						sem_post(&EscritoresMutex2);
					}
				}
			}
		}
	} else {
		sem_post(&SemLectoresVar);
		sem_post(&adelantamientos);
	}
	sem_wait(&SemLectoresVar);
	sem_wait(&semLectoresSC);
	if (!lectoresSC && lectores&& testigo) { //Aqui comprobar que hay escritores en otro nodo
		printf("Se acabaron los lectores SC \n");
		if (escritoresI) {
			sem_post(&semLectoresSC);
			finLectores();
			sem_post(&EscritoresMutex1); // No hace falta proteger esta variable (escritoresI ) porque este fragmento ya se ejucta en EM.
			sem_wait(&semLectoresSC);
			if(!escritoresII){
					sem_post(&EscritoresMutex2);
				}
		}

		else{
			if(comp_pet_escritorI()){

				sem_post(&SemLectoresVar);
				//printf("envio el testigo 1+++++++ \n");
				sem_post(&semLectoresSC);
				finLectores();
				enviarTestigo(calcular_nodo_sig());
				sem_wait(&semLectoresSC);
				sem_wait(&SemLectoresVar);
				//printf("envio el testigo 2\n");
			}
			else{
				if(escritoresII) {
					sem_post(&semLectoresSC);
					finLectores();
					sem_post(&EscritoresMutex2);
					if(!escritoresI){
					sem_post(&EscritoresMutex1);
						}
					sem_wait(&semLectoresSC);
				}
				else{
					if(comp_pet_escritorII()){
						sem_post(&SemLectoresVar);
						sem_post(&semLectoresSC);
						finLectores();
						enviarTestigo(calcular_nodo_sig());
						sem_wait(&semLectoresSC);
						sem_wait(&SemLectoresVar);
					}
				}
			}
		}
	}
	if(!lectoresSC &&  !testigo){
		if(!escritoresI &&!escritoresII){
			sem_post(&EscritoresMutex1);
			sem_post(&EscritoresMutex2);
			fflush(stdout);
		}
		if(escritoresI&&!escritoresII){
			sem_post(&EscritoresMutex2);
		}
		if(!escritoresI&&escritoresII){
			sem_post(&EscritoresMutex1);
		}
		if(escritoresI&&escritoresII){
			sem_post(&EscritoresMutex1);
		}
	}

	if(!lectoresSC&&!testigo){
		sem_post(&semLectoresSC);
		sem_post(&SemLectoresVar);
		finLectores();        //Ojo aqui el semaforo puede quedar abierto
	}
	else{
		sem_post(&SemLectoresVar);
		sem_post(&semLectoresSC);
	}



	printf("\nNodo[%i] Proceso de lectores saliendo ", id_nodo);
	fflush(stdout);

}
