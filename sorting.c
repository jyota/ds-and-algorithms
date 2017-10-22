/* Exercise 3-37 from Skiena's Algorithms Design Manual
   --------
   Implement versions of several different sorting algorithms,
   such as selection sort, insertion sort, mergesort, and quicksort.
   Conduct experiments to assess performance of these algorithms in
   a simple app that reads a large text file and reports exactly
   one instance of each word that appears within it. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

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
    for(i = 0; i < N; ++i){
        working_words[i] = strdup(words[i]);
    }
    selection_sort(working_words, N);
    print_unique(working_words, N);

    return 0;
}