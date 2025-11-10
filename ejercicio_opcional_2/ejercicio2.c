#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_BUFFER 10
#define MAX_DATA_PRODUCER 10
#define SLEEP_TIME 50000

#define MAX_PRODUCERS 10

sem_t empty;
sem_t full;
sem_t semaforo;

int n_elems=0;
int buffer[MAX_BUFFER];
int pos = 0 ;

void reset () {
  printf("\033[0m");
}

void blue () {
  printf("\033[0;34m");
}

void red () {
  printf("\033[1;31m");
}

void yellow () {
  printf("\033[1;33m");
}

void purple () {
  printf("\033[0;35m");
}


void* producer (void* args ) {
  
  int aux_pos = 0;

  for (int i=0; i<MAX_DATA_PRODUCER; i++) {

    sem_wait(&empty);
    sem_wait(&semaforo);
    int producer_id = *(int *) args;
    blue();
    printf("[PRODUCER ID=%d][%d] \t ENTER in critial region\n", producer_id,i); 
    reset();

    buffer[pos] = i;
    aux_pos = pos;
    pos = (pos + 1) % MAX_BUFFER;
    n_elems ++;

    sem_post(&semaforo);
    sem_post(&full);
    
    purple();
    printf("[PRODUCER ID=%d][%d] \t BUFFER[%d]=%d \n", producer_id, i, aux_pos, i);
    reset();

    usleep(SLEEP_TIME);
  }
  pthread_exit(0);
}

void consumer (void) {

  int data = 0;
  int pos_ = 0;
  int aux_pos = 0;

  for (int i=0; i<MAX_DATA_PRODUCER*MAX_PRODUCERS; i++) {

    sem_wait(&full);
    sem_wait(&semaforo);    

    data = buffer[pos_];
    aux_pos = pos_;
    pos_ = (pos_ + 1) % MAX_BUFFER;

    sem_post(&semaforo);
    sem_post(&empty);

    purple();
    printf("CONSUMER \t\t BUFFER[%d]=%d \n", aux_pos, data);
    reset();
    usleep(SLEEP_TIME*3);
  }
  pthread_exit(0);
}




int main(void)
{
  pthread_t th_consumer, th_producer[MAX_PRODUCERS];
  int ids[MAX_PRODUCERS];

  sem_init(&empty, 0, MAX_BUFFER);
  sem_init(&full, 0, 0);
  sem_init(&semaforo, 0, 1);


  pthread_create(&th_consumer, NULL, (void *)&consumer, NULL );

  for (int i=0; i<MAX_PRODUCERS; i++) {
    ids[i] = i;    
    pthread_create(&th_producer[i], NULL, (void *)&producer, (void *) &ids[i]);
    
  }
    
  pthread_join(th_consumer, NULL);

  for (int i=0; i<MAX_PRODUCERS; i++) {
    pthread_join(th_producer[i], NULL);
  }

  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&semaforo);

  return 0;

}