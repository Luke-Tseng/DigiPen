#include <stdio.h> /* printf */

/* Prototype from tablen.c */
void tablen(const char *filename);

int main(int argc, char **argv)
{
    /* Must be given a filename */
  if (argc < 2)
  {
    printf("Usage: tablen filename\n");
    printf("where: filename - file to process.\n");
    return 1;
  }

    /* This is the only "public" function you are writing. */
  tablen(argv[1]);

  return 0;
}