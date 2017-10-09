/* Exercise 3-16 from Skiena's Algorithms Design Manual
   --------
   Implement versions of several different dictionary
   data structures (linked lists, binary trees, 
   balanced binary search trees, and hash tables).
   Check their performance by reading a large text file
   and insert/report each word in the file
   not found in the dictionary up to that point. 
   
   My notes: 
   - for this task, dictionary implementations 
     only need to be able to insert and check for existing
     keys -- no delete or other operations needed.
   - I remove punctuation characters and 
     use whitespace to tokenize words. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "redblacktree.h" // using someone else's implementation

typedef enum { false, true } bool;

typedef struct tree {
    char *item;
    struct tree *parent;
    struct tree *left;
    struct tree *right;
} tree;

typedef struct list {
    char *item;
    struct list *next;
} list;

tree *search_tree(tree *l, char *x)
{
    if (l == NULL) return(NULL);

    int strcmp_result = strcmp(x, l->item);

    if(strcmp_result == 0){
        return(l);
    }else if(strcmp_result < 0){
        return(search_tree(l->left, x));
    }else{
        return(search_tree(l->right, x));
    }
}

void insert_tree(tree **l, char *x, tree *parent)
{
    tree *p;

    if(*l == NULL){
        p = malloc(sizeof(tree));
        p->item = strdup(x);
        p->left = NULL;
        p->right = NULL;
        p->parent = parent;
        *l = p;
        return;
    }

    int strcmp_result = strcmp(x, (*l)->item);
    
    if(strcmp_result < 0){
        insert_tree(&((*l)->left), x, *l);
    }else{
        insert_tree(&((*l)->right), x, *l);
    }
}

void traverse_tree_print(tree *l)
{
    if(l != NULL){
        traverse_tree_print(l->left);
        printf("%s\n", l->item);
        traverse_tree_print(l->right);
    }
}

void free_linked_list(list **l)
{
    list *current = *l;
    while(current != NULL){
    list *this = current;
    current = current->next;
    free(this);
    }
}

void insert_list(list **l, char *string)
{
    list *p;

    p = malloc(sizeof(list));
    p->item = strdup(string);
    p->next = *l;
    *l = p;
}
    
list *search_list(list *l, char *string)
{
    if(l == NULL) return(NULL);

    if(strcmp(l->item, string) == 0){
        return(l);
    }else{
        return(search_list(l->next, string));
    }
}

void add_or_skip_string_list(list **l, char *string)
{
    if(search_list(*l, string) == NULL){
        printf("%s\n", string);
        insert_list(l, string);
    }
}

void parse_text_dictionary_list(char *text)
{
    list *dictionary = malloc(sizeof(list));
    char *word;
    bool first_time = true;

    for(word = strtok(text, " "); word; word = strtok(NULL, " ")){
        if(first_time){
            printf("%s\n", word);
            dictionary->item = strdup(word);
            dictionary->next = NULL;    
            first_time = false;
        }else{
            add_or_skip_string_list(&dictionary, word);
        }
    }
}

void add_or_skip_string_bst(tree **l, char *string)
{
    if(search_tree(*l, string) == NULL){
        printf("%s\n", string);
        insert_tree(l, string, NULL);
    }
}

void parse_text_dictionary_bst(char *text)
{
    tree *dictionary = malloc(sizeof(tree));
    char *word;
    bool first_time = true;

    for(word = strtok(text, " "); word; word = strtok(NULL, " ")){
        if(first_time){
            printf("%s\n", word);
            dictionary->item = strdup(word);
            dictionary->left = NULL;
            dictionary->right = NULL;    
            dictionary->parent = NULL;
            first_time = false;
        }else{
            add_or_skip_string_bst(&dictionary, word);
        }
    }    
}

void add_or_skip_string_redblack_bst(struct redblack_tree *l, char *string)
{
    struct rb_node *result = tree_find(l, string);
    if(result->key == NULL){
        printf("%s\n", string);
        tree_insert(l, string);        
    }
}

void parse_text_dictionary_redblack_bst(char *text)
{
    struct redblack_tree *dictionary;
    char *word;
    init_tree(&dictionary);

    for(word = strtok(text, " "); word; word = strtok(NULL, " ")){
        add_or_skip_string_redblack_bst(dictionary, word);
    }
    //traverse_redblack_tree_print(dictionary);    
    //printf("Black height: %d\n", compute_black_height(dictionary));
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
    char *file_text = read_file("genebuild.txt");
    //parse_text_dictionary_list(file_text);
    //parse_text_dictionary_bst(file_text);
    parse_text_dictionary_redblack_bst(file_text);

    return 0;
}
