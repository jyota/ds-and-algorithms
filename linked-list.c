#include <stdio.h>
#include <stdlib.h>

typedef enum { false, true } bool;

// Struct lifted from Algorithm Design Manual
typedef struct list {
  int item;
  struct list *next;
} list;

// Exercise 3.38 from Sedgewick's Algorithms in C 3rd ed
list *copy_linked_list(list *l)
{
  if(l == NULL) return(NULL);
  list *new_list = malloc(sizeof(list));;
  new_list->item = l->item;
  new_list->next = copy_linked_list(l->next);
  return(new_list);
}

// Exercise 3.24 from Sedgewick's Algorithms in C 3rd edition
void count_nodes_circular_list(list *l)
{
   int i = 1;
   list *current = l->next;
   if(current == NULL){
     printf("Empty list, no nodes to count.\n");
   }else{
    while(current != NULL){
      if(current == l){
        printf("Number of nodes: %d\n", i);
        break;
      }else{
        ++i;      
        current = current->next;
      }
    }
  }
}

// Exercise 3.25 from Sedgwick's Algorithm's in C 3rd edition
// below does the 'between' count exclusive of the nodes.
void count_nodes_between(list *node_t, list *node_x)
{
  int i = 0;
  list *current = node_t;
  if(current == NULL || current->next == NULL){
    printf("Empty or one item list, can't count between t and x.\b");
  }else{
    current = current->next;
    while(current != NULL){
      if(current == node_x){
        printf("Number of nodes between x and t: %d\n", i);
        break;
      }
      ++i;
      current = current->next;
    }
  }
}

// Exercise 3.27 from Sedgwick's Algorithm's in C 3rd edition
list *move_after_t_to_after_x(list *t, list *x)
{
  if(t == NULL || t->next == NULL || x == NULL || x->next == NULL){
    printf("t and x must not be null and must be able to continue past them.\n");
    return(t);
  }else{
    list *item_to_move = t->next;
    list *current = t;
    t->next = t->next->next;
    while(current != NULL){
      if(current == x){
        item_to_move->next = x->next;
        x->next = item_to_move;
        break;
      }
      current = current->next;
    }
    return(current);
  }
}

// Algorithm design manual problem 3.23 
// (my recursive implementation)
// this is kind of 'iterative' masquerading as recursive, 
// see next function for 'pure recursive'
list *reverse_list_recursive(list *l, list *previous)
{
  list *next;
  if(l == NULL) return(previous);

  next = l->next;
  l->next = previous;
  return(reverse_list_recursive(next, l));
}

// pure recursive form, lifted from:
// https://stackoverflow.com/questions/14080758/reversing-a-linkedlist-recursively-in-c
list *reverse_pure_recursive(list *l)
{
  if(l != NULL && l->next == NULL) return(l);

  list *working_list = reverse_pure_recursive(l->next);
  l->next->next = l;
  l->next = NULL;
  return(working_list);
}


// Algorithm Design Manual problem 3.20
// in my slightly modified implementation below (compared to the solutions wiki), 
// if there is an even number of items,
// return the item just before the middle instead of after the middle.
list *find_middle(list *l)
{
  int i = 0;
  list *middle;
  list *current;
  if(l == NULL || l->next == NULL){
    return(l);
  }
  middle = l;
  current = l;
  while(current != NULL){
    current = current->next;
    ++i;
    if(i % 2 == 0 && current != NULL){
      middle = middle->next;
    }
  }
  return(middle);
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

// Exercise 3.34 - Sedgewick's Algorithms in C 
// Move largest item on list to be the final node on the list
// This approach returns pointer to new head.
list *move_largest_to_final(list *l)
{
  list *current = l;
  list *head = l;
  list *largest = NULL;
  list *last = NULL;
  list *prev_to_largest = NULL;
  list *largest_next = NULL;

  if(current == NULL || current->next == NULL){
    return l;
  }else{
    int largest_value_seen = -1;
    while(current != NULL){
      if(current->item > largest_value_seen){
        largest_value_seen = current->item;
        largest = current;
        if(last != NULL){
          prev_to_largest = last;
          largest_next = largest->next;
        }
      }
      last = current;    
      current = current->next;
    }
    if(largest == last || largest == NULL){
      return l;
    }else{
      if(prev_to_largest != NULL){
        prev_to_largest->next = largest_next;
      }else{
        l = l->next;
      }
      last->next = largest;
      largest->next = NULL;
      return l;
    }
  }
}

// Exercise 3.35 - Sedgewick's Algorithms in C 
// Move smallest item on list to be the first node on the list
// This approach returns pointer to new head.
list *move_smallest_to_first(list *l)
{
  list *head = l;
  list *current = l;
  list *last;
  list *prev_to_smallest;
  list *smallest;
  list *smallest_next;

  if(current == NULL || current->next == NULL){
    return head;
  }else{
    int smallest_value_seen = 999999;    
    while(current != NULL){
      if(current->item < smallest_value_seen){
        smallest_value_seen = current->item;
        smallest = current;
        if(last != NULL){
          prev_to_smallest = last;
          smallest_next = smallest->next; 
        }
      }
      last = current;
      current = current->next;
    }
    if(smallest == head || smallest == NULL){
      return head;
    }else{
      if(prev_to_smallest != NULL){
        prev_to_smallest->next = smallest_next;
      }
      smallest->next = head;
      return smallest;
    }
  }
}

// Exercise 3.39 - Sedgewick's Algorithms in C 3rd ed
void filter_list(list **l, bool (*func)(int))
{
  list *current, *pred;
  
    if(!l || !(*l)){
      // do nothing
    }else{
      current = *l;
      pred = NULL;
  
      while(current != NULL){
          if(!func(current->item)){
            if(pred == NULL){
              *l = current->next;
              pred = NULL;
            }else{
              pred->next = current->next;
            }
          }else{
            pred = current;            
          }
          current = current->next;
      }
    }
}


bool filter_func(int value){
  if(value > 2){
    return true;
  }else{
    return false;
  }
}

// Traverse and print for n times (for circular lists)
void print_traverse_circular(list **l, int n)
{
  list *current = *l;
  for(int i = 0; i < n; ++i){
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

void linked_list_tests()
{
  list *mylist = malloc(sizeof(list));
  mylist->item = 4;
  mylist->next = NULL;
  insert_list(&mylist, 3);
  insert_list(&mylist, 2);  
  insert_list(&mylist, 1);

  reverse_list(&mylist);
  print_traverse(&mylist);

  mylist = reverse_list_recursive(mylist, NULL);
  print_traverse(&mylist);

  mylist = reverse_pure_recursive(mylist);
  print_traverse(&mylist);

  list *mymiddle = find_middle(mylist);
  print_traverse(&mymiddle);

  // check for a 5 item list
  mylist->item = 1;
  mylist->next = NULL;
  insert_list(&mylist, 4);
  insert_list(&mylist, 3);  
  insert_list(&mylist, 5);  
  insert_list(&mylist, 2);  
  list *mylistcopy = copy_linked_list(mylist);
  list *mylistcopy2 = copy_linked_list(mylist);
  

  print_traverse(&mylist);
  printf("Filter list to > 2: ");
  filter_list(&mylist, &filter_func);
  print_traverse(&mylist);

  printf("Move largest to final: ");
  list *checklargest = move_largest_to_final(mylist);
  print_traverse(&checklargest);  
  printf("Move smallest to first: ");
  list *newhead = move_smallest_to_first(mylistcopy);
  print_traverse(&newhead);

  mymiddle = find_middle(mylistcopy);
  print_traverse(&mymiddle);

  // circular list exercises.

  mylistcopy2->next->next->next->next->next = mylistcopy2;
  count_nodes_circular_list(mylistcopy2);
  count_nodes_between(mylistcopy2->next->next, mylistcopy2->next->next->next->next->next);

  move_after_t_to_after_x(mylistcopy2->next, mylistcopy2->next->next->next->next->next);
  print_traverse_circular(&mylistcopy2, 5);
  printf("\n");
}

// Sedgewick Algorithms in C exercise 3.33 solution
void josephus_problem_linked_list_array(int N, int M)
{
  int i;
  int next[N];

  for(i = 0; i < N; ++i){
    next[i] = (i + 1) == N ? 0 : (i + 1);
  }

  int next_item = 0;
  while(next[next_item] != next[next[next_item]]){
    int prev_item = N-1;    
    for(i = 1; i < M; ++i){
      prev_item = next_item;
      next_item = next[next_item];
    }
    next[prev_item] = next[next_item];
    next_item = next[next_item];
  }
  printf("Josephus linked list array result: %d\n", next_item + 1);
}

void josephus_problem_linked_list(int N, int M)
{
  int i;
  list *t = malloc(sizeof(list));
  list *x = t;

  t->item = 1; 
  t->next = t;

  for(i = 2; i <= N; ++i){
    x->next = malloc(sizeof(list));
    x = x->next;
    x->item = i;
    x->next = t;
  }

  while(x != x->next){
    for(i = 1; i < M; ++i){
      x = x->next;
    }
    x->next = x->next->next;
  }
  printf("Josephus linked list result: %d\n", x->item);
}

int main(int argc, char *argv[])
{
  linked_list_tests();
  //int N = atoi(argv[1]);
  //int M = atoi(argv[2]);
  //josephus_problem_linked_list(N, M);
  //josephus_problem_linked_list_array(N, M);
  
  return(0); 
}

