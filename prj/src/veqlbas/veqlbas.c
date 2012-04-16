# include <stdio.h>
# include <stdlib.h>
# include "pac_def.h"
# include "pac_win.h"
# include "pac_bas.h"

int lveqlbas (char * fname1, char * fname2) {
   win_struc     *win1, *win2;
   trv_struc     base1, base2;
   int           nvars;
   int           i, j, k, ind1, ind2, total;
   int           st, n4bytes, tem, igual;

   if((st = read_itrv (fname1, &base1)) == 0) {
      fprintf(stderr, "lveqlbas : read_itrv() base 1 failed\n") ;
      return 0;
   }

   if((st = read_itrv (fname2, &base2)) == 0) {
      fprintf(stderr, "lveqlbas : read_itrv () base 2 failed\n") ;
      return 0;
   }

   // verificando se o numero de elementos nas duas bases
   // sao iguais
   // printf ("%d %d\n", base1.total, base2.total);
   if ((total=base1.total) != base2.total) {
     fprintf (stderr, "Distinct Basis because of number of total intervals\n");
     return 2;
   }

   // verificando se a janelas sao iguais
   win1 = &base1.win;  win2 = &base2.win;

   if ( (win1->hsize != win2->hsize) ||
        (win1->wsize != win2->wsize) ||
        (win1->size  != win2->size) ) {
     fprintf (stderr, "Distinct Basis because of window specification\n");
     return 2;
   }

   n4bytes = ((win1->hsize * win1->wsize) + NB1) / NB ;
   /* data window specification */
   for (j=0; j<n4bytes; j++) {
     if (win1->data[j] != win2->data[j]) {
       fprintf (stderr, "Distinct Basis because of data window specification\n");
       return 2;
     }
   }

  if (  (base1.size  != base2.size) ||
        (base1.total != base2.total) ||
        (base1.nv    != base2.nv) ||
        (base1.nb    != base2.nb) ) {
     fprintf (stderr, "Distinct Basis because of basis specification\n");
     return 2;
  }


   for (i=0; i<base1.total; i++) {
      ind1 = base1.ind[i];

      // verificar se existe em base2 tem este elemento ...
      tem = 0;
      for (k=0; k<base2.total; k++) {
        ind2 = base2.ind[k];
        igual = 1;
        for (j=0; j<base2.nb; j++) {
          if ( (base1.A[ind1+j] != base2.A[ind2+j]) ||
               (base1.B[ind1+j] != base2.B[ind2+j]) ) {
            igual = 0;
            break;
          }
        }
        if (igual) {tem = 1; break;}
      }
      if (!tem) {
        fprintf (stderr, "Distinct Basis because of data basis specification\n");
        return 2;
      }
   }

   free_itrv (&base1) ;   free_itrv (&base2) ;
   fprintf (stderr, "The basis are equal\n");
   return 1;
}

int main (int argc, char * * argv) {
  if (!lveqlbas(argv[1], argv[2])) {
       fprintf(stderr, "veqlbas : lveqlbas() failed\n") ;
       return 1;
  }
  return 0;
}
