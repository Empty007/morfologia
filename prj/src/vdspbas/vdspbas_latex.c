#include <stdio.h>
#include <stdlib.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "vdspbas.h"
/*
 *--------------------------------------------------------------------------
 * COPYRIGHT
 *
 * A PAC Learning Toolbox for the Khoros System 
 *
 * Copyright 1995 - University of Sao Paulo - USP
 *
 * All rights reserved.
 *
 * Due to potential patent problems, this program may only be used
 * for research in morphological image processing and practical classes
 * in University courses. The authors do not take any resposibilities
 * for use beyond this scope, nor do we claim any suitability and  
 * operability of this software for any purpose. It is provided "as is"
 * without express or implied warranty.
 *
 * I DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
 * NESS.  IN NO EVENT SHALL I BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY OTHER DAMAGES WHAT-
 * SOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PER-
 * FORMANCE OF THIS SOFTWARE.
 * 
 * No other rights, including, for example, the right to redis-
 * tribute this software and its documentation or the right to
 * prepare derivative works, are granted unless specifically
 * provided in a separate license agreement.
 *--------------------------------------------------------------------------
 */

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: pac_win.c
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Routines: 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

int maski[NB] = 
{ 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
	0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 
	0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
	0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 
	0x20000000, 0x40000000, 0x80000000  } ;

/****************************************************************
 *
 * Routine Name: 
 *
 *         Date: 
 *
 *      Purpose: 
 *
 *        Input: 
 *          
 *       Output: 1 on success, 0 on failure 
 *
 *   Written By: Nina S. Tomita
 *        
 ****************************************************************/
int cfg2stredu(cfg, cfgsize, winidx, m, n, str, strsize)
int  *cfg ;        /* cfg[w]  where w=number of variables */
int  cfgsize ;
int  *winidx ;     /* winidx[w] */
int  m, n ;        /* m and n are the height and the width of
					the rectangle corresponding to window domain */
char *str ;        /* str[strsize] */
int  strsize ;
{
	
    int i ;
    int block, nshifts ;
	
    for(i=0; i<cfgsize; i++) {
		nshifts = i%NB ;
		block = i/NB ;
		if((cfg[block] & maski[nshifts]) != 0) {
			str[2*winidx[i]] = '1' ;
		}
		else {
			str[2*winidx[i]] = '0' ;
		}
    }
    return(1) ;
}

/****************************************************************
 *
 * Routine Name: 
 *
 *         Date: June 22 1995
 *
 *      Purpose: 
 *
 *        Input: 
 *          
 *       Output: 1 on success, 0 on failure 
 *
 *   Written By: Nina S. Tomita
 *        
 ****************************************************************/
int init_str2(str, strsize, win, back_car, left_origin_marker, right_origin_marker)
char *str ;
int strsize ;
win_struc *win ;
char back_car, left_origin_marker, right_origin_marker ;
{
    int i, j, base ;
	
    if(strsize < 2*win->hsize*win->wsize) {
		return(0) ;
    }
    base = 0 ;
    for(i=0; i<win->hsize; i++) {
		for(j=0; j<win->wsize; j++) {
			str[base+2*j] = back_car ;
			str[base+2*j+1] = ' ' ;
		}
		str[base+(2*win->wsize)-1] = '\0' ;
		base = base + 2*win->wsize ;
	}
	str[((win->hsize-1)/2)*(win->wsize*2)+((2*(win->wsize-1)/2))-1] = left_origin_marker ;
	str[((win->hsize-1)/2)*(win->wsize*2)+((2*(win->wsize-1))/2)+1] = right_origin_marker ;
	
	return(1) ;
}




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

   st = init_str2(strA, 2500, &base.win, back_car, left_car, right_car) ;
   if(st == 0) {
      return(0) ;
   }
   st = init_str2(strB, 2500, &base.win, back_car, left_car, right_car) ;
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

//   fprintf(fd, "  VISUALIZATION OF BASE\n") ;
//   fprintf(fd, "    Total of base elements = %d\n", base.total) ;
//   fprintf(fd, "\n\n") ;

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

      cfg2stredu(&base.A[ind], base.nv, winidx, win->hsize, win->wsize, strA, 2500) ;
      cfg2stredu(B, base.nv, winidx, win->hsize, win->wsize, strB, 2500) ;

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
