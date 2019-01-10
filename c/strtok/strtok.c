#include "string.h"
#include "stdlib.h"
#include "stdio.h"
 
int main()
{
  char str[]="00:22:33:4B:55:5A";
  char *delim = ":";
  char * pch;

  char *array[6];
  int i = 0;

  printf ("Splitting string \"%s\" into tokens:\n",str);

  pch = strtok(str,delim);
  while (pch != NULL) {
    printf ("%s\n",pch);
	array[i++] = pch;
    pch = strtok (NULL, delim);
  }      

  printf("Output ot Array\n");

  for (i = 0; i < 6; i++) {
	  printf("%s\n", array[i]);
  }

  return 0;
}
