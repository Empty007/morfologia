/*
 *--------------------------------------------------------------------------
 * COPYRIGHT
 *
 * A Mathematical Morphology Toolbox for the Khoros System 
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
 >>>>         File Name: pac_bas.c
 >>>>
 >>>> Date Last Updated: April 25 1995 
 >>>>
 >>>>          Routines: 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

# include <stdio.h>
# include <stdlib.h>
# include "pac_def.h"
# include "pac_win.h"
# include "pac_xpl.h"
# include "pac_bas.h"

extern int mask[32] ;
/*
static int  mask[32] =
     { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
       0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 
       0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
       0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 
       0x20000000, 0x40000000, 0x80000000  } ;

*/


int disp_basis_by_ron (trv_struc * basis) {
   char  back_car = '-', left_car = '[', right_car = ']';
   win_struc     *win;
   int           nvars ;
   char          strA[2500] ;
   char          strB[2500] ;
   int           i, j, ind ;
   FILE          *fd ;
   int           start, end ;
   int           *B, *winidx ;
   int           st ;

   st = init_str(strA, 2500, &basis->win, back_car, left_car, right_car);
   if(st == 0) {
      return(0) ;
   }
   st = init_str(strB, 2500, &basis->win, back_car, left_car, right_car) ;
   if(st == 0) {
      return(0) ;
   }
   st = win_index(&basis->win, &winidx) ;
   if(st == 0) {
      return(0) ;
   }
   
   fd = stdout;

   fprintf(fd, "  VISUALIZATION OF BASE\n") ;
   fprintf(fd, "    Total of base elements = %d\n", basis->total) ;
   fprintf(fd, "\n\n") ;

   start=1; end=basis->total;
   win = &basis->win ;

   B = (int *)malloc(sizeof(int)*basis->nb) ;
   if(B==NULL) {
      fprintf(stderr, "memory allocation error\n") ;
      return(0) ;
   }

   for(i=start-1; i<end; i++) {

      ind = basis->ind[i] ;

      for(j=0; j<basis->nb; j++) {
         B[j] = ~basis->B[ind+j] ;
      }

      cfg2str(&basis->A[ind], basis->nv, winidx, win->hsize, win->wsize, strA, 2500) ;
      cfg2str(B, basis->nv, winidx, win->hsize, win->wsize, strB, 2500) ;

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

   // fclose(fd) ;  
   free (winidx);
   free(B) ;
   return 1;

}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int read_itrv ( fname, interv )

 char      *fname ;
 trv_struc *interv ;

{
    int     nvars, ninterv ;

    FILE    *fd ;
    char    header[8] ;
    int     n4bytes, data, i, j, b, st ;


          
   /* open file ----------------------------------------------------------- */
    fd = fopen(fname, "r") ;
    if(fd == NULL) {
       fprintf(stderr, "read_itrv() : file open error\n") ;
       return(0) ;
    }

   /* check file header --------------------------------------------------- */
    fscanf(fd, "%s", header) ;
    if(strncmp(header, "BASE", 6) != 0) {
       fprintf(stderr, "read_itrv() : unrecognized file format\n") ;
       fclose(fd) ;
       return(0) ;
    }

   /* input: number of variables and number of examples ------------------- */
    fscanf(fd, "%d %d\n", &nvars, &ninterv) ;

    n4bytes = (nvars+31)/32 ;

    st = alloc_itrv(interv, nvars, ninterv) ;
    if(st == 0) {
       fprintf(stderr, "read_itrv : alloc_interv() failed\n") ;
       fclose(fd) ;
       return(0) ;
    }

    st = read_win2(fd, &interv->win);
    if(st == 0){
       fprintf(stderr, "read_win2() failed\n") ;
       return(0) ;
    }

    i = 0 ;
    b = 0 ;

    /* reading process ---------------------------------------------------- */

    while(i<ninterv) {

       interv->ind[i] = b ;

       for(j=0; j<n4bytes; j++) {     /* reads inferior extremity data */
          fscanf(fd, "%x ", &data) ;
          interv->A[b+j] = data ;
       } 

       for(j=0; j<n4bytes; j++) {     /* reads superior extremity data */
          fscanf(fd, "%x ", &data) ;
          interv->B[b+j] = data ;
       } 

       i++ ;
       b = b+n4bytes ;

    }

    interv->total = ninterv ;

    fclose(fd) ;
    return(1) ;         
}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int  pertence ( X, n4bytes, A, B )

 int  *X ;
 int  n4bytes ;
 int  *A ;
 int  *B ;

{
    int  section, stop_flag ;

    stop_flag = 0 ;
    section = 0 ;

    while( (!stop_flag) && (section<n4bytes)) {
       if( ((A[section] & X[section]) != A[section]) ||
           ((B[section] & X[section]) != 0 )       )  {
          stop_flag = 1 ;
       }
       section++ ;
    }

    if(stop_flag) return(0) ;
    else   return(1) ;
}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int contido(A1, B1, n4bytes, A2, B2)

 int *A1, *B1, *A2, *B2 ;
 int n4bytes ;

{
    int  stop_flag ;
    int  section ;

    stop_flag = 0 ;
    section = 0 ;

    while( (!stop_flag) && (section < n4bytes) ){
       if( ((A1[section] & A2[section]) != A2[section]) ||
           ((B1[section] & B2[section]) != B2[section])  ) {
          stop_flag = 1 ;
       }
       section++ ;
    }

    if(stop_flag) return(0) ;
    else return(1) ;

}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int contido2(A, B, C)

 int *A, *B ;
 trv_struc *C ;
{
    int  stop_flag ;
    int  k ;

    stop_flag = 0 ;
    k = 0 ;

    while( (!stop_flag) && (k<C->total) ){
       if( contido(A, B, C->nb, &(C->A[C->ind[k]]), &(C->B[C->ind[k]])) ){
          stop_flag = 1 ;
       }
       k++ ;
    }

    if(stop_flag) return(1) ;
    else return(0) ;

}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int  alloc_itrv ( itrv, nvars, size )
 trv_struc *itrv ;
 int    nvars ;
 int    size ;
{
     int n4bytes ;

     n4bytes = (nvars+31)/32 ;

     itrv->A = itrv->B = NULL ;
     itrv->ind = NULL ;
     itrv->size = 0 ;
     itrv->total = 0 ;
     itrv->nv = nvars ;
     itrv->nb = n4bytes ;
     itrv->win.data = NULL ;

     if(size == 0) return(1) ;

     itrv->A = (int *) malloc (sizeof(int)*size*n4bytes);
     if(itrv->A == NULL) {
         fprintf(stderr, "alloc_itrv : memory allocation error\n") ;
         return(0) ;
     }
     itrv->B = (int *)malloc(sizeof(int)*size*n4bytes) ;
     if(itrv->B == NULL) {
         fprintf(stderr, "alloc_itrv : memory allocation error\n") ;
         return(0) ;
     }
     itrv->ind = (int *)malloc(sizeof(int)*size) ;
     if(itrv->ind == NULL) {
         fprintf(stderr, "alloc_itrv : memory allocation error\n") ;
         return(0) ;
     }
     itrv->size = size ;

     return(1) ;
}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int add_itrv( itrv, A, B)
 trv_struc *itrv ;
 int    *A, *B ;
{
     int  j, ind, st ;

     if(itrv->size == itrv->total) {
         st = realloc_itrv(itrv, itrv->size+BLKSIZE) ;
         if(st == 0) {
            fprintf(stderr, "add_itrv : realloc_itrv() failed\n") ;
            return(0) ;
         }
     }
     ind = itrv->total*itrv->nb ;
     itrv->ind[itrv->total] = ind ;

     for(j=0; j<itrv->nb; j++) {
         itrv->A[ind+j] = A[j] ;
         itrv->B[ind+j] = B[j] ;
     }
     (itrv->total)++ ;

     return(1) ;
}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int  realloc_itrv ( itrv, size )
 trv_struc *itrv ;
 int    size ;
{
     int n4bytes ;

     n4bytes = itrv->nb ;

     itrv->A = (int *)realloc(itrv->A, sizeof(int)*size*n4bytes) ;
     if(itrv->A == NULL) {
         fprintf(stderr, "realloc_itrv : memory allocation error\n") ;
         return(0) ;
     }
     itrv->B = (int *)realloc(itrv->B, sizeof(int)*size*n4bytes) ;
     if(itrv->B == NULL) {
         fprintf(stderr, "realloc_itrv : memory allocation error\n") ;
         return(0) ;
     }
     itrv->ind = (int *)realloc(itrv->ind, sizeof(int)*size) ;
     if(itrv->ind == NULL) {
         fprintf(stderr, "realloc_itrv : memory allocation error\n") ;
         return(0) ;
     }
     itrv->size = size ;

     return(1) ;
}



/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
void reset_itrv ( itrv )
 trv_struc *itrv ;
{
     itrv->total = 0 ;
}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
void free_itrv ( itrv )
 trv_struc *itrv ;
{
    if(itrv->size == 0) return ;

    if(itrv->A != NULL) free(itrv->A) ;
    if(itrv->B != NULL) free(itrv->B) ;
    if(itrv->ind != NULL) free(itrv->ind) ;
    itrv->size = 0 ;
    itrv->total = 0 ;
    free_win(&itrv->win);
}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int sel_itrv( X, itrv1, itrv2, itrv3 )
 int   *X ;
 trv_struc *itrv1, *itrv2, *itrv3 ;
{
    int    i ;
    int    st ;

    for(i=0; i<itrv1->total; i++) {
       if(pertence(X, itrv1->nb, &(itrv1->A[itrv1->ind[i]]),  
                   &(itrv1->B[itrv1->ind[i]])) ) {
          st = add_itrv(itrv2, &(itrv1->A[itrv1->ind[i]]),  
                   &(itrv1->B[itrv1->ind[i]]) );
          if(st == 0) {
             fprintf(stderr, "sel_itrv : add_itrv() failed\n") ;
             return(0) ;
          }
       }
       else {
          st = add_itrv(itrv3, &(itrv1->A[itrv1->ind[i]]),  
                   &(itrv1->B[itrv1->ind[i]]) );
          if(st == 0) {
             fprintf(stderr, "sel_itrv : add_itrv() failed\n") ;
             return(0) ;
          }
       }
    }
    return(1) ;
}
   

/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int uni_itrv (itrv1, itrv2)
 trv_struc *itrv1, *itrv2 ;
{
    int  i, j, k ;
    int  st ;

    if(itrv1->total+itrv2->total > itrv1->size) {
       st = realloc_itrv(itrv1, itrv1->total+itrv2->total) ;
       if(st == 0) {
          fprintf(stderr, "uni_itrv : realloc_itrv() failed\n") ;
          return(0) ;
       }
    }

    k = itrv1->total ;
    for(i=0; i<itrv2->total; i++) {
       itrv1->ind[k] = k*itrv1->nb ;
       for(j=0; j<itrv1->nb; j++) {
           itrv1->A[itrv1->ind[k]+j] = itrv2->A[itrv2->ind[i]+j] ;
           itrv1->B[itrv1->ind[k]+j] = itrv2->B[itrv2->ind[i]+j] ;
       }
       k++ ;
    }
    itrv1->total = itrv1->total+itrv2->total ;

    return(1) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int dimension(A, B, nvars)
 int    *A, *B ;
 int    nvars ;
{
    int   counter ;
    int   i, section, nshifts, n ;


    counter = 0 ;
    for(n=0; n<nvars; n++) {
       nshifts = n%32 ;
       section = n/32 ;
       if((A[section] & mask[nshifts]) == mask[nshifts]) {
          counter++ ;
       }
       if((B[section] & mask[nshifts]) == mask[nshifts]) {
          counter++ ;
       }
    }
    return(nvars-counter) ;
}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int  write_itrv(fname, itrv)
 char     *fname ;
 trv_struc *itrv ;
{
     FILE  *fd ;
     int   n4bytes, section, i, j ;


     fd = fopen(fname, "w") ;
     if(fd == NULL) {
        fprintf(stderr, "write_base : fopen failed\n") ;
        return(0) ;
     }

     fprintf(fd, "BASE %d %d\n", itrv->nv, itrv->total) ;

     write_win2(fd, &itrv->win);

     n4bytes = (itrv->nv+31)/32 ;

     for(i=0; i<itrv->total; i++) {
        for(j=0; j<n4bytes; j++) {
            fprintf(fd, "%x ", itrv->A[itrv->ind[i]+j]); 
        }
        fprintf(fd, "  ") ;
        for(j=0; j<n4bytes; j++) {
            fprintf(fd, "%x ", itrv->B[itrv->ind[i]+j]); 
        }
        fprintf(fd, "\n") ;
     }

     fclose(fd) ;

     return(1) ;

}


/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int disp_interv ( interv )

 trv_struc *interv ;

{
     int  i, j, n4bytes ;

     n4bytes = interv->nb ;

     for(i=0; i<interv->total; i++) {

        fprintf(stdout, "A =") ;
        for(j=0; j<n4bytes; j++) {
           fprintf(stdout, " %8x", interv->A[interv->ind[i]+j]) ;
        }
        fprintf(stdout, "     B =" ) ;
        for(j=0; j<n4bytes; j++) {
           fprintf(stdout, " %8x", interv->B[interv->ind[i]+j]) ;
        }
        fprintf(stdout, "\n") ;

     }
}



/****************************************************************
*
* Routine Name: 
*
*         Date: April 25 1995
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
int  pertence2(A, B, xpl)
 int       *A, *B ;
 xpl_struc *xpl ;
{
     int  k, n4bytes ;

     k = 0 ;
     n4bytes = (xpl->nv+31)/32 ;

     while(k<xpl->total) {
        if(pertence(&(xpl->cfg[xpl->ind[k]]), n4bytes, A, B) ) {
           return(1) ;
        }
        k++ ;
     }
     return(0) ;
}



