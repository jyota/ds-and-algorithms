#include <stdio.h>
#include <stdlib.h>

typedef enum { false, true } bool;

// Struct lifted from Algorithm Design Manual
typedef struct list {
  int item;
  struct list *next;
} list;

// My recursive singly linked list reverse implementation
list *reverse_list_recursive(list *l, list *previous)
{
  list *next;
  if(l == NULL) return(previous);

  next = l->next;
  l->next = previous;
  return(reverse_list_recursive(next, l));
}

// My traverse singly-linked list and print implementation
void print_traverse(list **l)
{
  list *current = *l;
  while(current != NULL){
    printf("%d ", current->item);
    current = current->next;
  }
  printf("\n");  
}

// Lifted from Algorithm Design Manual
list *search_list(list *l, int x)
{
  if(l == NULL) return(NULL);

  if(l->item == x)
    return(l);
  else
    return(search_list(l->next, x));
}

// Lifted from Algorithm Design Manual
void insert_list(list **l, int x)
{
  list *p;

  p = malloc(sizeof(list));
  p->item = x;
  p->next = *l;
  *l = p;
}

// Lifted from Algorithm Design Manual
list *predecessor_list(list *l, int x)
{
  if((l == NULL) || (l->next == NULL)){
    printf("Error: predecessor sought on null list.\n");
    return(NULL);
  }

  if((l->next)->item == x)
    return(l);
  else
    return(predecessor_list(l->next, x));
}

// Lifted from Algorithm Design Manual
void delete_list(list **l, int x)
{
  list *p;
  list *pred;
  list *search_list(), *predecessor_list();

  p = search_list(*l, x);
  if(p != NULL){
    pred = predecessor_list(*l, x);
    if(pred == NULL)
      *l = p->next;
    else
      pred->next = p->next;

    free(p);
  }
}

// Lifted from Algorithm Design Manual solution
void reverse_list(list **l)
{
  list *current, *previous, *next;

  if(!l || !(*l)){
    // do nothing
  }else{

    current = *l;
    previous = NULL;
    next = NULL;

    while(current != NULL){
      next = current->next;
      current->next = previous;
      previous = current;
      current = next;
    }
    *l = previous;
  }
}

int main()
{
  list *mylist;
  mylist->item = 4;
  mylist->next = NULL;
  insert_list(&mylist, 3);
  insert_list(&mylist, 2);  
  insert_list(&mylist, 1);

  mylist = reverse_list_recursive(mylist, NULL);
  print_traverse(&mylist);

  reverse_list(&mylist);
  print_traverse(&mylist);


  return 0; 
}

