#include "proyecto.h"

int main(int argc, char *argv[]) {
  max=4;
	if (argc < 2) {
		printf("Llamada incorrecta. Utilice: ./proyecto [id_nodo]\n");
		exit(-1);
	}
	id_nodo = atoi(argv[1]);

	// Inicializar variables globales del nodo
	escritoresI = 0;
	escritoresII = 0;
	lectores = 0;
	lectoresSC = 0;
	if (id_nodo == 0)
		testigo = 1;
	else
		testigo = 0;

	// Inicializar vectores de peticiones y peticiones atendidas del nodo
	int i;
	for (i = 0; i < 3; i++) {
		pet_escritorI[i] = 0;
		pet_escritorII[i] = 0;
		pet_lectores[i] = 0;
		at_escritorI[i] = 0;
		at_escritorII[i] = 0;
		at_lectores[i] = 0;
	}

	// Inicializar semaforos
	sem_init(&SemEscritoresII, 0, 1);
	sem_init(&fin_lectores, 0, 0);
	sem_init(&paso_sol_escritorI, 0, 0);
	sem_init(&paso_sol_escritorII, 0, 0);
	sem_init(&paso_sol_lector, 0, 0);
	sem_init(&SemEscritoresI, 0, 1);
	sem_init(&SemEscritoresEsperandoII, 0, 1);
	sem_init(&SemLectoresVar, 0, 1);
	sem_init(&SemTestigo, 0, 1);
	sem_init(&paso_esc2, 0, 0);
	sem_init(&EscritoresMutex1, 0, 1);
	sem_init(&EscritoresMutex2, 0, 1);
	sem_init(&sem_lectores2, 0, 0);
	sem_init(&sem_pet_escritorI, 0, 1);
	sem_init(&sem_pet_escritorII, 0, 1);
	sem_init(&sem_pet_lectores, 0, 1);
	sem_init(&sem_at_escritorI, 0, 1);
	sem_init(&sem_at_escritorII, 0, 1);
	sem_init(&sem_at_lectores, 0, 1);
	sem_init(&sem_lectores, 0, 0);
	sem_init(&semLectoresSC, 0, 1);
	sem_init(&adelantamientos, 0, 1);
	sem_init(&semNumeroMaximo, 0, 0);

	// Crear el hilo receptor-peticiones
	pthread_create(&thread_receptor, NULL, &Receptor_Peticiones, NULL);

	// Crear el hilo receptor-testigo
	pthread_create(&thread_receptor_token, NULL, &Receptor_Testigo, NULL);

	// Crear el hilo receptor-lectores
	pthread_create(&thread_receptor_lectores, NULL, &Receptor_Lectores, NULL);

	// Inicializar el buzón de mensajes entre nodos


	system("clear");
	int opcion;
	int proceso;
	printf("1.- Empezar pago.\n");
	printf("2.- Empezar prerreserva.\n");
	printf("3.- Empezar anulación.\n");
	printf("4.- Empezar consulta de grada.\n");
	printf("5.- Empezar consulta de evento.\n");
	printf("6.- Finalizar pago.\n");
	printf("7.- Finalizar prerreserva.\n");
	printf("8.- Finalizar anulación.\n");
	printf("9.- Finalizar consulta de grada.\n");
	printf("10.- Finalizar consulta de evento.\n");
	printf("11.- Mostrar información del nodo.\n");
	printf("0. Salir.\n");
	printf("Escoja una opcion: ");
	do {

		scanf("%d", &opcion);

		switch (opcion) {
		case 1:
			pthread_create(&thread_escritoresI, NULL, &EscritorI, NULL);
			break;
		case 2:
			proceso = 0;
			pthread_create(&thread_escritoresII, NULL, &EscritorII, &proceso);
			break;
		case 3:
			proceso = 1;
			pthread_create(&thread_escritoresII, NULL, &EscritorII, &proceso);
			break;
		case 4:
			proceso = 1;
			pthread_create(&thread_lectores, NULL, &Lector, &proceso);
			break;
		case 5:
			proceso = 0;
			pthread_create(&thread_lectores, NULL, &Lector, &proceso);
			break;
		case 6:
			sem_post(&finalizar_pago);
			break;
		case 7:
			sem_post(&finalizar_prerreserva);
			break;
		case 8:
			sem_post(&finalizar_anulacion);
			break;
		case 9:
			sem_post(&finalizar_grada);
			break;
		case 10:
			sem_post(&finalizar_evento);
			break;
		case 11:
			printf("ID: %d\n", id_nodo);
			printf("Testigo: %d\n", testigo);
			printf("Dentro: %d\n", dentro());
			printf("Número de Escritores tipo I: %d\n", escritoresI);
			printf("Número de Escritores tipo II: %d\n", escritoresII);
			printf("Número de lectores: %d\n", lectores);
			printf("Vector de peticiones pendientes de Escritores tipo I:");
			for (i = 0; i < 3; i++)
				printf(" %d", pet_escritorI[i]);
			printf("\nVector de peticiones pendientes de Escritores tipo II:");
			for (i = 0; i < 3; i++)
				printf(" %d", pet_escritorII[i]);
			printf("\nVector de peticiones pendientes de Lectores:");
			for (i = 0; i < 3; i++)
				printf(" %d", pet_lectores[i]);
			printf("\nVector de peticiones atendidas de Escritores tipo I:");
			for (i = 0; i < 3; i++)
				printf(" %d", at_escritorI[i]);
			printf("\nVector de peticiones atendidas de Escritores tipo II:");
			for (i = 0; i < 3; i++)
				printf(" %d", at_escritorII[i]);
			printf("\nVector de peticiones atendidas de Lectores:");
			for (i = 0; i < 3; i++)
				printf(" %d", at_lectores[i]);
		}
	} while (opcion > 0 && opcion < 12);

	return 0;
}
