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
 >>>>         File Name: pac_cfg.c
 >>>>
 >>>> Date Last Updated: July 10 1995 
 >>>>
 >>>>          Routines: 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


#include <stdio.h>
#include "pac_def.h"
#include "pac_win.h"
#include "pac_cfg.h"

extern int mask[NB] ;

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void leftrotate(pp)
 cfg_node **pp ;
{
    cfg_node *p, *r ;

    p = *pp ;
    *pp = r = p->right ;
    p->right = r->left ;
    r->left = p ;
    p->bal-- ;
    if(r->bal > 0) p->bal -= r->bal ;
    r->bal-- ;
    if(p->bal <0) r->bal += p->bal ;
}


/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void rightrotate(pp)
 cfg_node **pp ;
{
    cfg_node *p, *l ;

    p = *pp ;
    *pp = l = p->left ;
    p->left = l->right ;
    l->right = p ;
    p->bal++ ;
    if(l->bal < 0) p->bal -= l->bal ;
    l->bal++ ;
    if(p->bal > 0) l->bal += p->bal ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
cfg_node *alloc_cfg_node(cfgsize)
int cfgsize ;
{
    cfg_node *p ;

    p = (cfg_node *)malloc(sizeof(cfg_node)) ;
    if(p == NULL) {
       return(NULL) ;
    }
    p->fq0 = 0 ;
    p->fq1 = 0 ;
    p->bal = 0 ;
    p->left = p->right = NULL ;
    p->data = (int *)malloc(sizeof(int)*cfgsize) ;
    if(p->data == NULL) {
       free(p) ;
       return(NULL) ;
    }
    return(p) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void set_cfg_node(p, cfgsize, x, fq0, fq1)
 cfg_node *p ;
 int      cfgsize, *x, fq0, fq1 ;
{
    int j ;

    p->fq0 += fq0 ;
    p->fq1 += fq1 ;
    for(j=0; j<cfgsize; j++) {
       p->data[j] = x[j] ;
    }
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int  compare_cfg_node(p, cfgsize, x)
 cfg_node *p ;
 int      cfgsize, *x ;
{
    int  j, dif ;

    dif = EQUAL ;
     
    for(j=cfgsize-1; (j>=0) && (dif==EQUAL); j--) {
       if(x[j] > p->data[j]) { 
          dif = LESS ;
       }
       else {
          if(x[j] < p->data[j]) {
             dif = GREAT ;
          }
       }
    }

    return(dif) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int insert_cfg(cfg, pp, x, fq0, fq1)
cfg_struc *cfg ;
cfg_node  **pp ;
int       *x ;
int       fq0, fq1;
{
    int deltaH = 0, st ;
    cfg_node *p ;

    p = *pp ;
    if(p == NULL) {
       *pp = p = alloc_cfg_node(cfg->cfgsize) ;
       if(p == NULL) {
          fprintf(stderr, "Not enough memory\n") ;
          return(-1) ;
       }
       set_cfg_node(p, cfg->cfgsize, x, fq0, fq1) ;
       deltaH = 1 ;
       cfg->total++ ;
       cfg->sumtotal += (fq0+fq1) ;
    }
    else {
       st = compare_cfg_node(p, cfg->cfgsize, x) ;
       if(st == 0) {
          p->fq0 += fq0 ;
          p->fq1 += fq1 ;
          cfg->sumtotal += (fq0+fq1) ;
       }
       else if(st == LESS) {
          if(insert_cfg(cfg, &p->right, x, fq0, fq1)) {
             p->bal++ ;
             if(p->bal == 1) deltaH = 1 ;
             else if(p->bal == 2) {
                if(p->right->bal == -1) rightrotate(&p->right) ;
                leftrotate(pp) ;
             }
          }
       }
       else {
          if(insert_cfg(cfg, &p->left, x, fq0, fq1)) {
             p->bal-- ;
             if(p->bal == -1) deltaH = 1 ;
             else if(p->bal == -2) {
                if(p->left->bal == 1) leftrotate(&p->left) ;
                rightrotate(pp) ;
             }
          }
       }
    }
    return(deltaH) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int read_cfg(fname, cfg)
 char  *fname;
 cfg_struc *cfg; 
{
   FILE  *fd;
   char  header[CFG_HEAD_LEN+1];
   cfg_node *p ;
   int   n4bytes, *data, x, nvars, total, total_counted ;
   int   fq0, fq1, i, j, st ;


   /* open file ---------------------------------------------------------- */
    fd = fopen(fname, "r") ;
    if(fd == NULL) {
       fprintf(stderr, "read_cfg() : fname open error\n") ;
       return(0) ;
    }

   /* check file header -------------------------------------------------- */
    fscanf(fd, "%s", header) ;
    if(strncmp(header, CFG_HEAD, CFG_HEAD_LEN) != 0) {
       fprintf(stderr, "read_cfg : unrecognized file format\n") ;
       fclose(fd) ;
       return(0) ;
    }

   /* gets number of variables and number of examples ------------------- */
    fscanf(fd, "%d %d %d\n", &nvars, &total, &total_counted) ;

    st = read_win2(fd, &cfg->win);
    if(st == 0){
       fprintf(stderr, "read_win2() failed\n") ;
       return(0) ;
    }

    n4bytes = (nvars+NB1)/NB ;

    cfg->total = 0 ;
    cfg->sumtotal = 0 ;
    cfg->cfgsize = n4bytes ;
    cfg->nv = nvars ;
    cfg->root = NULL ;

   /* reading loop ------------------------------------------------------ */
    data = (int *)malloc(sizeof(int)*n4bytes) ;
    if(data == NULL) {
       fprintf(stderr, "Not enough memory\n") ;
       return(0);
    }

    i = 0 ;
    while(i<total) {

       for(j=0; j<n4bytes; j++) {
          fscanf(fd, "%x ", &x) ;
          data[j] = x ;
       } 

       fscanf(fd, "%d ", &fq0) ;

       fscanf(fd, "%d ", &fq1) ;

       st = insert_cfg(cfg, &cfg->root, data, fq0, fq1) ;

       i++ ;

    }

    fclose(fd) ;

    return(1) ;     
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void free_cfg_node(p)
cfg_node *p ;
{
   free(p->data) ;
   free(p) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void free_cfg_tree(p)
cfg_node *p ;
{
   if(p != NULL) {
      free_cfg_tree(p->left) ;
      free_cfg_tree(p->right) ;
      free_cfg_node(p) ;
   }
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void free_cfg(cfg)
cfg_struc *cfg ;
{
   free_win(&cfg->win);
   free_cfg_tree(cfg->root) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void reset_cfg(cfg)
cfg_struc *cfg ;
{
   free_cfg_tree(cfg->root) ;
   cfg->root = NULL ;
   cfg->total = 0 ;
   cfg->sumtotal = 0 ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void print_cfg_tree(fd, p, cfgsize)
FILE *fd ;
cfg_node *p ;
int cfgsize ;
{
    int i ;

    if(p != NULL) {
       print_cfg_tree(fd, p->left, cfgsize) ;
       for(i=0; i<cfgsize; i++) {
          fprintf(fd, "%x ", p->data[i]) ;
       }
       fprintf(fd, "%d %d\n", p->fq0, p->fq1) ;
       print_cfg_tree(fd, p->right, cfgsize) ;
    }
}


/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int write_cfg(fname, cfg)
char      *fname ;
cfg_struc *cfg ;
{
    FILE  *fd ;

    fd = fopen(fname, "w") ;
    if(fd == NULL) {
       fprintf(stderr, "write_cfg : file open error\n") ;
       return(0) ;
    }

    fprintf(fd, "%s ", CFG_HEAD) ;
    fprintf(fd, "%d %d %d\n", cfg->nv, cfg->total, cfg->sumtotal) ;

    write_win2(fd, &cfg->win);

    print_cfg_tree(fd, cfg->root, cfg->cfgsize) ;

    fclose(fd) ;

    return(1) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int merge_cfg(cfg, p)
cfg_struc *cfg ;
cfg_node  *p ;
{
    int   st ;

    if(p != NULL) {
       st = merge_cfg(cfg, p->left) ;
       if(st == 0) return(0) ;
       st = merge_cfg(cfg, p->right) ;
       if(st == 0) return(0) ;
       st = insert_cfg(cfg, &cfg->root, p->data, p->fq0, p->fq1) ;
       if(st == -1) {
          return(0) ;
       }
    }
    return(1) ;
}


/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int count_conflicts(p)
 cfg_node *p ;
{
   int  count=0 ;

   if(p != NULL) {
      count = count_conflicts(p->left) + count_conflicts(p->right) ; 
      if((p->fq0 != 0) && (p->fq1 != 0)) return(count+1) ;
   }
   return(count) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void disp_cfg_node(fd, cfg, winidx, str, counter, data, fq0, fq1)
FILE      *fd ;
cfg_struc *cfg ;
int       *winidx ;
char      *str ;
int       counter ;
int       *data, fq0, fq1 ;
{
   win_struc *win ;
   int       i, j ;

   win = &cfg->win ;

   cfg2str(data, cfg->nv, winidx, win->hsize, win->wsize, str, 2500) ;

   for(j=0; j<(win->hsize/2); j++) {
      fprintf(fd, "%5s   %s\n", "", &str[2*j*win->wsize]) ;
   }

   fprintf(fd, "%5d)  %s %8d %8d\n", counter+1, 
      &str[2*(win->hsize/2)*win->wsize], fq0, fq1) ;

   for(j=(win->hsize/2)+1; j<win->hsize; j++) {
      fprintf(fd, "%5s   %s\n", "", &str[2*j*win->wsize]) ;
   }
   fprintf(fd, "\n") ;

}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void dsp_cfg_tree1(fd, cfg, p, winidx, start, end, counter, str)
FILE     *fd ;
cfg_struc *cfg ;
cfg_node *p ;
int      *winidx ;
int      start, end, *counter ;
char     *str ;
{
   void  disp_cfg() ;

   if(p != NULL) {

      dsp_cfg_tree1(fd, cfg, p->left, winidx, start, end, counter, str) ;

      if(*counter >= start-1) {
        if(*counter < end) {
           if((p->fq0 > 0) && (p->fq1>0)) {
              disp_cfg_node(fd, cfg, winidx, str, *counter, p->data, p->fq0, p->fq1) ;
           }
        }
        else return ;
      }

      (*counter)++ ;

      dsp_cfg_tree1(fd, cfg, p->right, winidx, start, end, counter, str) ;
   }
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
void dsp_cfg_tree2(fd, cfg, p, winidx, start, end, counter, str)
FILE      *fd ;
cfg_struc *cfg ;
cfg_node  *p ;
int       *winidx ;
int       start, end, *counter ;
char      *str ;
{
   void disp_cfg() ;

   if(p != NULL) {

      dsp_cfg_tree2(fd, cfg, p->left, winidx, start, end, counter, str) ;

      if((*counter) >= start-1) {
         if((*counter) < end) {
            disp_cfg_node(fd, cfg, winidx, str, *counter, p->data, p->fq0, p->fq1) ;
         }
         else return ;
      }

      (*counter)++ ;

      dsp_cfg_tree2(fd, cfg, p->right, winidx, start, end, counter, str) ;
   }
}


/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int count_pat(cfg, p1, p2, p3, strwin, numpixels, nc)
cfg_struc *cfg ;
char      *p1, *p2, *p3 ;
int       *strwin ;
int       numpixels, nc ;
{
    int       i, j, k, l, s ;
    int       nshifts ;
    int       config_ok ;
    int       *config ;
    int       st ;

    config = (int *)malloc(sizeof(int)*cfg->cfgsize) ;
    if(config == NULL) {
       fprintf(stderr, "count_pat : memory allocation error\n") ;
       return(0) ;
    }

    for(j = 0; j<numpixels; j++) {         /* shifts the window */

      if(p3[j] != 0) {   /* mask condition satisfied */

        if(((j+strwin[0])>=0) && ((j+strwin[cfg->win.size-1])<numpixels)) {

           config_ok = 1;

           for(i=0; i<cfg->cfgsize; i++) {
              config[i] = 0 ;
           }

           for(i=0; ((i<cfg->win.size) && config_ok); i++) {
              s = i/NB ;
              k = j+strwin[i] ;
              if( (abs((j%nc)-(k%nc))) <= ((cfg->win.wsize)/2) ) {
                 if(p1[k] != 0) {
                    nshifts = i%NB ;
                    config[s] = config[s] | mask[nshifts] ;
                 }
              }   
              else {
                 config_ok = 0 ;
              }
           }

           if(config_ok) {
              if(p2[j] == 0) {
                 st = insert_cfg(cfg, &cfg->root, config, 1, 0) ;
              }
              else {
                 st = insert_cfg(cfg, &cfg->root, config, 0, 1) ;
              }
              if(st == -1) {
                 fprintf(stderr, "lvpat1 : insert_cfg() failed\n") ;
                 free(config) ;
                 return(0) ;
              }
           }
        }
      }

    }

    free(config) ;
    return(1) ;
}



/* -----------------------------------------------------------*/
int sort_by_freq(cfg, type)
 cfg_struc *cfg ;
{
    cfg_node *p ;
    int      st ;

    p = NULL ;
    if(type == ASC) {
       st = sort_asc_cfg(cfg->root, &p) ;
    }
    else {
       st = sort_desc_cfg(cfg->root, &p) ;
    }
    if(st == 0) {
       fprintf(stderr, "sort_by_freq : subroutine failed\n") ;
       return(0) ;
    }
    cfg->root = p ;
    return(1) ;
}


int sort_asc_cfg(p, pp)
 cfg_node *p, **pp ;
{
    int st ;

    if(p != NULL) {
       st = sort_asc_cfg(p->left, pp) ;
       if(st == 0) {
          fprintf(stderr, "sort_cfg failed\n") ;
          return(0) ;
       }
       st = sort_asc_cfg(p->right, pp) ;
       if(st == 0) {
          fprintf(stderr, "sort_cfg failed\n") ;
          return(0) ;
       }
       st = insert_by_freq1(pp, p) ;
    }
    return(1) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int insert_by_freq1(pp, p)
cfg_node  **pp, *p ;
{
    int deltaH = 0, st ;
    cfg_node     *q ;

    q = *pp ;
    if(q == NULL) {
       q = *pp = p ;
       q->left = NULL ;
       q->right = NULL ;
       q->bal = 0 ;
       deltaH = 1 ;
    }
    else {

    if((p->fq0+p->fq1) >= (q->fq0+q->fq1)) {
       if(insert_by_freq1(&q->right, p)) {
          q->bal++ ;
          if(q->bal == 1) deltaH = 1 ;
          else if(q->bal == 2) {
             if(q->right->bal == -1) rightrotate(&q->right) ;
             leftrotate(pp) ;
          }
       }
    }
    else {
       if(insert_by_freq1(&q->left, p)) {
          q->bal-- ;
          if(q->bal == -1) deltaH = 1 ;
          else if(q->bal == -2) {
             if(q->left->bal == 1) leftrotate(&q->left) ;
             rightrotate(pp) ;
          }
       }
    }
    }
    return(deltaH) ;
}

int sort_desc_cfg(p, pp)
 cfg_node *p, **pp ;
{
    int st ;

    if(p != NULL) {
       st = sort_desc_cfg(p->left, pp) ;
       if(st == 0) {
          fprintf(stderr, "sort_cfg failed\n") ;
          return(0) ;
       }
       st = sort_desc_cfg(p->right, pp) ;
       if(st == 0) {
          fprintf(stderr, "sort_cfg failed\n") ;
          return(0) ;
       }
       st = insert_by_freq2(pp, p) ;
    }
    return(1) ;
}

/****************************************************************
*
* Routine Name: 
*
*         Date: June 05 1995
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
int insert_by_freq2(pp, p)
cfg_node  **pp, *p ;
{
    int deltaH = 0, st ;
    cfg_node     *q ;

    q = *pp ;
    if(q == NULL) {
       q = *pp = p ;
       q->left = NULL ;
       q->right = NULL ;
       q->bal = 0 ;
       deltaH = 1 ;
    }
    else {

    if((p->fq0+p->fq1) < (q->fq0+q->fq1)) {
       if(insert_by_freq2(&q->right, p)) {
          q->bal++ ;
          if(q->bal == 1) deltaH = 1 ;
          else if(q->bal == 2) {
             if(q->right->bal == -1) rightrotate(&q->right) ;
             leftrotate(pp) ;
          }
       }
    }
    else {
       if(insert_by_freq2(&q->left, p)) {
          q->bal-- ;
          if(q->bal == -1) deltaH = 1 ;
          else if(q->bal == -2) {
             if(q->left->bal == 1) leftrotate(&q->left) ;
             rightrotate(pp) ;
          }
       }
    }
    }
    return(deltaH) ;
}

