/******************************************************************************
filename    scantext.c
author      Luke Tseng
DP email    luke.tseng@digipen.edu
course      CS120
section     B
assignment  5
due date    11/20/2021

Brief Description:
  This program has functions to read a string.
******************************************************************************/
static int mystrlen(const char *string)
{
  int length = 0; /* The number of characters in the string */
  
    /* Count the characters (not including the NUL) */  
  while (*string++)
    length++;

  return length;
}
/*Given a string, count the number of tabs in the string and 
return the count*/
int count_tabs(const char *string) {
  int count = 0;
  int length = mystrlen(string);
  int i;
  for (i = 0; i <length;++i) {
    if(*(string+i) == '\t') {
      ++count;
    }
  }
  return count;
}
/*Given a string, substitute old_char with new_char.
Returns the number of substitutions*/
int substitute_char(char *string, char old_char, char new_char) {
  int count = 0;
  int i;
  int length = mystrlen(string);
  for (i = 0; i <length;++i) {
    if(*(string+i) == old_char) {
      *(string+i) = new_char;
      ++count;
    }
  }
  return count;
}
/*Given a string and a tabsize, calculate the length of the string
and the print length of the string. The print length is the number
of characters that will be required to display the string after the
tabs are expanded into tabsize spaces.*/
void calculate_lengths(const char *string, int tabsize, int *string_length, 
                       int *display_length) {
int length = mystrlen(string);
int stringCount = 0;
int displayCount = 0;
int i;
for (i = 0;i<length;++i) {
  if(*(string+i) == '\t') {
    ++stringCount;
    displayCount += tabsize;
  } else {
    ++stringCount;
    ++displayCount;
  }
}
  *string_length = stringCount;
  *display_length = displayCount;  
}
/*Given a string, count the number of words and return the count.
Words in a string are delimited by whitespace characters which are
any of the following: space, newline, tab. The sample driver shows
numerous examples.*/
int count_words(const char *string) {
  int length = mystrlen(string);
  int count = 0;
  int wordSize = 0;
  int i;
  for(i = 0;i<length;++i) {
    if(*(string+i) == '\t' || *(string+i) == '\n' ||*(string+i) == ' ') {
      if(wordSize > 0) {
        ++count;
      }
      wordSize = 0;
    } else {
      ++wordSize;
    }
  }
  return count;
}