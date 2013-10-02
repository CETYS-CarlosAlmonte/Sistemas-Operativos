#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

static char items[10];
pthread_t threads[20];
pthread_mutex_t mutex;

void produce(int*);
void consume(int*);

int main(int argc, char **argv)
{
	pthread_mutex_init(&mutex, NULL);

	int items_indexes[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int i;
	for (i = 0; i < 10; i++)
		pthread_create(&threads[i], NULL, (void*) produce, (void *) &items_indexes[i]);

	for (i = 10; i < 20; i++)
		pthread_create(&threads[i], NULL, (void*) consume, (void*) &items_indexes[i-10]);

	for (i = 0; i < 20; i++)
		pthread_join(threads[i], NULL);


	return 0;
}

void produce(int *value)
{
	while (1)
	{
		sleep(1);
		printf("Producer thread #%d waiting...\n", *value+1);
		pthread_mutex_lock(&mutex);
		if  (!items[*value])
		{
			items[*value] = 1;
			printf("Producer thread #%d done!\n", *value+1);
		}
		pthread_mutex_unlock(&mutex);
	}
}

void consume(int *value)
{
	while(1)
	{
		sleep(1);
		printf("Consumer thread #%d waiting...\n", *value+1);
		pthread_mutex_lock(&mutex);
		if (items[*value])
		{
			items[*value] = 0;
			printf("Consumer thread #%d done!\n", *value+1);
		}
		pthread_mutex_unlock(&mutex);
	}
}
