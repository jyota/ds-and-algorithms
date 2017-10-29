/* Exercise 3-37 from Skiena's Algorithms Design Manual
   --------
   Implement versions of several different sorting algorithms,
   such as selection sort, insertion sort, heapsort, mergesort, 
   and quicksort.
   Conduct experiments to assess performance of these algorithms in
   a simple app that reads a large text file and reports exactly
   one instance of each word that appears within it. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

typedef struct {
    char *q[100000];
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
	char *temp;

	temp = q->q[i];
	q->q[i] = strdup(q->q[j]);
	q->q[j] = strdup(temp);
}

void bubble_up(priority_queue *q, int p)
{
    if(pq_parent(p) == -1) return;
    if(strcmp(q->q[pq_parent(p)], q->q[p]) > 0){
        pq_swap(q, p, pq_parent(p));
        bubble_up(q, pq_parent(p));
    }
}

void pq_insert(priority_queue *q, char *x)
{
    if(q->n >= 100000){
        printf("Warning: priority queue overflow insert x=%s\n", x);
    }else{
        q->n = (q->n) + 1;
        q->q[q->n] = strdup(x);
        bubble_up(q, q->n);
    }
}

void pq_init(priority_queue *q)
{
    q->n = 0;
}

void make_heap(priority_queue *q, char *s[], int n)
{
    int i;

    pq_init(q);
    for(i = 0; i < n; ++i){
        pq_insert(q, s[i]);
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
            if(strcmp(q->q[min_index], q->q[child + i]) > 0){
                min_index = child + i;
            }
        }
    }

    if(min_index != p){
        pq_swap(q, p, min_index);
        bubble_down(q, min_index);
    }
}

char *extract_min(priority_queue *q)
{
    char *min;

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

int partition(char *s[], int l, int r)
{
    int i = l -1;
    int j = r;
    char *v = strdup(s[r]);

    for(;;){
        while(strcmp(s[++i], v) < 0){
            // do nothing but increment i
        }
        while(strcmp(v, s[--j]) < 0){
            // do nothing but decrement j
            if(j == l){
                break;
            }
        }
        if(i >= j){
            break;
        }
        char *temp = strdup(s[i]);
        s[i] = strdup(s[j]);
        s[j] = temp;
    }
    char *final_temp = strdup(s[i]);
    s[i] = strdup(s[r]);
    s[r] = final_temp;
    return i;
}

void quicksort(char *s[], int l, int r)
{
    int i;
    if(r <= l) return;
    i = partition(s, l, r);
    quicksort(s, l, i - 1);
    quicksort(s, i + 1, r);
}

void my_heapsort(char *s[], int n)
{
    int i;
    priority_queue q;

    make_heap(&q, s, n);

    for(i = 0; i < n; ++i){
        s[i] = extract_min(&q);
    }
}

void print_unique(char *words[], int N)
{
    int i;
    int cmp_result;
    for(i = 1; i < N; ++i){
        cmp_result = strcmp(words[i - 1], words[i]);
        if(cmp_result != 0){
            printf("%s\n", words[i]);
        }
    }
}

void insertion_sort(char *words[], int N)
{
    int i;
    int j;

    for(i = 1; i < N; ++i){
        j = i;
        while((j > 0) && (strcmp(words[j], words[j-1]) < 0)){
            char *string_to_move = words[j];
            words[j] = words[j-1];
            words[j-1] = string_to_move;
            --j;
        }
    }
}

void selection_sort(char *words[], int N)
{
    int i;
    int j;
    int cmp_result;

    for(i = 0; i < N; ++i){
        for(j = i+1; j < N; ++j){
            cmp_result = strcmp(words[j], words[i]);
            if(cmp_result < 0){
                char *string_to_move = words[i];
                words[i] = words[j];
                words[j] = string_to_move;
            }
        }
    }
}

char *read_file(char *filename)
{
    char *returnable = NULL;
    FILE *fp = fopen(filename, "r");

    if(fp != NULL){
        if(fseek(fp, 0L, SEEK_END) == 0){
            long buffer_size = ftell(fp);
            if(buffer_size == -1){
                return NULL;
            }

            returnable = malloc(sizeof(char) * (buffer_size + 1));

            if(fseek(fp, 0L, SEEK_SET) != 0){
                return NULL;
            }

            size_t new_length = fread(returnable, sizeof(char), buffer_size, fp);
            if(ferror(fp) != 0){
                fputs("Error reading file.\n", stderr);
            }else{
                returnable[new_length++] = '\0';
            }
        }
        fclose(fp);
    }

    size_t position = 0;
    char *current = NULL;
    for (current = returnable; *current; ++current){
        if (isalpha(*current) || isspace(*current)){
            if(!isspace(*current)){
                returnable[position++] = tolower(*current);
            }else{
                returnable[position++] = ' ';
            }
        }
    }
    returnable[position] = '\0';

    return returnable;
}

int main(int argc, char *argv[])
{   
    clock_t start, end;
    double selection_sort_time;
    double insertion_sort_time;
    double heapsort_time;
    double quicksort_time;

    char *word;
    char *file_text = read_file("poe-narrative-695.txt"); 
    char *words[100000];
    char *working_words[100000];
    int N = 0;
    int i;

    for(word = strtok(file_text, " "); word; word = strtok(NULL, " ")){
        words[N] = strdup(word);
        ++N;
    }

    // Setup a copy to keep the original array unsorted.
    
    for(i = 0; i < N; ++i){
        working_words[i] = strdup(words[i]);
    }
    start = clock();
    selection_sort(working_words, N);
    end = clock();
    selection_sort_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(i = 0; i < N; ++i){
        working_words[i] = strdup(words[i]);
    }
    start = clock();
    insertion_sort(working_words, N);
    end = clock();
    insertion_sort_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(i = 0; i < N; ++i){
        working_words[i] = strdup(words[i]);
    }
    start = clock();
    my_heapsort(working_words, N);
    end = clock();
    heapsort_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    for(i = 0; i < N; ++i){
        working_words[i] = strdup(words[i]);
    }
    start = clock();
    quicksort(working_words, 0, N - 1);
    end = clock();
    quicksort_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Selection sort time: %f\nInsertion sort time: %f\nHeapsort time: %f\nQuicksort time: %f\n",
           selection_sort_time, insertion_sort_time, heapsort_time, quicksort_time);

    return 0;
}
