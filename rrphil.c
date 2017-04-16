#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX 10
#define LEFT (philosopher_num+(N-1))%N
#define RIGHT (philosopher_num+1)%N

enum state {THINKING,HUNGRY,EATING};
enum state philState[MAX];

float millis;
clock_t start,end;
int timer[MAX];
int N;


void* philospher(void *num);
void  pickup_forks(int);
void  return_forks(int);
void  philosopher_test(int);


pthread_mutex_t mutex;
pthread_cond_t cond_var;
int phil_num[MAX];


int main()
{
   char ch='n';
	start=clock();

	printf("Enter number of Philosophers(max 10): \n");
	scanf("%d",&N);
	for(int i=0;i<N;i++)
        timer[i]=0;
	for(int k=0;k<N;k++)
		phil_num[k]=k;
	do{
			 pthread_t thread_id[N];
			 pthread_mutex_init(&mutex,NULL);
			 pthread_cond_init(&cond_var,NULL);
			 
			 for(int i=0;i<N;i++)
			 {
				  pthread_create(&thread_id[i],NULL,philospher,&phil_num[i]);
				  printf("Philosopher %d is thinking\n",i+1);
			 }
			 for(int i=0;i<N;i++)
				  pthread_join(thread_id[i],NULL);

			 printf("\nDo you wish to continue: ");
					scanf("\n%c",&ch);  
     }while(ch=='y' || ch=='Y');
     for(int i=0;i<N;i++)
             printf("Philosopher %d eating time is %d\n",i,timer[i] );
      end=clock();
      millis=(end-start)/100;
      printf("Elapsed Time = %f\n",millis );
      exit(0);
}

void *philospher(void *num)	{
     int *i = num;
     pickup_forks(*i);
     int eat_time=(rand()%3)+1;
     sleep(eat_time);
     timer[*i]=eat_time+timer[*i];
     return_forks(*i);
}

void pickup_forks(int philosopher_num)	{
	pthread_mutex_lock(&mutex);
	philState[philosopher_num] = HUNGRY;
	printf("Philosopher %d is Hungry\n",philosopher_num+1);
	philosopher_test(philosopher_num);
	while (philState[philosopher_num]!=EATING)
	pthread_cond_wait(&cond_var, &mutex);
	pthread_mutex_unlock(&mutex);
}
  

  
void return_forks(int philosopher_num)	{
		pthread_mutex_lock(&mutex);

		philState[philosopher_num] = THINKING;
		printf("Philosopher %d returned chopsticks:\t%d and %d\n",philosopher_num+1,LEFT+1,philosopher_num+1);
		printf("Philosopher %d is thinking\n",philosopher_num+1);

		philosopher_test(LEFT);
		philosopher_test(RIGHT);
		pthread_mutex_unlock(&mutex);
}


void philosopher_test(int philosopher_num)
{
    if (philState[philosopher_num] == HUNGRY && philState[LEFT] != EATING && philState[RIGHT] != EATING)
    {
		philState[philosopher_num] = EATING;
		printf("Philosopher %d picked up chopsticks:\t%d and %d\n",philosopher_num+1,LEFT+1,philosopher_num+1);
		printf("\tPhilosopher %d is Eating\n",philosopher_num+1);
    }
    pthread_cond_signal(&cond_var);
}
