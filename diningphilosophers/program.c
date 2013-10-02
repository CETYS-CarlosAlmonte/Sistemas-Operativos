#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int size = 5;
pthread_t philosophers[5];
pthread_mutex_t mutex;

static char forks[5]; //forks[i] belongs to philosophers[i]'s RIGHT side. 0: available, 1: unavailable.
static char eating[5];

void run(int*);
int look_for_forks(int*);
void release_forks(int*);

int main(int argc, char **argv)
{
	pthread_mutex_init(&mutex, NULL);

	int philosopher_index[size];
	int i;

	for (i = 0; i < size; i++)
	{
		philosopher_index[i] = i;
		pthread_create(&philosophers[i], NULL, (void*)run, (void*)&philosopher_index[i]);
	}

	for (i = 0; i < size; i++)
		pthread_join(philosophers[i], NULL);
	printf("HELLO\n");

	return 0;
}

void run(int *i)
{
	while (1)
	{
		sleep(1);
		if (eating[*i])
		{
			printf("Philosopher #%d is eating 'yum yum'.\n", *i+1);
			release_forks(i);
			eating[*i] = 0;
			printf("Philosopher #%d released forks.\n", *i+1);
		}
		else
		{
			printf("Philosopher #%d is thinking.\n", *i+1);
			pthread_mutex_lock(&mutex);
			if (eating[*i] = look_for_forks(i))
				printf("Philosopher #%d got his forks.\n", *i+1);
			pthread_mutex_unlock(&mutex);
		}
	}
}

int look_for_forks(int *i)
{
	if (!forks[*i])
	{
		if (*i > 0 && !forks[*i-1])
		{
			forks[*i] = 1;
			forks[*i-1] = 1;
			return 1;
		}
		else if (*i == 0 && !forks[size-1])
		{
			forks[0] = 1;
			forks[size-1] = 1;
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

void release_forks(int *i)
{
	pthread_mutex_lock(&mutex);
	forks[*i] = 0;
	if (*i == 0)
		forks[size-1] = 0;
	else
		forks[*i-1] = 0;
	pthread_mutex_unlock(&mutex);
}
