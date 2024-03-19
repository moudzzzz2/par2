

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_BUCKETS 10
#define NUM_THREADS NUM_BUCKETS
#define ARRAY_SIZE 100
#define MAX_VALUE 100

int array[ARRAY_SIZE];
int* buckets[NUM_BUCKETS];
int bucket_sizes[NUM_BUCKETS];
pthread_t threads[NUM_THREADS];

void initialize_array() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % MAX_VALUE;
    }
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void* sort_bucket(void* arg) {
    int index = *(int*)arg;
    qsort(buckets[index], bucket_sizes[index], sizeof(int), compare);
    return NULL;
}

void distribute_elements_to_buckets() {
    for (int i = 0; i < NUM_BUCKETS; i++) {
        buckets[i] = malloc(ARRAY_SIZE * sizeof(int)); // Simplified allocation
        bucket_sizes[i] = 0;
    }
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int bucket_index = array[i] * NUM_BUCKETS / MAX_VALUE;
        buckets[bucket_index][bucket_sizes[bucket_index]++] = array[i];
    }
}

void merge_buckets() {
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 0; j < bucket_sizes[i]; j++) {
            array[index++] = buckets[i][j];
        }
        free(buckets[i]); // Clean up
    }
}

int main() {
    initialize_array();
    distribute_elements_to_buckets();

    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sort_bucket, (void*)&thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    merge_buckets();

    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}