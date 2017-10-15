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
#include <time.h>

typedef enum { false, true } bool;

typedef struct tree {
    char *item;
    struct tree *parent;
    struct tree *left;
    struct tree *right;
} tree;

typedef struct redblack_tree {
    char *item;
    struct redblack_tree *parent;
    struct redblack_tree *left;
    struct redblack_tree *right;
    bool red;
} redblack_tree;

typedef struct list {
    char *item;
    struct list *next;
} list;

char *hash_map[65536];

int hash(char *string, int bins)
{
    int hash = 0;
    int a = 127;
    for(; *string != '\0'; string++){
        hash = (a * hash + *string) % bins;
    }
    return hash;
}

int hash_search_or_insert(char *string)
{
    int this_string_hash = hash(string, 65536);
    int i = 0;
    for(i = this_string_hash; i < 65536; ++i){
        if(hash_map[i] == NULL){
            //printf("%s\n", string); 
            hash_map[i] = strdup(string);
            return i;
        }else{
            if(strcmp(string, hash_map[i]) == 0){
                return i;
            }else{
                i = (i + 1) % 65536;
            }
        }
    }
    return -1;
}

tree *rotate_right(tree *head)
{ 
    tree *x = head->left; 
    head->left = x->right; 
    x->right = head;
    return x; 
}

tree *rotate_left(tree *head)
{ 
    tree *x = head->right; 
    head->right = x->left; 
    x->left = head;
    return x; 
}

redblack_tree *rb_rotate_right(redblack_tree *head)
{ 
    redblack_tree *x = head->left; 
    head->left = x->right; 
    x->right = head;
    return x; 
}

redblack_tree *rb_rotate_left(redblack_tree *head)
{ 
    redblack_tree *x = head->right; 
    head->right = x->left; 
    x->left = head;
    return x; 
}

tree *splay(tree *head, char *item)
{ 
    if (head == NULL){
        tree *new = malloc(sizeof(tree));
        new->left = NULL;
        new->right = NULL;
        new->item = strdup(item);
        return new;
    };

    if (strcmp(item, head->item) < 0){
        if (head->left == NULL){
            tree *new = malloc(sizeof(tree));
            new->left = NULL;
            new->right = head;
            new->item = strdup(item);
            return new;                
        }
        if (strcmp(item, head->left->item) < 0)
        { 
            head->left->left = splay(head->left->left, item); 
            head = rotate_right(head); 
        } else { 
            head->left->right = splay(head->left->right, item); 
            head->left = rotate_left(head->left); 
        }
        return rotate_right(head);
    } else {
        if (head->right == NULL){
            tree *new = malloc(sizeof(tree));
            new->left = head;
            new->right = NULL;
            new->item = strdup(item);
            return new;                            
        }
        if (strcmp(head->right->item, item) < 0){ 
            head->right->right = splay(head->right->right, item); 
            head = rotate_left(head); 
        } else { 
            head->right->left = splay(head->right->left, item); 
            head->right = rotate_right(head->right); 
        }
        return rotate_left(head);
    }
}

void insert_splay_tree(tree **head, char *item)
{ 
    *head = splay(*head, item); 
}

redblack_tree *rb_search_tree(redblack_tree *l, char *x)
{
    if (l == NULL) return(NULL);

    int strcmp_result = strcmp(x, l->item);

    if(strcmp_result == 0){
        return(l);
    }else if(strcmp_result < 0){
        return(rb_search_tree(l->left, x));
    }else{
        return(rb_search_tree(l->right, x));
    }
}

redblack_tree *redblack_insert(redblack_tree *head, char *item, bool change)
{
    if(head == NULL){
        redblack_tree *new = malloc(sizeof(redblack_tree));
        new->left = NULL;
        new->right = NULL;
        new->item = strdup(item);
        new->red = true;
        return new;
    }
    if((head->left != NULL) && (head->left->red) && (head->right != NULL) && (head->right->red)){
        head->red = true;
        head->left->red = false;
        head->right->red = false;
    }
    if(strcmp(item, head->item) < 0){
        head->left = redblack_insert(head->left, item, false);
        if(head->red && (head->left != NULL) && head->left->red && change){
            head = rb_rotate_right(head);
        }
        if((head->left != NULL) && head->left->red && (head->left->left != NULL) && head->left->left->red){
            head = rb_rotate_right(head);
            head->red = false;
            head->right->red = true;
        }
    }else{
        head->right = redblack_insert(head->right, item, true);
        if(head->red && (head->right != NULL) && head->right->red && !change){
            head = rb_rotate_left(head);
        }
        if((head->right != NULL) && head->right->red && (head->right->right != NULL) && head->right->right->red){
            head = rb_rotate_left(head);
            head->red = false;
            head->left->red = true;
        }
    }
    return head;    
}

void rb_insert(redblack_tree **head, char *item)
{
    *head = redblack_insert(*head, item, false);
    (*head)->red = false;
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
        //printf("%s\n", string);
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
            //printf("%s\n", word);
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
        //printf("%s\n", string);
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
            //printf("%s\n", word);
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

void add_or_skip_string_splay_bst(tree **l, char *string)
{
    if(search_tree(*l, string) == NULL){
        //printf("%s\n", string);
        insert_splay_tree(l, string);
    }
}

void parse_text_dictionary_splay_bst(char *text)
{
    tree *dictionary = malloc(sizeof(tree));
    char *word;
    bool first_time = true;

    for(word = strtok(text, " "); word; word = strtok(NULL, " ")){
        if(first_time){
            //printf("%s\n", word);
            dictionary->item = strdup(word);
            dictionary->left = NULL;
            dictionary->right = NULL;    
            dictionary->parent = NULL;
            first_time = false;
        }else{
            add_or_skip_string_splay_bst(&dictionary, word);
        }
    }    
    //traverse_tree_print(dictionary);
}

void add_or_skip_string_rb_bst(redblack_tree **l, char *string)
{
    if(rb_search_tree(*l, string) == NULL){
        //printf("%s\n", string);
        rb_insert(l, string);
    }
}

void parse_text_dictionary_rb_bst(char *text)
{
    redblack_tree *dictionary = malloc(sizeof(redblack_tree));
    char *word;
    bool first_time = true;

    for(word = strtok(text, " "); word; word = strtok(NULL, " ")){
        if(first_time){
            //printf("%s\n", word);
            dictionary->item = strdup(word);
            dictionary->left = NULL;
            dictionary->right = NULL;    
            dictionary->parent = NULL;
            dictionary->red = false;
            first_time = false;
        }else{
            add_or_skip_string_rb_bst(&dictionary, word);
        }
    }    
    //traverse_tree_print(dictionary);
}

void add_or_skip_string_hash(char *string)
{
    hash_search_or_insert(string);
}

void parse_text_dictionary_hash(char *text)
{
    int i;
    char *word;
    for(i = 0; i < 65536; ++i){
        hash_map[i] = NULL;
    }
    for(word = strtok(text, " "); word; word = strtok(NULL, " ")){
        add_or_skip_string_hash(word);
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
    double list_time_used;
    double bst_time_used;
    double splay_bst_time_used;
    double redblack_bst_time_used;
    double hash_time_used;
    
    char *file_text = read_file("poe-narrative-695.txt"); 
    start = clock();
    parse_text_dictionary_list(file_text);
    end = clock();
    list_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    file_text = read_file("poe-narrative-695.txt"); 
    start = clock();
    parse_text_dictionary_bst(file_text);
    end = clock();
    bst_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    file_text = read_file("poe-narrative-695.txt"); 
    start = clock();
    parse_text_dictionary_splay_bst(file_text);
    end = clock();
    splay_bst_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    file_text = read_file("poe-narrative-695.txt"); 
    start = clock();
    parse_text_dictionary_rb_bst(file_text);
    end = clock();
    redblack_bst_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    file_text = read_file("poe-narrative-695.txt"); 
    start = clock();
    parse_text_dictionary_hash(file_text);
    end = clock();
    hash_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Dictionary performance\nLinked-list time: %f\nBST time: %f\nSplay BST time: %f\nRed-black BST time: %f\nHash (linear-probing) time: %f\n",
           list_time_used, bst_time_used, splay_bst_time_used, redblack_bst_time_used, hash_time_used);

    return 0;
}
