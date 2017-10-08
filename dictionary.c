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

typedef enum { false, true } bool;
typedef enum { Red, Black } redblack;

typedef struct redblack_tree {
    char *item;
    struct redblack_tree *parent;
    struct redblack_tree *left;
    struct redblack_tree *right;
    redblack color;
} redblack_tree;

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

redblack_tree *search_redblack_tree(redblack_tree *l, char *x)
{
    if (l == NULL) return(NULL);

    int strcmp_result = strcmp(x, l->item);

    if(strcmp_result == 0){
        return(l);
    }else if(strcmp_result < 0){
        return(search_redblack_tree(l->left, x));
    }else{
        return(search_redblack_tree(l->right, x));
    }
}

void recolor_redblack_as_needed(redblack_tree *x)
{ 
    redblack_tree *uncle = NULL;
    redblack_tree *grandparent = NULL;
    
    if(x->parent == NULL){
        x->color = Black;
        return;
    }

    if(x->parent->parent != NULL){
        grandparent = x->parent->parent;
    } else {
        return;
    }

    if(grandparent->left == x->parent){
        uncle = grandparent->right;
    }else{
        uncle = grandparent->left;
    }

    if(uncle != NULL){
        x->parent->color = Black;
        uncle->color = Black;
        grandparent->color = Red;
        recolor_redblack_as_needed(grandparent);
    }
}

void left_left_case(redblack_tree *x)
{
    if(x->parent == NULL || x->parent->parent == NULL){
        printf("ERROR: Trying to right rotate without parent/grandparent...\n");
        return;
    }
    redblack_tree *orig_parent_right = x->parent->right;
    redblack_tree *orig_grandparent = x->parent->parent;
    x->parent->parent = orig_grandparent->parent;
    x->parent->right = orig_grandparent;
    orig_grandparent->parent = x;
    orig_grandparent->left = orig_parent_right;
    orig_grandparent->color = Red;
    x->parent->color = Black;
}

void left_right_case(redblack_tree *x)
{
    if(x->parent == NULL || x->parent->parent == NULL){
        printf("ERROR: Trying to left rotate without parent/grandparent...\n");
        return;
    }
    redblack_tree *orig_grandparent = x->parent->parent;
    redblack_tree *orig_parent = x->parent;

    orig_parent->right = NULL;
    x->left = orig_parent;
    orig_parent->parent = x;
    x->parent = orig_grandparent->parent;
    orig_grandparent->parent = x;
    x->right = orig_grandparent;
    orig_grandparent->left = NULL;
    orig_grandparent->color = Red;
    x->color = Black;
}

void right_left_case(redblack_tree *x)
{
    if(x->parent == NULL || x->parent->parent == NULL){
        printf("ERROR: Trying to left rotate without parent/grandparent...\n");
        return;
    }
    redblack_tree *orig_grandparent = x->parent->parent;
    redblack_tree *orig_parent = x->parent;

    orig_parent->left = NULL;
    x->right = orig_parent;
    orig_parent->parent = x;
    x->parent = orig_grandparent->parent;
    orig_grandparent->parent = x;
    x->left = orig_grandparent;
    orig_grandparent->right = NULL;
    orig_grandparent->color = Red;
    x->color = Black;
}

void right_right_case(redblack_tree *x)
{
    if(x->parent == NULL || x->parent->parent == NULL){
        printf("ERROR: Trying to left rotate without parent/grandparent...\n");
        return;
    }
    redblack_tree *orig_parent_left = x->parent->left;
    redblack_tree *orig_grandparent = x->parent->parent;
    x->parent->parent = orig_grandparent->parent;
    x->parent->left = orig_grandparent;
    orig_grandparent->parent = x;
    orig_grandparent->right = orig_parent_left;
    orig_grandparent->color = Red;
    x->parent->color = Black;
}


void redblack_insert_adjustment(redblack_tree *l)
{
    if(l->parent == NULL || l->parent->color == Black){
        return;
    }else{
        redblack_tree *uncle = NULL;
        redblack_tree *grandparent = NULL;
        if(l->parent->parent != NULL) grandparent = l->parent->parent;

        if(grandparent->left == l->parent){
            uncle = grandparent->right;
        }else{
            uncle = grandparent->left;
        }
        if(uncle != NULL){
            if(uncle->color == Red){
                l->parent->color = Black;
                uncle->color = Black;
                grandparent->color = Red;        
                recolor_redblack_as_needed(grandparent);
            }else{
                if(grandparent == NULL){
                    return;
                } else if (grandparent->left == l->parent && l->parent->left == l){
                    left_left_case(l);
                } else if (grandparent->left == l->parent && l->parent->right == l){
                    left_right_case(l);
                } else if (grandparent->right == l->parent && l->parent->right == l){
                    right_right_case(l);
                } else if (grandparent->right == l->parent && l->parent->left == l){
                    right_left_case(l);
                }
            }
        }
    }
}

void insert_redblack_tree(redblack_tree **l, char *x, redblack_tree *parent)
{
    redblack_tree *p;

    if(*l == NULL){
        p = malloc(sizeof(redblack_tree));
        p->item = strdup(x);
        p->left = NULL;
        p->right = NULL;
        p->parent = parent;
        p->color = Red;
        *l = p;
        redblack_insert_adjustment(*l);
        return;
    }

    int strcmp_result = strcmp(x, (*l)->item);
    
    if(strcmp_result < 0){
        insert_redblack_tree(&((*l)->left), x, *l);
    }else{
        insert_redblack_tree(&((*l)->right), x, *l);
    }
}

int compute_black_height(redblack_tree* x) {
    if (x == NULL)
        return 0; 

    int left_height = compute_black_height(x->left);
    int right_height = compute_black_height(x->right);
    int add = x->color == Black ? 1 : 0;

    if (left_height == -1 || right_height == -1 || left_height != right_height)
        return -1; 
    else
        return left_height + add;
}

void traverse_redblack_tree_print(redblack_tree *l)
{
    if(l != NULL){
        //traverse_redblack_tree_print(l->left);
        printf("%s - color: %s\n", l->item, (l->color == Black ? "Black" : "Red"));
        traverse_redblack_tree_print(l->right);
    }
}

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

void add_or_skip_string_redblack_bst(redblack_tree **l, char *string)
{
    if(search_redblack_tree(*l, string) == NULL){
        printf("%s\n", string);
        insert_redblack_tree(l, string, NULL);
        (*l)->color = Black;
    }
}

void parse_text_dictionary_redblack_bst(char *text)
{
    redblack_tree *dictionary = malloc(sizeof(redblack_tree));
    char *word;
    bool first_time = true;

    for(word = strtok(text, " "); word; word = strtok(NULL, " ")){
        if(first_time){
            printf("%s\n", word);
            dictionary->item = strdup(word);
            dictionary->left = NULL;
            dictionary->right = NULL;
            dictionary->parent = NULL;
            dictionary->color = Black;
            first_time = false;
        }else{
            add_or_skip_string_redblack_bst(&dictionary, word);
        }
    }
    printf("Black height of red-black tree: %d\n", compute_black_height(dictionary));
    traverse_redblack_tree_print(dictionary);
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