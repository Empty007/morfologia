#include <stdio.h>
#include <stdlib.h>
#include "pac_win.h"
#include "pac_bas.h"

int lvdspbas(fname1, fname2, from, to, bck_str, left_str, right_str)
 char *fname1, *fname2;
 int from, to;
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
      fprintf(stderr, "lvdspbas : read_itrv() failed\n") ;
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
      fprintf(stderr, "lvdspbas : fopen() failed\n") ;
      return(0) ;
   }

   fprintf(fd, "  VISUALIZATION OF BASE\n") ;
   fprintf(fd, "    Total of base elements = %d\n", base.total) ;
   fprintf(fd, "\n\n") ;

   /* 
    * if(from>to) {
    *   start=1; end=base.total ;
    *   if(end>50) end = 50 ;
    * }
    * else {
    *   if(from<=0) { start=1 ; }
    *   else start=from ;
    *   if(to>base.total) { end=base.total ; }
    *   else end = to ;
    * }
    */

   start=1; end=base.total;
   win = &base.win ;

   B = (int *)malloc(sizeof(int)*base.nb) ;
   if(B==NULL) {
      fprintf(stderr, "memory allocation error\n") ;
      return(0) ;
   }

   for(i=start-1; i<end; i++) {

      ind = base.ind[i] ;

      for(j=0; j<base.nb; j++) {
         B[j] = ~base.B[ind+j] ;
      }

      cfg2str(&base.A[ind], base.nv, winidx, win->hsize, win->wsize, strA, 2500) ;
      cfg2str(B, base.nv, winidx, win->hsize, win->wsize, strB, 2500) ;

      for(j=0; j<win->hsize/2; j++) {
         fprintf(fd, "%5s   %s", "", &strA[2*j*win->wsize]) ;
         fprintf(fd, "%5s   %s\n", "", &strB[2*j*win->wsize]) ;
      }

      fprintf(fd,"%5d)  %s", i+1, &strA[2*(win->hsize/2)*win->wsize]) ;
      fprintf(fd,"%5s   %s\n", "", &strB[2*(win->hsize/2)*win->wsize]) ;

      for(j=win->hsize/2+1; j<win->hsize; j++) {
         fprintf(fd, "%5s   %s", "", &strA[2*j*win->wsize]) ;
         fprintf(fd, "%5s   %s\n", "", &strB[2*j*win->wsize]) ;
      }
      fprintf(fd, "\n") ;
   }

   fclose(fd) ;  
   free_itrv(&base) ;
   /* free_xpl(winidx) ; */
   free (winidx);
   free(B) ;
   return 1;

}

int main (int argc, char * * argv) {
  if(!lvdspbas(argv[1], argv[2], 1, atoi (argv[3]), "-", "[", "]")) {
       fprintf(stderr, "vdspbas : lvdspbas() failed\n") ;
       return 1;
  } 
  return 0;
}
