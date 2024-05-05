#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include "sha1.h"

#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define BYTE_RANGE 256
#define MAX_STR_LEN 50
#define TRUE 1
#define FALSE 0

//Defines the struct used for local variables
struct sThreadArguments {
	int count[BYTE_RANGE];
	unsigned char buffer[BUFFER_SIZE];
	pthread_mutex_t mutex;
	sem_t semaphore_full, semaphore_empty;
	int bytes_in_buffer;
	char *pFilename;
	int iDoneReading;
};


void* thread_A(void* sThreadArguments) {
	struct sThreadArguments *sArgs = (struct sThreadArguments *)sThreadArguments;
	
	//Reads the file provided as a parameter for the user, else prints error
	FILE* fp = fopen(sArgs->pFilename, "rb");
	if (!fp) {
		printf(sArgs->pFilename);
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}

	while (1) {
		//Waits until the buffer has available space
		while (sArgs->bytes_in_buffer > 0) {
			sem_wait(&sArgs->semaphore_empty);
		}
		
		//Locks the shared memory
		pthread_mutex_lock(&sArgs->mutex);

		int read_bytes = fread(sArgs->buffer + sArgs->bytes_in_buffer, 1, BUFFER_SIZE - sArgs->bytes_in_buffer, fp);
		sArgs->bytes_in_buffer += read_bytes;
		
		if (read_bytes < BUFFER_SIZE - sArgs->bytes_in_buffer) {
			pthread_mutex_unlock(&sArgs->mutex);
			sArgs->iDoneReading = TRUE;
			sem_post(&sArgs->semaphore_full);
			break;
		}
		
		//Unlocks the shared memory
		pthread_mutex_unlock(&sArgs->mutex);
		
		//Signals the next thread to run
		sem_post(&sArgs->semaphore_full);
	}
	fclose(fp);
	pthread_exit(NULL);
}

void* thread_B(void* sThreadArguments) {
	struct sThreadArguments *sArgs = (struct sThreadArguments *)sThreadArguments;
	memset(sArgs->count, 0, sizeof(sArgs->count));
	
	//Declare sha1 context variable for use in the other sha1 functions
	SHA1_CTX sha1_context;

	//Initialize sha1_context
	SHA1Init(&sha1_context);

	while (1) {
		//Waits until buffer is filled
		while (sArgs->bytes_in_buffer == 0) {
			sem_wait(&sArgs->semaphore_full);
		}
		
		//Locks the shared memory
		pthread_mutex_lock(&sArgs->mutex);
		
		//
		if(sArgs->bytes_in_buffer == -1) {
			pthread_mutex_unlock(&sArgs->mutex);
			break;
		}
		
		SHA1Update(&sha1_context, sArgs->buffer, sArgs->bytes_in_buffer);

		for (int i = 0; i < sArgs->bytes_in_buffer; i++) {
			sArgs->count[sArgs->buffer[i]]++;
		}

		sArgs->bytes_in_buffer = 0;
		
		//Unlocks the shared memory
		pthread_mutex_unlock(&sArgs->mutex);
		
		//Signals the first thread to run
		sem_post(&sArgs->semaphore_empty);
		
		if (sArgs->bytes_in_buffer == 0 && sArgs->iDoneReading == TRUE) {
			break;
		}
	}
	//Initialize the the context to obtain hash digest
	unsigned char sha1_digest[20];
	SHA1Final(sha1_digest, &sha1_context);
	
	//Prints out byte valus
	/*for (int i = 0; i < BYTE_RANGE; i++) {
		printf("%d: %d\n", i, sArgs->count[i]);
	}*/
	
	//Prints out hex values from sha1 hash, i < buffer in sha1_digest[20]
	for(int i = 0; i < 20; i++) {
		printf("%02x", sha1_digest[i]);
	}
	printf("\n");

	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	//Exits the program if the user doesn't put in exactly 1 parameter when running the program
	if(argc != 2) {
		printf("./eksamen_v24_oppgave4 <filename>\n");
		exit(1);
	}
	
	//Allocates memory to the struct sThreadArguments
	struct sThreadArguments *sThreadArguments = malloc(sizeof(struct sThreadArguments));
	if(sThreadArguments == NULL) {
		perror("Error: Failed to allocate memory\n");
		exit(1);
	}
	
	//Allocates memory to pFilename
	sThreadArguments->pFilename = malloc(MAX_STR_LEN);
	if(sThreadArguments->pFilename == NULL) {
		perror("Error: Failed to allocate memory\n");
		exit(1);
	}
	
	//Initialises variables
	pthread_t threadA, threadB;
	sThreadArguments->bytes_in_buffer = 0;
	sThreadArguments->iDoneReading = FALSE;
	
	//Initialises mutex with default attributes(NULL)
	pthread_mutex_init(&sThreadArguments->mutex, NULL);
	
	//Initialises semaphores
	sem_init(&sThreadArguments->semaphore_full, 0, 0);
	sem_init(&sThreadArguments->semaphore_empty, 0, 1);
	
	//Copies the filename from arg[1] which is the filename, to pFilename with MAX_STR_LEN - 1 to prevent buffer overflow and ensure null-termination
	strncpy(sThreadArguments->pFilename, argv[1], MAX_STR_LEN - 1);
	sThreadArguments->pFilename[MAX_STR_LEN - 1] = '\0';

	if (pthread_create(&threadA, NULL, thread_A, (void*)sThreadArguments) != 0) {
		perror("Could not create thread A");
		exit(1);
	}

	if (pthread_create(&threadB, NULL, thread_B, (void*)sThreadArguments) != 0) {
		perror("Could not create thread B");
		exit(1);
	}

	if (pthread_join(threadA, NULL) != 0) {
		perror("Could not join thread A");
		exit(1);
	}
	
	if (pthread_join(threadB, NULL) != 0) {
		perror("Could not join thread B");
		exit(1);
	}
	
	//Destroys mutexes and semaphores
	pthread_mutex_destroy(&sThreadArguments->mutex);
	sem_destroy(&sThreadArguments->semaphore_full);
	sem_destroy(&sThreadArguments->semaphore_empty);
	
	//Frees allocated memory
	free(sThreadArguments->pFilename);
	free(sThreadArguments);

	return 0;
}
