#include "proyecto.h"

void *EscritorI(void *arg) {
	int i;
	sem_wait(&SemEscritoresI);
	escritoresI++;
	sem_post(&SemEscritoresI);
	sem_wait(&SemTestigo);
	if (!testigo) {

		sem_post(&SemTestigo);
		sem_wait(&EscritoresMutex1);
		sem_wait(&SemEscritoresI);

		if (escritoresI == 1 && !testigo) {
			sem_post(&SemEscritoresI);
			solicitud_testigo(1);
		
		      if(prioridad_lectores==1){
			escritoresIEsperandoConTestigo++;

			sem_wait(&EscritoresMutex1);
			escritoresIEsperandoConTestigo--;
		      }
			sem_wait(&SemEscritoresII);
			if (escritoresII == 0) {
			  	
				sem_post(&SemEscritoresII);
				sem_wait(&EscritoresMutex2);
				
			} else {
				sem_post(&SemEscritoresII);
			}

		} else {
			sem_post(&SemEscritoresI);
		}
		printf("\nNodo[%i] Proceso pagando", id_nodo);
		fflush(stdout);

		sem_wait(&finalizar_pago);
		printf("\nNodo[%i] Finalizando proceso de pagos", id_nodo);
		fflush(stdout);
		sem_wait(&SemEscritoresI);
		escritoresI--;
		sem_post(&SemEscritoresI);
	} else {

		sem_post(&SemTestigo);
		sem_wait(&SemEscritoresI);
		escritoresIEsperandoConTestigo++;
		if (escritoresI == 1) {

			sem_post(&SemEscritoresI);
			sem_wait(&SemEscritoresII);

			if (escritoresII == 0 && lectores == 0) {
				sem_post(&SemEscritoresII);

				sem_wait(&EscritoresMutex1);
				sem_wait(&EscritoresMutex2);
			} else {
				sem_post(&SemEscritoresII);

				sem_wait(&EscritoresMutex1);

			}

		} else {
			sem_post(&SemEscritoresI);

			sem_wait(&EscritoresMutex1);
		}
		escritoresIEsperandoConTestigo--;
		printf("\nNodo[%i] Proceso pagando", id_nodo);
		fflush(stdout);

		sem_wait(&finalizar_pago);
		printf("\nNodo[%i] Finalizando proceso de pagos", id_nodo);
		fflush(stdout);
		sem_wait(&SemEscritoresI);
		escritoresI--;
		sem_post(&SemEscritoresI);
	}
	actualizar_atendidas(ESCRITORI);
	sem_wait(&SemEscritoresI);
 
								  fflush(stdout);
	if (escritoresI > 0) {
		sem_post(&SemEscritoresI);
		sem_post(&EscritoresMutex1);
	} else {

		if (comp_pet_escritorI() >= 0) {
			sem_post(&SemEscritoresI);
			enviarTestigo(calcular_nodo_sig());
			sem_post(&EscritoresMutex1);
			if (escritoresII == 0)
				sem_post(&EscritoresMutex2);
		} else {

			sem_post(&SemEscritoresI);
			sem_wait(&SemEscritoresEsperandoII);
			if (escritoresEsperandoII > 0) {
				sem_post(&SemEscritoresEsperandoII);
				sem_post(&paso_esc2);
			} else {

				sem_post(&SemEscritoresEsperandoII);

				sem_wait(&SemEscritoresII);

				if (escritoresII > 0) {
					sem_post(&SemEscritoresII);
					sem_post(&EscritoresMutex2);
				} else {

					sem_post(&SemEscritoresII);

					if (comp_pet_escritorII() >= 0) {
						enviarTestigo(calcular_nodo_sig());
						sem_post(&EscritoresMutex1);
						if (escritoresII == 0)
							sem_post(&EscritoresMutex2);

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
							if (lectores > 0) {
								sem_post(&SemLectoresVar);

								sem_post(&sem_lectores2);
							} else {
								sem_post(&SemLectoresVar);

								if (comp_pet_lectores() >= 0) {
								
									enviarTestigo(calcular_nodo_sig());
									sem_post(&EscritoresMutex1);
									sem_post(&EscritoresMutex2);
								} else {

									sem_post(&EscritoresMutex1);
									sem_post(&EscritoresMutex2);
								}
							}
						}
					}
				}
			}

		}
	}
}
