#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Exercise 3.56 from Sedgewick's Algorithms in C 3rd ed.
void count_print_char_occurrence(char *string)
{
  int char_lookup[255] = {0}; 
  int i = 0;
  int current_lookup;
  int char_min_abs = abs(CHAR_MIN);

  while(string[i] != '\0'){
      current_lookup = char_min_abs + string[i];
      ++char_lookup[current_lookup];
      ++i;
  }
  for(i = 0; i < 255; ++i){
      if(char_lookup[i] != 0){
          printf("%c: %d\n", (char) (i - char_min_abs), char_lookup[i]);
      }
  }
}

int main(int argc, char *argv[])
{
    char string[] = "These are a few of my favorite things.";
    count_print_char_occurrence(string);
    return 0;
}