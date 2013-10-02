#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_t barber;
pthread_t waiting_customers[5];
pthread_mutex_t barberchair;
static char chairs[5];
static int working; //index of the customer's chair being attended. THE ACTUAL CHAIR IS AVAILABLE. -1 is for sleeping.

int chairs_available(void); //return number of chairs available
void run_barber(void);
int check_for_customer(void); //returns de index of ther 1st chair that has waiting customer, returns -1 if there's none.
void call_customer(int*); //locks the mutex and empties the chairs[working]
int available_chair(void); //returns the index of the 1st chair available, returns -1 if there's non
void run_customer(int*); //Customer will wake Barber if there is not other customer, else will wait to be called.

int main(int argc, char **argv)
{
	working = sizeof(chairs);
	int c_index[5] = {0, 1, 2, 3, 4};

	pthread_mutex_init(&barberchair, NULL);


		pthread_create(&barber, NULL, (void*)run_barber, NULL);

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
			pthread_create(&waiting_customers[x], NULL, (void*)run_customer, (void*)&c_index[x]);
			chairs[x] = 1;
		}
		for (i = 0; i < s; i++)
			pthread_join(waiting_customers[i], NULL);

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
		sleep(2);
		if (working != -1 && working != sizeof(chairs))
			call_customer(&working);
		else if (working == sizeof(chairs))
		{
			pthread_mutex_lock(&barberchair);
			printf("Barber is checking for customer...\n");
			working = check_for_customer();
			pthread_mutex_unlock(&barberchair);
		}
		else if (working == -1)
			printf("Barber is sleeping...\n");

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
	printf("Shaving customer #%d\n", *i+1);
	pthread_mutex_lock(&barberchair);
	chairs[*i] = 0;
	printf("Customer #%d left.\n", *i+1);
	*i = sizeof(chairs);
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

void run_customer(int *i)
{
	while (1)
	{
		sleep(2);
		if (working == -1)
		{
			pthread_mutex_lock(&barberchair);
			printf("Customer #%d is waking Barber.\n", *i+1);
			working = *i;
			pthread_mutex_unlock(&barberchair);
		}
		else if (!chairs[*i])
			break;
		else
			printf("Customer #%d is waiting...\n", *i+1);


	}
}
