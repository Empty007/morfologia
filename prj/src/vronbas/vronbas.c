#include <stdio.h>
#include <stdlib.h>
#include "pac_win.h"
#include "pac_bas.h"

int nina_bas2ron_bas (fname1, fname2, from, bck_str, left_str, right_str)
 char *fname1, *fname2;
 int from;
 char  *bck_str, *left_str, *right_str ;
{
   win_struc     *win;
   trv_struc     base ;
   int           nvars ;
   char          strA[2500] ;
   char          strB[2500] ;
   int           i, j, ind ;
   FILE          *fd ;
   int           start, end ;
   int           *B, *winidx ;
   char          back_car, left_car, right_car ;
   int           st ;

   st = set_cars(bck_str, left_str, right_str, &back_car, &left_car, &right_car) ;
   if(st == 0) {
      return(0) ;
   }  

   st = read_itrv(fname1, &base);
   if(st == 0) {
      fprintf(stderr, "nina_bas2ron_bas(): read_itrv() failed\n") ;
      return(0) ;
   }

   st = init_str(strA, 2500, &base.win, back_car, left_car, right_car) ;
   if(st == 0) {
      return(0) ;
   }
   st = init_str(strB, 2500, &base.win, back_car, left_car, right_car) ;
   if(st == 0) {
      return(0) ;
   }
   st = win_index(&base.win, &winidx) ;
   if(st == 0) {
      return(0) ;
   }
   
   fd = fopen(fname2, "w") ;
   if(fd == NULL) {
      fprintf(stderr, "nina_bas2ron_bas(): fopen() failed\n") ;
      return(0) ;
   }

   win = &base.win;

   fprintf(fd, "BASE %d %d\n", win->size, base.total); // no. de variaveis e no. de intervalos
   write_win2 (fd, win);
   fprintf(fd, "\n") ;

   start=1; end=base.total;

   B = (int *) malloc (sizeof(int)*base.nb);
   if(B==NULL) {
      fprintf (stderr, "nina_bas2ron_bas(): memory allocation error\n");
      return 0;
   }

   for(i=start-1; i<end; i++) {

      ind = base.ind[i] ;

      for(j=0; j<base.nb; j++) {
         B[j] = ~base.B[ind+j] ;
      }

      cfg2str(&base.A[ind], base.nv, winidx, win->hsize, win->wsize, strA, 2500) ;
      cfg2str(B, base.nv, winidx, win->hsize, win->wsize, strB, 2500) ;

      fprintf (fd, "%d\n\n", i+1);

      for(j=0; j<win->hsize/2; j++) {
         fprintf(fd, "%5s   %s\t", "", &strA[2*j*win->wsize]) ;
         fprintf(fd, "%5s   %s\t\n", "", &strB[2*j*win->wsize]) ;
      }

         fprintf(fd, "%5s   %s\t", "", &strA[2*(win->hsize/2)*win->wsize]) ;
         fprintf(fd, "%5s   %s\t\n", "", &strB[2*(win->hsize/2)*win->wsize]) ;

      for(j=win->hsize/2+1; j<win->hsize; j++) {
         fprintf(fd, "%5s   %s\t", "", &strA[2*j*win->wsize]) ;
         fprintf(fd, "%5s   %s\t\n", "", &strB[2*j*win->wsize]) ;
      }
      fprintf(fd, "\n") ;
   }

   fclose(fd) ;  
   free_itrv(&base) ;
   free (winidx);
   free(B) ;
   return 1;
}

int main (int argc, char * * argv) {
  if( !nina_bas2ron_bas (argv[1], argv[2], 1, "-", " ", " ") ) {
       fprintf(stderr, "vronbas : nina_bas2ron_bas() failed\n") ;
       return 1;
  } 
  return 0;
}
