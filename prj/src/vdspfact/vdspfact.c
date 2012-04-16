
#include "dsp.h"

int lvdspfact(fname1, fname2) 
  char * fname1, * fname2;
{
  FILE * fi, *fo, *temp;
  char * buffer, *linha, *ap;
  int n = 2502, cont = 0, qtos;
  int dimC, tam = 1, tam_ant = 1;
  int * vetor = (int *) node_allocation (sizeof (int) * 1);
  int st;
     
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
      fprintf (fo, "(%2d) %s\n", ++cont, linha);
      ap = linha + 10; dimC = (int) strtol(ap, (char  **)NULL, 10);
      tam = (dimC*dimC+31)/32;
      if (tam > tam_ant) {
        free (vetor);
        vetor = (int *) node_allocation (sizeof (int) * tam);
        tam_ant = tam;
      }
      fgets (linha, n-2, fi);
      str2vetor (linha, dimC, vetor);
      imp_vet_mat (&fo, vetor, dimC); 
      fprintf (fo,"\n");
    }
    else {
      buffer[4] = 0;
      if (!strcmp (buffer, "BASE")) {
        ap = linha + strlen (linha);
	while (! isdigit (*ap)) ap--;
        while (isdigit (*ap)) ap--;
        ap++; 
        qtos = (int) strtol(ap, (char  **)NULL, 10);
        temp = fopen ("vdspfact.tmp", "w");
        if (!temp) {
          fprintf (stderr, "vdspfact () : fopen failed\n");
          return 0;
        }
        fprintf (temp, "%s", linha);
        qtos += 2; /* devido a palavra BASE, ao WINSPEC e a janela */
        while (qtos) {
          fgets (linha, n-2, fi);
          fprintf (temp, "%s", linha);
          qtos--;
        }
        fclose (temp);
        imp_base_mat ("vdspfact.tmp", &fo, cont);
        unlink ("vdspfact.tmp");
      }
    }
  }
  free (linha);
  free (buffer);
  free (vetor);
  fclose (fi);
  fclose (fo);
  return 1;
}

int main (int argc, char * * argv) {
  if(!lvdspfact(argv[1], argv[2])) {
       fprintf(stderr, "vdspfact : lvdspfact () failed\n") ;
       return 1 ;
    }
  return 0;
}





