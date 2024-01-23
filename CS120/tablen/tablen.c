#include <stdio.h> /* FILE */
#define MAX_LINE_LENGTH 115

static int tabs(const char *filename) {
  int i;
  int lineCount = 1;
  int lineTabs = 0;
  int count = 0;
  char line[MAX_LINE_LENGTH];
  FILE *f;
  f = fopen(filename,"r");
  while (!feof(f)) {
    if (fgets(line, MAX_LINE_LENGTH, f)) {
      lineTabs = 0;
      for(i = 0;i<MAX_LINE_LENGTH;++i) {
        if(line[i] == '\n') {
            line[i] = '\0';
            i = MAX_LINE_LENGTH;
        } else if(line[i] == '\t') {
          ++count;
          ++lineTabs;
        }
      }
      if(lineTabs > 0) {
        printf("#%3i: (tabs:%2i) |%s|\n", lineCount, lineTabs,line);
      }
      ++lineCount;
    }
  }
  return count;
}

static int lines(const char *filename) {
  int longestLine = 0;
  int longestLineCount = 0;
  int lineCount = 1;
  int lineLength;
  int i;
  int count = 0;
  char line[MAX_LINE_LENGTH];
  FILE *f;
  f = fopen(filename,"r");
  while (!feof(f)) {
    if (fgets(line, MAX_LINE_LENGTH, f)) {
      lineLength = 0;
      for(i = 0;i<MAX_LINE_LENGTH;++i) {
        ++lineLength;
        if(line[i] == '\n') {
          line[i] = '\0';
          --lineLength;
          i = MAX_LINE_LENGTH;
        }
      }
      if(lineLength > 80) {
        if(lineLength > longestLine) {
          longestLine = lineLength;
          longestLineCount = lineCount;
        }
        printf("#%3i: (%3i) |%s|\n", lineCount, lineLength, line);
        ++count;
      }
      ++lineCount;
    }
  }
  printf("***** LINE TOO LONG *****: %d lines longer than 80.", count);
  printf(" Longest line is %d at %d chars.\n", longestLineCount, longestLine);
  return count;
}

void tablen(const char *filename) {
  int tab = 0;
    FILE *f;
    f = fopen(filename,"r");
    if (f) {
      tab = tabs(filename);
      printf("*****  TABS IN USE  *****: Number of tabs in the file is %d.\n",
      tab);
      lines(filename);
    } else {
      printf("Can't open inputxxx.txt for read.");
    }
    return;
}