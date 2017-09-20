#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef enum { false, true } bool;

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

// Exercise 3.57 from Sedgewick's Algorithms in C 3rd ed.
bool is_palindrome(char *string)
{
  int i;
  int str_len = strlen(string) - 1;
  int j = str_len;
  bool seen_a_char = false;

  if(str_len == 0){
      printf("Error: can't test palindrome of zero length string.\n");
      return false;
  }

  while(i < str_len){
    while(string[i] == ' ' && i < str_len){
        ++i;
    }
    while(string[j] == ' ' && j > 0){ 
        --j;
    }
    
    if((i == str_len && string[i] == ' ') || (j == 0 && string[j] == ' ')){
        // do nothing.
    }else if(string[i] != string[j]){
        return false;
    }else{
        seen_a_char = true;
        if(i < str_len) ++i;
        if(j > 0) --j;
    }
  }
  if(seen_a_char){
      return true;
  }else{
      printf("Error: can't check for palindrome in a string with only spaces.\n");
      return false;
  }
}

int main(int argc, char *argv[])
{  
  char string[] = "These are a few of my favorite things.";
  bool is_a_palindrome;

  printf("Count of char occurence for: %s\n", string);
  count_print_char_occurrence(string);

  char palindrome_string[] = "  iT opiNonAv evAnoNip oTi";
  printf("Palindrome check for: %s\n", palindrome_string);
  is_a_palindrome = is_palindrome(palindrome_string);
  printf("%s\n", is_a_palindrome ? "Yes" : "No");
  return 0;
}