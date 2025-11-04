/*
 * Latency measurement for mutex and semaphores
 * Systems Distributed and Concurrentes
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#define MEASUREMENT_TIME 60
#define SLEEP_MICROSECONDS 1000

pthread_mutex_t test_mutex;
sem_t test_semaphore;

void save_temporal_data(long* mutex_times, long* sem_times, int count) {
    FILE* mutex_file = fopen("mutex_temporal.dat", "w");
    FILE* sem_file = fopen("semaphore_temporal.dat", "w");
    
    if (!mutex_file || !sem_file) {
        printf("Error opening temporal data files\n");
        return;
    }
    
    // Guardar datos en orden temporal (medida, valor)
    for (int i = 0; i < count; i++) {
        fprintf(mutex_file, "%d %ld\n", i+1, mutex_times[i]);
        fprintf(sem_file, "%d %ld\n", i+1, sem_times[i]);
    }
    
    fclose(mutex_file);
    fclose(sem_file);
}

void measure_latencies() {
    struct timespec start, end;
    time_t start_time = time(NULL);
    
    /* Arrays para todas las muestras */
    long mutex_samples[60000];
    long sem_samples[60000];
    int sample_count = 0;
    
    printf("Measuring latencies for %d seconds...\n", MEASUREMENT_TIME);
    
    /* Medir todas las muestras */
    while (time(NULL) - start_time < MEASUREMENT_TIME) {
        long mutex_time, sem_time;
        
        /* Measure mutex latency */
        clock_gettime(CLOCK_MONOTONIC, &start);
        pthread_mutex_lock(&test_mutex);
        pthread_mutex_unlock(&test_mutex);
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        mutex_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        mutex_samples[sample_count] = mutex_time;
        
        /* Measure semaphore latency */
        clock_gettime(CLOCK_MONOTONIC, &start);
        sem_wait(&test_semaphore);
        sem_post(&test_semaphore);
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        sem_time = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        sem_samples[sample_count] = sem_time;
        
        sample_count++;
        usleep(SLEEP_MICROSECONDS);
    }
    
    /* Guardar datos en orden temporal para Python */
    save_temporal_data(mutex_samples, sem_samples, sample_count);
    
    /* Calcular estadísticas */
    long mutex_min = mutex_samples[0], mutex_max = mutex_samples[0], mutex_total = 0;
    long sem_min = sem_samples[0], sem_max = sem_samples[0], sem_total = 0;
    double mutex_sum_sq = 0.0, sem_sum_sq = 0.0;
    
    for (int i = 0; i < sample_count; i++) {
        mutex_total += mutex_samples[i];
        sem_total += sem_samples[i];
        if (mutex_samples[i] < mutex_min) mutex_min = mutex_samples[i];
        if (mutex_samples[i] > mutex_max) mutex_max = mutex_samples[i];
        if (sem_samples[i] < sem_min) sem_min = sem_samples[i];
        if (sem_samples[i] > sem_max) sem_max = sem_samples[i];
    }
    
    double mutex_avg = (double)mutex_total / sample_count;
    double sem_avg = (double)sem_total / sample_count;
    
    for (int i = 0; i < sample_count; i++) {
        mutex_sum_sq += (mutex_samples[i] - mutex_avg) * (mutex_samples[i] - mutex_avg);
        sem_sum_sq += (sem_samples[i] - sem_avg) * (sem_samples[i] - sem_avg);
    }
    
    double mutex_std = sqrt(mutex_sum_sq / sample_count);
    double sem_std = sqrt(sem_sum_sq / sample_count);
    
    printf("\n=== MUTEX STATISTICS ===\n");
    printf("Samples: %d\n", sample_count);
    printf("Minimum: %ld ns\n", mutex_min);
    printf("Maximum: %ld ns\n", mutex_max);
    printf("Average: %.2f ns\n", mutex_avg);
    printf("Std Dev: %.2f ns\n", mutex_std);
    
    printf("\n=== SEMAPHORE STATISTICS ===\n");
    printf("Samples: %d\n", sample_count);
    printf("Minimum: %ld ns\n", sem_min);
    printf("Maximum: %ld ns\n", sem_max);
    printf("Average: %.2f ns\n", sem_avg);
    printf("Std Dev: %.2f ns\n", sem_std);
    
    printf("\nTemporal data saved to mutex_temporal.dat and semaphore_temporal.dat\n");
}

int main() {
    pthread_mutex_init(&test_mutex, NULL);
    sem_init(&test_semaphore, 0, 1);
    
    measure_latencies();
    
    pthread_mutex_destroy(&test_mutex);
    sem_destroy(&test_semaphore);
    
    return 0;
}