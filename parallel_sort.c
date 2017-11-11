/* Exercise 3-39 from Skiena's Algorithms Design Manual
   --------
   Implement parallel sort. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

#define NOTHREADS 4

const int max_buffer_size = 10000000;

int *mergesort_work;
int *random_items;

typedef struct {
    int l;
    int r;
} merge_node;


void merge(int l, int m, int r)
{
    int i, j, k;

    for(i = m + 1; i > l; --i){
        mergesort_work[i - 1] = random_items[i - 1];
    }
    for(j = m; j < r; ++j){
        mergesort_work[r + m - j] = random_items[j + 1];
    }
    for(k = l; k <= r; ++k){
        if(mergesort_work[j] < mergesort_work[i]){
            random_items[k] = mergesort_work[j--];
        }else{
            random_items[k] = mergesort_work[i++];
        }
    }
}

void my_mergesort(merge_node which)
{
    int m = (which.r + which.l) / 2;
    merge_node left;
    merge_node right;
    left.l = which.l;
    left.r = m;
    right.l = m + 1;
    right.r = which.r;

    if(which.r <= which.l) return;

    my_mergesort(left);
    my_mergesort(right);
    merge(which.l, m, which.r);
}

void parallel_mergesort(void *input)
{
    merge_node *which = (merge_node *) input;
    int m = (which->r + which->l) / 2;
    merge_node left;
    merge_node right;
    left.l = which->l;
    left.r = m;
    right.l = m + 1;
    right.r = which->r;
    
    pthread_t tid1;
    pthread_t tid2;
    int ret;

    if(which->r <= which->l) return;

    ret = pthread_create(&tid1, NULL, parallel_mergesort, &left);
    if(ret){
        printf("Unable to create thread.\n");
        exit(1);
    }

    ret = pthread_create(&tid2, NULL, parallel_mergesort, &right);
    if(ret){
        printf("Unable to create thread.\n");
        exit(1);
    }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    merge(which->l, m, which->r);
    pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
    int i;
    random_items = malloc(sizeof(int) * max_buffer_size);
    mergesort_work = malloc(sizeof(int) * max_buffer_size);
    clock_t start, end;
    pthread_t tid;
    int ret;
    double non_parallel_time;
    double parallel_time;
    merge_node initial;
    initial.l = 0;
    initial.r = max_buffer_size;

    srand(9999);
    
    for(i = 0; i < max_buffer_size; ++i){
        random_items[i] = rand();
    }

    start = clock();
    my_mergesort(initial);
    end = clock();
    non_parallel_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(i = 0; i < max_buffer_size; ++i){
        random_items[i] = rand();
    }

    start = clock();
    ret=pthread_create(&tid, NULL, parallel_mergesort, &initial);
    if(ret){
        printf("Unable to create thread.\n");
        exit(1);
    }
    pthread_join(tid, NULL);
    end = clock();
    parallel_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(i = 0; i < 30; ++i){
        printf("%d\n", random_items[i]);
    }
    //printf("Non-parallel time to sort: %f\nParallel time to sort: %f\n", non_parallel_time, parallel_time);

    return 0;
}