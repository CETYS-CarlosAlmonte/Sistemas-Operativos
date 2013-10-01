#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

static char items[10];
pthread_t threads[20];
int production_count = 0;
sem_t producing_sem;
sem_t consuming_sem;

void produce(int*);
void consume(int*);

int main(int argc, char **argv)
{

	if (sem_init(&producing_sem, 0, 1) < 0 )
	{
		printf("Failed to initialize 'Producing Items semaphore'\n");
		exit(0);
	}
	if (sem_init(&consuming_sem, 0, 0) < 0)
	{
		printf("Failed to initialize 'Consuming Items semaphore'\n");
		exit(0);
	}

	int items_indexes[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int i;
	for (i = 0; i < 10; i++)
		pthread_create(&threads[i], NULL, (void*) produce, (void *) &items_indexes[i]);

	for (i = 10; i < 20; i++)
		pthread_create(&threads[i], NULL, (void*) consume, (void*) &items_indexes[i-10]);

	for (i = 0; i < 20; i++)
		pthread_join(threads[i], NULL);

	sem_destroy(&producing_sem);
	sem_destroy(&consuming_sem);
	printf("ALL PROCESSES ENDED\n");

	return 0;
}

void produce(int *value)
{
	printf("Producer thread #%d waiting...\n", *value+1);
	sem_wait(&producing_sem);
	if (!items[*value])
		items[*value] = 1;
	sem_post(&producing_sem);
	printf("Producer thread #%d done!\n", *value+1);
	sem_post(&consuming_sem);
}

void consume(int *value)
{
	printf("Consumer thread #%d waiting...\n", *value+1);
	sem_wait(&consuming_sem);
	while (!(items[*value])) { }
	items[*value] = 0;
	printf("Consumer thread #%d done!\n", *value+1);
}
