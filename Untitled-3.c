#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 100
#define NUM_BUCKETS 10
#define MAX_VALUE 100

// Bucket structure
typedef struct {
    int* elements;
    int size;
    int capacity;
} Bucket;

// Function to initialize buckets
void initialize_buckets(Bucket* buckets) {
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        buckets[i].elements = (int*)malloc(ARRAY_SIZE * sizeof(int));
        buckets[i].size = 0;
        buckets[i].capacity = ARRAY_SIZE;
    }
}

// Function to add an element to a bucket
void add_to_bucket(Bucket* bucket, int element) {
    if (bucket->size == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->elements = (int*)realloc(bucket->elements, bucket->capacity * sizeof(int));
    }
    bucket->elements[bucket->size++] = element;
}

// Comparison function for qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Main Bucket Sort function
void bucket_sort(int* array, int size) {
    Bucket buckets[NUM_BUCKETS];
    initialize_buckets(buckets);

    // Distribute elements into buckets
    for (int i = 0; i < size; ++i) {
        int index = array[i] * NUM_BUCKETS / MAX_VALUE;
        add_to_bucket(&buckets[index], array[i]);
    }

    // Sort each bucket and merge
    int array_index = 0;
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        qsort(buckets[i].elements, buckets[i].size, sizeof(int), compare);
        for (int j = 0; j < buckets[i].size; ++j) {
            array[array_index++] = buckets[i].elements[j];
        }
        free(buckets[i].elements); // Free memory allocated for bucket elements
    }
}

// Utility function to generate random array
void generate_random_array(int* array, int size) {
    for (int i = 0; i < size; ++i) {
        array[i] = rand() % MAX_VALUE;
    }
}

// Utility function to print array
void print_array(int* array, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// Example usage
int main() {
    int array[ARRAY_SIZE];
    generate_random_array(array, ARRAY_SIZE);

    printf("Original array:\n");
    print_array(array, ARRAY_SIZE);

    bucket_sort(array, ARRAY_SIZE);

    printf("Sorted array:\n");
    print_array(array, ARRAY_SIZE);

    return 0;
}
