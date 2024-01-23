/******************************************************************************
filename    spellcheck.c
author      Luke Tseng
DP email    luke.tseng@digipen.edu
course      CS120
section     B
assignment  6
due date    12/02/2021

Brief Description:
  Word functions from file
******************************************************************************/
#include <string.h> /* To be completed by the student       */
#include <stdio.h>  /* FILE (To be completed by the student */

/* FILE_OK, FILE_ERR_OPEN, WORD_OK, WORD_BAD, LONGEST_WORD */
#include "spellcheck.h"

char buffer[LONGEST_WORD]; /* To hold each line from the file */

/* Given a string, convert all lowercase letters to uppercase.
Returns a pointer to the first character of the string that was
passed in. */
char *mystrupr(char *string) {
  size_t i;
  for(i = 0; i< strlen(string); ++i) {
    if(*(string+i)>=97 && *(string+i)<= 122) {
      *(string+i) -= 32;
    }
  }
  return string;
}

/* Given the filename of a dictionary, count the number of
words that start with letter. If the file can't be opened,
return FILE_ERR_OPEN, otherwise return the number of
words that start with letter. */
int words_starting_with(const char *dictionary, char letter) {

  FILE *fp;
  int count = 0;
  fp = fopen(dictionary, "r");
  if(letter >=65 && letter <= 90) {
    letter += 32;
  }
  if (fp) {
    while (!feof(fp)) {
      if(fgets(buffer, LONGEST_WORD, fp)) {
        if(buffer[0] == letter) {
          ++count;
        }
      }
    }
    fclose(fp);
    return count;
  } else {
    return FILE_ERR_OPEN;
  }
}
/* Given the filename of a dictionary, count the number of
words that have length 1 to count and store them in the
array lengths at the appropriate index. If the file can't be
opened, return FILE_ERR_OPEN, otherwise return
FILE_OK */
int word_lengths(const char *dictionary, int lengths[], int count) {
  FILE *fp;
  fp = fopen(dictionary, "r");
  if (fp) {
    while (!feof(fp)) {
      if(fgets(buffer, LONGEST_WORD, fp)) {
        int length = strlen(buffer)-1;
        if(length >=1 && length <= count){
          ++lengths[length];
        }
      }
    }
    fclose(fp);
    return FILE_OK;
  } else {
    return FILE_ERR_OPEN;
  }
}

/* Given the filename of a dictionary, return some information
about it (using the DICTIONARY_INFO structure). If the
file can't be opened, return FILE_ERR_OPEN, otherwise
return FILE_OK. */
int info(const char *dictionary, struct DICTIONARY_INFO *dinfo) {
  FILE *fp;
  int words = 0;
  fp = fopen(dictionary, "r");
  if (fp) {
    if ( fgets(buffer, LONGEST_WORD, fp)) {
      dinfo->shortest = strlen(buffer)-1;
      dinfo->longest = strlen(buffer)-1;
      ++words;
    }
    while (!feof(fp)) {
      if(fgets(buffer, LONGEST_WORD, fp)) {
        int length = strlen(buffer)-1;
        if (length >= 1) {
          ++words;
          if(length < dinfo->shortest) {
            dinfo->shortest = length;
          }
          if(length > dinfo->longest) {
            dinfo->longest = length;
          }
        }
      }
    }
    fclose(fp);
    dinfo->count = words;
    return FILE_OK;
  } else {
    return FILE_ERR_OPEN;
  }
}

/* Given the filename of a dictionary and a word, lookup the
word in the dictionary. If the word was found, return
WORD_OK. If the word was not found, return WORD_BAD. If
the dictionary file can't be opened, return
FILE_ERR_OPEN. */
int spell_check(const char *dictionary, const char *word) {
  FILE *fp;
  size_t i;
  fp = fopen(dictionary, "r");
  if (fp) {
    while (!feof(fp)) {
      if (fgets(buffer, LONGEST_WORD, fp)) {
        for(i = 0;i<LONGEST_WORD;++i) {
          if(buffer[i] == '\n') {
            buffer[i] = '\0';
          }
        }
        for(i = 0;i<strlen(word);++i) {
          if(*(word+i) >=65 && *(word+i) <= 90) {
            buffer[i] -=32;
          }
        }
        if(strcmp(buffer,word) == 0) {
          return WORD_OK;
        }
      }
    }
    fclose(fp);
    return WORD_BAD;
  } else {
    return FILE_ERR_OPEN;
  }
}