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

#include <stdio.h>
#include "pac_def.h"
#include "pac_win.h"

int mask[NB] = 
           { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
             0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 
             0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
             0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 
             0x20000000, 0x40000000, 0x80000000  } ;

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

int read_win(wname, win)
 char       *wname ;
 win_struc  *win ;
{
    FILE     *fd ;    
   
   /* open file */
    fd = fopen(wname, "r") ;
    if(fd == NULL) {
       fprintf(stderr, "read_win() : fopen failed\n") ;
       return(0) ;
    }

    read_win2(fd, win) ;

    fclose(fd) ;
    return(1) ;
}

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
int read_win2(fd, win)
 FILE *fd ;
 win_struc *win ;
{
    char     header[WIN_HEAD_LEN+1] ;
    int      dt ;
    int      *data, n4bytes ;
    int      m, n, mn, size ;
    int      i, j, st;


   /* check file header */
    if(fread(header, 1, WIN_HEAD_LEN, fd) < WIN_HEAD_LEN) {
       fprintf(stderr, "read_win2() : unrecognized file format\n") ;
       return(0) ;
    }
    header[WIN_HEAD_LEN] = '\0' ;

    if(strcmp(header, WIN_HEAD) == 0) {

       /* read header datas */
       fscanf(fd, " %5d", &m) ;
       fscanf(fd, " %5d", &n) ;
       fscanf(fd, " %d", &size) ;

       mn = m * n ;
   
       n4bytes = (mn+NB1)/NB ;

       data = (int *)malloc(n4bytes*sizeof(int)) ;
       if(data == NULL) {
          fprintf(stderr, "read_win2() : Memory allocation failed\n") ;
          return(0) ;
       }

      /* data reading loop */
       for(j=0; j<n4bytes; j++) {
          st = fscanf(fd, "%x", &dt) ;
          if(st == EOF) {
             fprintf(stderr, "read_win2() : file data error\n") ;
             free(data) ;
             return(0) ;
          }
          data[j] = dt ;
       }

       win->data = data ;
       win->hsize = m ;
       win->wsize = n ;
       win->size = size ;
       return(1) ;
    }

    fprintf(stderr, "read_win2() : unrecognized file format\n") ;
    return(0) ;         
}


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
void free_win(win)
 win_struc  *win ;
{
    if(win->data != NULL) free(win->data) ;
}


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
void disp_win(win)
 win_struc  *win ;
{
    int  nshifts ;
    int  mask ;
    int  i, j, section ;
    int  data ;

    for(i=0; i<win->hsize; i++) {
        for(j=0; j<win->wsize; j++) {
            nshifts = ((i*win->wsize)+j)%NB ;
            mask = 1<<nshifts ;
            section = ((i*win->wsize)+j)/NB ;
            data = win->data[section] ;
            if((data & mask) == mask) {
                printf("# ") ;
            }
            else {
                printf("- ") ;
            }
        }
        printf("\n") ;
    }
}


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
int write_win(wname, win)
 char       *wname ;
 win_struc  *win ;
{
    FILE     *fd ;    

   
   /* open file */
    fd = fopen(wname, "w") ;
    if(fd == NULL) {
       fprintf(stderr, "write_win() : fopen failed\n") ;
       return(0) ;
    }
    write_win2(fd, win);
    fclose(fd) ;
    return(1) ;   


}

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
void write_win2(fd,win)
 FILE *fd;
 win_struc *win;
{
    int      j, n4bytes;

   /* writes file header */
    fprintf(fd, "%s ", WIN_HEAD) ;

    fprintf(fd, "%5d %5d %5d\n", win->hsize, win->wsize, win->size) ;

   /* writes the code of window format specification */
    n4bytes = ((win->hsize*win->wsize)+NB1)/NB ;

    for(j=0; j<n4bytes; j++) {
       fprintf(fd, "%x ", win->data[j]) ;
    }
    fprintf(fd, "\n") ;
}

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
int copy_win(win1,win2)
 win_struc *win1, *win2 ;
{
    int n4bytes ,i;

    n4bytes = ((win1->hsize*win1->wsize) + NB1)/NB;

    win2->data = (int *)malloc(sizeof(int)*n4bytes) ;
    if( win2->data == NULL) {
       fprintf(stderr, "copy_win : memory allocation error\n") ;
       return(0);
    }
    win2->size = win1->size ;
    win2->hsize = win1->hsize ;
    win2->wsize = win1->wsize ;

    for(i=0; i<n4bytes; i++) {
       win2->data[i] = win1->data[i] ;
    }
    return(1);
}

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
int cfg2str(cfg, cfgsize, winidx, m, n, str, strsize)
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
       if((cfg[block] & mask[nshifts]) != 0) {
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
int  win_index(win, winidx)
win_struc *win ;
int       **winidx ;
{
    int  *p, i, l, block, nshifts ;

    *winidx = p = (int *)malloc(sizeof(int)*win->size) ;
    if(p == NULL) {
       return(0) ;
    }

    l = 0 ;
    for(i=0; i<win->hsize*win->wsize; i++) {
       nshifts = i%NB ;
       block = i/NB ;
       if((win->data[block] & mask[nshifts]) != 0) {
          p[l] = i ;
          l++ ;
       }
    }

    return(1) ;
}
