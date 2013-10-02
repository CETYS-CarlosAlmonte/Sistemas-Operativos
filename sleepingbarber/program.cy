#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_t barber;
pthread_t waiting_customers[5];
pthread_mutex_t barberchair;
static char chairs[5];
static int working; //index of the customer's chair being attended. THE ACTUAL CHAIR IS AVAILABLE.

int chairs_available(void); //return number of chairs available
void run_barber(void);
int check_for_customer(void); //returns de index of ther 1st chair that has waiting customer, returns -1 if there's none.
void call_customer(int*); //locks the mutex and empties the chairs[working]
int available_chair(void); //returns the index of the 1st chair available, returns -1 if there's non
void run_customer(void);

int main(int argc, char **argv)
{
	pthread_mutex_init(&barberchair, NULL);
	pthread_create(&barber, NULL, (void*) run_barber, NULL);
	if (fork() > 0)
		pthread_join(barber, NULL);
	else {

	while (1)
	{
		printf("There are %d chairs available.\n", chairs_available());
		unsigned int iclients;
		printf("Enter number of clients:\n");
		scanf("%u", &iclients);

		int s, av = chairs_available();
		if ((int)iclients >= av)
			s = av;
		else
			s = iclients;

		if (((int)iclients - av) > 0)
			printf("%d customers will leave.\n", (iclients-av));

		int i;
		for (i = 0; i < s; i++)
		{
			int x = available_chair();
			pthread_create(&waiting_customers[x], NULL, (void*)run_customer, NULL);
			chairs[x] = 1;
			pthread_join(waiting_customers[x], NULL);
		}
	}}

	return 0;
}

int chairs_available()
{
	int counter = 0, i;
	for (i = 0; i < sizeof(chairs); i++)
	{
		if (!chairs[i])
			counter++;
	}
	return counter;
}

void run_barber()
{
	while (1)
	{
		if (!working)
		{
			pthread_mutex_lock(&barberchair);
			printf("Barber is sleeping...\n");
			//waiting to someone to wake the barber.
			while ((working = check_for_customer()) == -1)
				working = 0;

			call_customer(&working);
			pthread_mutex_unlock(&barberchair);
		}
		else //customer that was sitting at charis[working], THE ACTUAL CHAIR IT IS AVAILABLE AT THIS POINT.
			printf("Attending customer #%d.\n", working);
	}
}

int check_for_customer()
{
	int i;
	for (i = 0; i < sizeof(chairs); i++)
	{
		if (chairs[i])
			return i;
	}

	return -1;
}

void call_customer(int *i)
{
	pthread_mutex_lock(&barberchair);
	chairs[*i] = 0;
	pthread_mutex_unlock(&barberchair);
}

int available_chair()
{
	int i;
	for (i = 0; i < sizeof(chairs); i++)
	{
		if (!chairs[i])
			return i;
	}
	return -1;
}

void run_customer()
{
	printf("hello\n");
}
