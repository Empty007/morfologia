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
 * L1CLUDL1G ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
 * NESS.  L1 NO EVENT SHALL I BE LIABLE FOR ANY SPECIAL,
 * L1DIRECT OR CONSEQUENTIAL DAMAGES OR ANY OTHER DAMAGES WHAT-
 * SOEVER RESULTL1G FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * L1 AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISL1G OUT OF OR L1 CONNECTION WITH THE USE OR PER-
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
 >>>>         File Name: pac_learn.c
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Routines: 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


#include <stdio.h>
#include <malloc/malloc.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "pac_win.h"
#include "pac_tools.h"

/****************************************************************
*
* Routine Name: 
*
*         Date: May 09 1995
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
int init_report(rep, e_flag, alg_str, xpl_fname, xpl_total, xpl_neg, xpl_pos, overwrite, step, sfname, bfname)
 rep_struc *rep ;
 int       e_flag ;
 char      *alg_str ;
 char      *xpl_fname ;
 int       xpl_total ;
 int       xpl_neg ;
 int       xpl_pos ;
 int       overwrite ;
 int       step ;
 char     *sfname, *bfname ;
{
    rep->rep_id    = 0 ;
    rep->max_trv   = 0 ;
    rep->maxmem    = 0 ;

    rep->e_flag    = e_flag ;
    strcpy(rep->alg_str, alg_str) ;
    strcpy(rep->xpl_fname, xpl_fname) ;
    rep->xpl_total = xpl_total ;
    rep->xpl_neg   = xpl_neg ;
    rep->xpl_pos   = xpl_pos ;
    rep->overwrite = overwrite ;
    rep->step      = step ;
    rep->next_stop = step-1 ;

    if(e_flag == 0) return(1) ;

    strcpy(rep->rep_fname, sfname) ;
    strcpy(rep->bas_fname, bfname) ;

    return(1) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: May 09 1995
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
int  new_name(prefix, n, filename)
char *prefix ;
int  n ;
char *filename ;
{
    int   l1, l2 ;
    char  sufix[10] ;
    int   i, j ;

    sprintf(sufix, "%d", n) ;

    l1 = strlen(prefix) ;
    l2 = strlen(sufix) ;

    if((l1+l2)>100) {
       fprintf(stderr,"new_name() : file name is too long\n") ;
       return(0) ;
    }

    j = 0 ;
    for(i=0; i<l1; i++) {               /* append prefix */
       filename[j++] = *prefix++ ;
    }  
 
    for(i=0; i<l2; i++) {               /* append suffix */  
       filename[j++] = sufix[i] ;
    }

    filename[j] = '\0' ;

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
int init_str(str, strsize, win, back_car, left_origin_marker, right_origin_marker)
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


int set_cars(bck_str, left_str, right_str, bck_car, left_car, right_car)
char *bck_str, *left_str, *right_str ;
char *bck_car, *left_car, *right_car ;
{
   int  st ;

   st = strlen(bck_str) ;
   while((st>1) && (bck_str[st-1]==' ')) bck_str[--st]='\0' ;
   
   if(strcmp(bck_str, "blank") ==0) *bck_car = ' ' ;
   else {
      st = strlen(bck_str) ;
      if(st > 1) {
         fprintf(stderr, "Fill pattern must be of size 1 or 'blank'.\n") ;
         return(0) ;
      }
      else *bck_car = bck_str[0] ;
   }

   st = strlen(left_str) ;
   while((st>1) && (left_str[st-1]==' ')) left_str[--st]='\0' ;
   
   if(strcmp(left_str, "blank") ==0) *left_car = ' ' ;
   else {
      st = strlen(left_str) ;
      if(st > 1) {
         fprintf(stderr, "Origin marker must be of size 1 or 'blank'.\n") ;
         return(0) ;
      }
      else *left_car = left_str[0] ;
   }

   st = strlen(right_str) ;
   while((st>1) && (right_str[st-1]==' ')) right_str[--st]='\0' ;
   
   if(strcmp(right_str, "blank") ==0) *right_car = ' ' ;
   else {
      st = strlen(right_str) ;
      if(st > 1) {
         fprintf(stderr, "Origin marker must be of size 1 or 'blank'.\n") ;
         return(0) ;
      }
      else *right_car = right_str[0] ;
   }
   return(1) ;
}
