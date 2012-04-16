# include "dsp.h"
#include <stdlib.h>
#include <stdio.h>

int lvsepbas(fname1, fname2, num) 
  char * fname1, * fname2;
  int num;
{
  FILE * fi, *fo;
  char * buffer, *linha, *ap;
  int n = 2502, cont = 0, qtos;
     
  fi = fopen (fname1, "r");
  if (!fi) {
    fprintf (stderr, "vdspfact () : fopen failed\n");
    return 0;
  }
  
  fo = fopen (fname2, "w");
  if (!fo) {
    fprintf (stderr, "vdspfact () : fopen failed\n");
    return 0;
  }

  linha  = (char *) node_allocation (sizeof (char) * n);
  buffer = (char *) node_allocation (sizeof (char) * n);
  
  while ( !feof (fi)) {
    fgets(linha, n-2, fi);
    strcpy (buffer, linha);
    buffer[9] = 0;
    if (!strcmp (buffer, "MATRIZ C:")) {
      /* peguei uma linha a proxima linha e' a matriz C*/
      ++cont;
      if (cont == num) {
        fgets (linha, n-2, fi); /* linha que contem a matriz C */
        fgets (linha, n-2, fi); /* a linha que contem o header a base */
        ap = linha + strlen (linha);
	while (! isdigit (*ap)) ap--;
        while (isdigit (*ap)) ap--;
        ap++; 
        qtos = (int) strtol(ap, (char  **)NULL, 10);
        fprintf (fo, "%s", linha);
        qtos += 2; /* devido a palavra BASE, ao WINSPEC e a janela */
        while (qtos) {
          fgets (linha, n-2, fi);
          fprintf (fo, "%s", linha);
          qtos--;
        }
	free (linha);
        free (buffer);
        fclose (fi);
        fclose (fo);
        return 1;
        
      }
    }
  }
  free (linha);
  free (buffer);
  fclose (fi);
  fclose (fo);
  return 0;
}


int main (int argc, char * * argv) {
  if(!lvsepbas(argv[1], argv[2], atoi (argv[3]))) {
       fprintf(stderr, "vsepbas : lvsepbas() failed\n") ;
       return 1 ;
    }
    return 0;
}    

