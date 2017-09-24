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

  while(i < str_len && i < j){
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

// Exercise 3.58 from Sedgewick's Algorithms in C (3rd ed)
char *mystrcpy(char *s1)
{
    char *working_string;
    size_t i;
    size_t s1_length = strlen(s1);
    working_string = malloc(s1_length * sizeof(char));
    for(i = 0; i < s1_length; ++i){
        working_string[i] = s1[i];
    }

    return working_string;
}

char *mystrcat(char *s1, char *s2)
{
    char *working_string;
    size_t i;
    size_t s1_length = strlen(s1);
    size_t s2_length = strlen(s2);
    size_t combined_length = s1_length + s2_length;
    working_string = malloc(combined_length * sizeof(char));
    for(i = 0; i < combined_length; ++i){
        char char_to_copy;
        if(i < s1_length){
            char_to_copy = s1[i];
        }else{
            char_to_copy = s2[i - s1_length];
        }
        working_string[i] = char_to_copy;
    }

    return working_string;
}

// Exercise 3.59 from Sedgewick's Algorithms in C (3rd ed)
// 2nd arg is a string of strings to check for in 1st arg string
// (each string in 2nd arg should be space delimited)
void find_substrings(char *parent_string, char *search_strings)
{
    char *word;
    size_t i;
    size_t j;
    size_t parent_strlen = strlen(parent_string);
    size_t word_strlen;

    for(word = strtok(search_strings, " "); word; word = strtok(NULL, " ")){
        word_strlen = strlen(word);
        for(i = 0; i < parent_strlen; ++i){
            if(parent_string[i] == word[j]){
                if(j == word_strlen - 1){
                    printf("%s\n", word);
                }else{
                    ++j;
                }
            }else{
                j = 0;
            }
        }
    }
}

int main(int argc, char *argv[])
{  
  char string[] = "These are a few of my favorite things.";
  char test_concat_string[] = " Except powerWheels.";
  bool is_a_palindrome;

  printf("Count of char occurence for: %s\n", string);
  count_print_char_occurrence(string);

  //char palindrome_string[] = "  iT opiNonAv evAnoNip oTi";
  //char palindrome_string[] = "if i had a hifi";
  char palindrome_string[] = "J JJ ifi            JJ    J";
  printf("Palindrome check for: %s\n", palindrome_string);
  is_a_palindrome = is_palindrome(palindrome_string);
  printf("%s\n", is_a_palindrome ? "Yes" : "No");
  printf("mystrcpy for: %s\n", string);
  char *copied_string = mystrcpy(string);
  printf("%s\n", copied_string);
  printf("mystrcat for: %s + %s\n", string, test_concat_string);
  char *concat_string = mystrcat(string, test_concat_string);
  printf("%s\n", concat_string);
  printf("----------\nEnter string to search within: ");
  char parent_string[100];
  fgets(parent_string, sizeof(parent_string), stdin);
  parent_string[strcspn(parent_string, "\n")] = 0;
  printf("Enter search terms (separated by spaces): ");
  char search_terms[100];
  fgets(search_terms, sizeof(search_terms), stdin);
  search_terms[strcspn(search_terms, "\n")] = 0;
  printf("Substring search for: %s\n(search terms: %s)\n", parent_string, search_terms);
  find_substrings(parent_string, search_terms);
  return 0;
}