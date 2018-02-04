#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "h2_2.txt"
int main(void)
{
  // Print something to standard out
  printf("This is an interesting string.\n");

  // Create a file for write
  FILE * pfile = fopen(FILE_NAME,"w+");
  if (pfile==NULL)
  {
    perror("Error opening file");
    exit(-2);
  }

  // Write a single character to the file then close the file
  putc('P', pfile);
  fclose(pfile);

  // Open the file again in append mode
  pfile = fopen(FILE_NAME, "a+");

  // Get a string input from command line
  char * str = (char *)malloc(sizeof(char)*256);
  scanf("%[^\n]s", str);

  // Write the string to the file
  fprintf(pfile, "%s", str);

  // Flush file output
  fflush(pfile);

  // Close the file
  fclose(pfile);

  // Open the file for reading
  pfile = fopen(FILE_NAME, "r");
  if(pfile==NULL)
  {
    perror("Error opening file");
    exit(-2);
  }

  // Read a single character from the file
  char c = getc(pfile);

  // Read a string from the file then close the file
  fgets(str, strlen(str), pfile);
  fclose(pfile);

  // Free the memory
  free(str);
  return 0;
}
