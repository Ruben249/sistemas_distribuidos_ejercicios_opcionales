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

// calculate min, max, avg, stddev
void calculate_stats(long* data, int n, long* min, long* max, double* avg, double* std) {
    *min = data[0];
    *max = data[0];
    long total = 0;
    
    for (int i = 0; i < n; i++) {
        total += data[i];
        if (data[i] < *min) *min = data[i];
        if (data[i] > *max) *max = data[i];
    }
    
    *avg = (double)total / n;
    
    double sum_sq = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq += (data[i] - *avg) * (data[i] - *avg);
    }
    *std = sqrt(sum_sq / n);
}

// save temporal data to file
void save_temporal_data(long* times, int count, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return;
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %ld\n", i+1, times[i]);
    }
    
    fclose(file);
}

// measure latency of mutex or semaphore
int measure_latency(long* samples, int is_mutex) {
    struct timespec start, end;
    time_t start_time = time(NULL);
    int sample_count = 0;
    
    while (time(NULL) - start_time < MEASUREMENT_TIME) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        if (is_mutex) {
            pthread_mutex_lock(&test_mutex);
            pthread_mutex_unlock(&test_mutex);
        } else {
            sem_wait(&test_semaphore);
            sem_post(&test_semaphore);
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        long latency = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        samples[sample_count] = latency;
        sample_count++;
        usleep(SLEEP_MICROSECONDS);
    }
    
    return sample_count;
}

int main() {
    pthread_mutex_init(&test_mutex, NULL);
    sem_init(&test_semaphore, 0, 1);
    
    long mutex_samples[60000];
    long sem_samples[60000];
    
    printf("Measuring MUTEX latencies for %d seconds...\n", MEASUREMENT_TIME);
    int mutex_count = measure_latency(mutex_samples, 1);
    save_temporal_data(mutex_samples, mutex_count, "mutex_temporal.dat");
    
    printf("Measuring SEMAPHORE latencies for %d seconds...\n", MEASUREMENT_TIME);
    int sem_count = measure_latency(sem_samples, 0);
    save_temporal_data(sem_samples, sem_count, "semaphore_temporal.dat");
    
    long mutex_min, mutex_max, sem_min, sem_max;
    double mutex_avg, mutex_std, sem_avg, sem_std;
    
    calculate_stats(mutex_samples, mutex_count, &mutex_min, &mutex_max, &mutex_avg, &mutex_std);
    calculate_stats(sem_samples, sem_count, &sem_min, &sem_max, &sem_avg, &sem_std);
    
    printf("\n=== MUTEX STATISTICS ===\n");
    printf("Samples: %d\n", mutex_count);
    printf("Minimum: %ld ns\n", mutex_min);
    printf("Maximum: %ld ns\n", mutex_max);
    printf("Average: %.2f ns\n", mutex_avg);
    printf("Std Dev: %.2f ns\n", mutex_std);
    
    printf("\n=== SEMAPHORE STATISTICS ===\n");
    printf("Samples: %d\n", sem_count);
    printf("Minimum: %ld ns\n", sem_min);
    printf("Maximum: %ld ns\n", sem_max);
    printf("Average: %.2f ns\n", sem_avg);
    printf("Std Dev: %.2f ns\n", sem_std);
    
    printf("\nTemporal data saved to mutex_temporal.dat and semaphore_temporal.dat\n");
    
    pthread_mutex_destroy(&test_mutex);
    sem_destroy(&test_semaphore);
    
    return 0;
}