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
 >>>>         File Name: pac_xpl.c
 >>>>
 >>>> Date Last Updated: August 31 1995 
 >>>>
 >>>>          Routines: 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h>
#include "pac_def.h"
#include "pac_win.h"
#include "pac_cfg.h"
#include "pac_xpl.h"


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
int read_xpl ( fname, xpl )

 char      *fname ;
 xpl_struc *xpl ;

{
    int      nvars, total ;
    FILE     *fd ;
    char     header[8] ;

    int      n4bytes, data, i, j, b, st ;


   /* open file ---------------------------------------------------------- */
    fd = fopen(fname, "r") ;
    if(fd == NULL) {
       fprintf(stderr, "read_xpl() : fname open error\n") ;
       return(0) ;
    }

   /* check file header -------------------------------------------------- */
    fscanf(fd, "%s", header) ;
    if(strncmp(header, "EXAMPLE", 7) != 0) {
       fprintf(stderr, "read_xpl() : unrecognized fname format\n") ;
       fclose(fd) ;
       return(0) ;
    }

   /* gets number of variables and number of examples ------------------- */
    fscanf(fd, "%d %d\n", &nvars, &total) ;

 
    n4bytes = (nvars+31)/32 ;

    st = alloc_xpl(xpl, nvars, total) ;
    xpl->total = total ;
    xpl->nb = n4bytes ;

    st = read_win2(fd, &xpl->win);
    if(st == 0){
       fprintf(stderr, "read_win2() failed\n") ;
       return(0) ;
    }
    
   /* reading loop ------------------------------------------------------ */
    i = 0 ;
    b = 0 ;

    xpl->nneg = 0 ;
    xpl->npos = 0 ;

    while(i<total) {

       xpl->ind[i] = b ;
       for(j=0; j<n4bytes; j++) {
          fscanf(fd, "%x ", &data) ;
          xpl->cfg[b+j] = data ;
       } 

       fscanf(fd, "%d ", &data) ;
       xpl->vl[i] = data ;

       if(data == 0) {
          xpl->nneg++ ;
       }
       else {
          xpl->npos++ ;
       }

       fscanf(fd, "%d\n ", &data) ;
       xpl->fx[i] = data ;

       i++ ;
       b = b+n4bytes ;

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
void  free_xpl(xpl)
xpl_struc *xpl ;
{
     if(xpl->ind != NULL) free(xpl->ind) ;
     if(xpl->vl != NULL) free(xpl->vl) ;
     if(xpl->fx != NULL) free(xpl->fx) ;
     if(xpl->cfg != NULL) free(xpl->cfg) ;
     xpl->size = xpl->total = 0 ;
     free_win(&xpl->win);
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
int  alloc_xpl(xpl, nv, size)
 xpl_struc *xpl ;
 int       nv ;
 int       size ;
{
    xpl->ind = NULL ;
    xpl->cfg = NULL ;
    xpl->vl = NULL ;
    xpl->fx = NULL ;
    xpl->nv = nv ;
    xpl->nb = (nv+31)/32 ;
    xpl->size = size ;
    xpl->total = 0 ;
    xpl->nneg = xpl->npos = 0 ;
    xpl->win.data = NULL ;

    if(size > 0) {
       xpl->ind = (int *)malloc(sizeof(int)*size);
       if (xpl->ind==NULL){
          fprintf(stderr,"alloc_xpl: memory allocation error\n");
          return(0);
       }

       xpl->cfg = (int *)malloc(sizeof(int)*xpl->nb*size);
       if (xpl->cfg==NULL){
          fprintf(stderr,"alloc_xpl: memory allocation error\n");
          return(0);
       }

       xpl->vl = (int *)malloc(sizeof(int)*size);
       if (xpl->vl==NULL){
          fprintf(stderr,"alloc_xpl: memory allocation error\n");
          return(0);
       }

       xpl->fx = (int *)malloc(sizeof(int)*size);
       if (xpl->fx==NULL){
          fprintf(stderr,"alloc_xpl: memory allocation error\n");
          return(0);
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
int sep_xpls ( xpl, xpl1, xpl2 )

 xpl_struc *xpl, *xpl1, *xpl2 ;

{
    int      count1, count2 ;
    int      i, j, k1, k2, n4bytes, st ;


    n4bytes = xpl->nb ;

    count1 = 0 ;

    for(i=0; i<xpl->total; i++) {
       if(xpl->vl[i] == 1) {
          count1++ ;
       }
    }
    count2 = xpl->total - count1 ;

    st = alloc_xpl(xpl1, xpl->nv, count1) ;
    if(st == 0) {
       fprintf(stderr, "sep_xpls : alloc_xpl() failed\n") ;
       return(0) ;
    }
    xpl1->total = count1 ;

    st = alloc_xpl(xpl2, xpl->nv, count2) ;
    if(st == 0) {
       fprintf(stderr, "sep_xpls : alloc_xpl() failed\n") ;
       return(0) ;
    }
    xpl2->total = count2 ;

    k1=k2 = 0 ;
    for(i = 0; i<xpl->total; i++ ) {

       if(xpl->vl[i] == 1) {
          xpl1->vl[k1] = 1 ;
          xpl1->fx[k1] = xpl->fx[i] ;
          xpl1->ind[k1] = k1*n4bytes ;
          for(j=0; j<n4bytes; j++) {
             xpl1->cfg[k1*n4bytes+j] = xpl->cfg[i*n4bytes+j] ;
          }
          k1++ ;
       }
       else {
          xpl2->vl[k2] = 0 ;
          xpl2->fx[k2] = xpl->fx[i] ;
          xpl2->ind[k2] = k2*n4bytes ;
          for(j=0; j<n4bytes; j++) {
             xpl2->cfg[k2*n4bytes+j] = xpl->cfg[i*n4bytes+j] ;
          }
          k2++ ;
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
int disp_xpl ( xpl )
 
 xpl_struc *xpl ;

{
     int  i, j, n4bytes ;

     n4bytes = xpl->nb ;

     for(i=0; i<xpl->total; i++) {
        for(j=0; j<n4bytes; j++) {
           fprintf(stdout, "%8x ", xpl->cfg[xpl->ind[i]+j]) ;
        }
        if(xpl->vl[i] == NEGATIVE) {
           fprintf(stdout, "   %s   %d\n", "NEG", xpl->fx[i]) ;
        }
        else {
           fprintf(stdout, "   %s   %d\n", "POS", xpl->fx[i]) ;
        }
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
int write_xpl(fname, xpl)
  char *fname ;
  xpl_struc *xpl ;
  
{
    FILE *fd ;
    int  n4bytes ;
    int  ind, i ;


    /* Open file */
    fd = fopen(fname,"w");
    if (fd==NULL){
       fprintf(stderr, "write_xpl : file open error\n");
       return(0);
    }

    /* writes a header */
    fprintf(fd, "EXAMPLE ");
    fprintf(fd, "%d %d\n", xpl->nv, xpl->nneg+xpl->npos);

    write_win2(fd, &xpl->win);

    /* writes the examples */
    n4bytes = (xpl->nv+31)/32;

    for(ind=0; ind<xpl->total; ind++){
       if(xpl->vl[ind] != -1){
	  for(i=0; i<n4bytes; i++) {
	     fprintf(fd,"%x ", xpl->cfg[ind*n4bytes+i]);
          }
          fprintf(fd,"%d %d\n", xpl->vl[ind], -1);
       }
    }

    fclose(fd);
    return(1);
}



/****************************************************************
*
* Routine Name: 
*
*         Date: August 31 1995
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
int verif_cond ( cfg, xpl, totprop, relprop, rejects, n_examples )

  cfg_struc *cfg ;
  xpl_struc *xpl ;
  float      totprop, relprop;
  int        rejects ;
  int        n_examples ;
     
{
   int  st, i, r, n ;


   printf("n_examples = %d\n", n_examples) ;
   

   st = alloc_xpl(xpl, cfg->nv, cfg->total) ;
   if(st == 0) {
      fprintf(stderr, "verif_cond : alloc_xpl() failed\n") ;
      return(0) ;
   }

   st = copy_win(&cfg->win, &xpl->win) ;
   if(st == 0){
      fprintf(stderr, "verif_cond : copy_win() failed\n") ;
      return(0) ;
   }

   if(cfg->sumtotal < n_examples) n=cfg->sumtotal ;
   else n=n_examples ;
   
   xpl->total = cfg->total ;
   for(i=0; i<xpl->total; i++) {
      xpl->ind[i] = i*cfg->cfgsize ;
      xpl->vl[i] = 0 ;
      xpl->fx[i] = 0 ;
   }

   i = 0 ;
   r = (float)totprop*cfg->sumtotal ;
   st = reduce2xpl(cfg->root, cfg->cfgsize, r, relprop, rejects, &i, &n, xpl) ;
   if(st == 0) {
      fprintf(stderr, "verif_cond : reduce2xpl() failed\n") ;
      return(0) ;
   }

   return(1) ;

}


/****************************************************************
*
* Routine Name: 
*
*         Date: August 31 1995
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
int reduce2xpl(p, cfgsize, rate1, rate2, rejection, i, n, xpl)
 cfg_node  *p ;
 int        cfgsize ;
 int        rate1 ;
 float      rate2 ;
 int        rejection ;
 int       *i ;
 int       *n ;
 xpl_struc *xpl ;
{
   int  st, f0, f1, greatest, least ;
   int  j, k ;


   if((p != NULL) && (*n > 0)) {
      j = *i ;

      st = reduce2xpl(p->left, cfgsize, rate1, rate2, rejection, &j, n, xpl) ;
      if(st == 0) {
          fprintf(stderr, "Error in reduce2xpl()\n") ;
          return(0) ;
      }

      f0 = p->fq0 ;
      f1 = p->fq1 ;

      if( f0 > f1 ){
	greatest=f0; least=f1;
      }
      else{
           greatest=f1; least=f0;
      }

      if((f0+f1)>=rate1){/*2*/
	
	if (least==greatest){ /*3*/

		switch (rejection) {
		   case 1:
			xpl->vl[j] = -1 ;
		 	break;
		   case 2:
			xpl->vl[j] = 0 ;
			xpl->nneg++ ;
			break;
		   case 3:
			xpl->vl[j] = 1 ;
			xpl->npos++;
			break;
		   default:
			xpl->vl[j] = -1 ;
		}/*switch*/
	} /*3*/

	else if(least==0) {

           if(f0==0) {
               xpl->vl[j] = 1 ;
	       xpl->npos++;
           }
           else {
               xpl->vl[j] = 0;
	       xpl->nneg++ ;
           }
	}

	else{ /*4*/

	   if(least<(rate2*greatest)){
               if(f0<f1) {
                  xpl->vl[j] = 1 ;
	          xpl->npos++;
               }
               else {
                  xpl->vl[j] = 0;
	          xpl->nneg++ ;
               }
	   }
	   else {

		switch (rejection) {
			case 1:
			     xpl->vl[j] = -1 ;
 			     break;
			case 2:
		             xpl->vl[j] = 0 ;
		             xpl->nneg++ ;
			     break;
			case 3:
			     xpl->vl[j] = 1 ;
			     xpl->npos++;
 			     break;
			default:
			     xpl->vl[j] = -1 ;
		} /*switch*/

	   }
         } /*4*/

      } /*2*/

      else xpl->vl[j] = -1 ;

      if(xpl->vl[j] != -1) *n = *n-(f0+f1) ;
      
      for(k=0; k<cfgsize; k++) xpl->cfg[xpl->ind[j]+k] = p->data[k] ;

      j++ ;

      st = reduce2xpl(p->right, cfgsize, rate1, rate2, rejection, &j, n, xpl) ;
      if(st == 0) {
          fprintf(stderr, "Error in reduce2xpl()\n") ;
          return(0) ;
      }
      *i = j ;
   }

   return(1);

} 

