/* Exercise 3-38 from Skiena's Algorithms Design Manual
   --------
   Implement external sort on a larger than memory data set. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

const int max_chunks = 999;
const int chunk_size = 500;

int num_chunks = 0;

typedef struct {
    int value;
    int source;
} heapnode;

typedef struct {
    heapnode q[100000];
    int n;
} priority_queue;

int pq_parent(int n)
{
    if(n == 1){ 
        return -1;
    }else{
        return (int) n / 2;
    } 
}

int pq_young_child(int n)
{
    return 2 * n;
}

void pq_swap(priority_queue *q, int i, int j)
{
	heapnode temp;

	temp = q->q[i];
	q->q[i] = q->q[j];
	q->q[j] = temp;
}

void bubble_up(priority_queue *q, int p)
{
    if(pq_parent(p) == -1) return;
    if(q->q[pq_parent(p)].value > q->q[p].value){
        pq_swap(q, p, pq_parent(p));
        bubble_up(q, pq_parent(p));
    }
}

void pq_insert(priority_queue *q, int x, int source)
{
    if(q->n >= 100000){
        printf("Warning: priority queue overflow insert\n");
    }else{
        q->n = (q->n) + 1;
        q->q[q->n].value = x;
        q->q[q->n].source = source;
        bubble_up(q, q->n);
    }
}

void pq_init(priority_queue *q)
{
    q->n = 0;
}

void make_heap(priority_queue *q, int s[], int n)
{
    int i;

    pq_init(q);
    for(i = 0; i < n; ++i){
        pq_insert(q, s[i], 0);
    }
}

void bubble_down(priority_queue *q, int p)
{
    int child;
    int i;
    int min_index;

    child = pq_young_child(p);
    min_index = p;

    for(i = 0; i <= 1; ++i){
        if((child + i) <= q->n){
            if(q->q[min_index].value > q->q[child + i].value){
                min_index = child + i;
            }
        }
    }

    if(min_index != p){
        pq_swap(q, p, min_index);
        bubble_down(q, min_index);
    }
}

heapnode extract_min(priority_queue *q)
{
    heapnode min;

    if(q->n <= 0){
        printf("Warning: empty priority queue.\n");
    }else{
        min = q->q[1];

        q->q[1] = q->q[q->n];
        q->n = q->n - 1;
        bubble_down(q, 1);
    }
    return min;
}

void my_heapsort(int s[], int n)
{
    int i;
    priority_queue q;

    make_heap(&q, s, n);

    for(i = 0; i < n; ++i){
        s[i] = extract_min(&q).value;
    }
}

int cmpfnc (const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void create_sorted_chunks(char *filename)
{
    int arr[chunk_size];
    char *outfilename = malloc(sizeof(char) * 12);
    int i = 0;
    int j = 0;
    int chunks_read = 0;
    FILE *in = fopen(filename, "r");

    while(j < max_chunks){
        snprintf(outfilename, 12, "%d.sorted", num_chunks);
        for(i = 0; i < chunk_size; ++i){
            if(fscanf(in, "%d ", &arr[i]) != 1){
                break;
            }
            chunks_read++;
        }
        if(chunks_read > 0){
            qsort(arr, chunks_read, sizeof(int), cmpfnc);
            FILE *out = fopen(outfilename, "w");
            for(i = 0; i < chunks_read; ++i){
                fprintf(out, "%d ", arr[i]);
            }
            fclose(out);
            ++num_chunks;
        }
        if(chunks_read < chunk_size){
            break;
        }
        chunks_read = 0;
    }
    fclose(in);    
}
 
int main(int argc, char *argv[])
{   
    create_sorted_chunks("random_small.txt");
    return 0;
}
