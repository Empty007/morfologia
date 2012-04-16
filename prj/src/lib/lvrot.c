 /*
  * Khoros: $Id$
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id$";
#endif

 /*
  * $Log$
  */

/*
 *--------------------------------------------------------------------------
 * COPYRIGHT
 *
 * MMach v1.1 - A Mathematical Morphology Toolbox for the Khoros System 
 *
 * Copyright 1994 - University of Sao Paulo - USP
 *                  University of Campinas - UNICAMP
 *                  National Institute for Space Research - INPE
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
 >>>>         File Name: lvrotstr.c
 >>>>
 >>>>      Program Name: vrotstr
 >>>>
 >>>> Date Last Updated: Fri Apr 28 15:58:03 1995 
 >>>>
 >>>>          Routines: lvrotstr - the library call for vrotstr
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


#include "vinclude.h"


/* -library_includes */
#include "mat_str.h"
/* -library_includes_end */


/****************************************************************
*
* Routine Name: lvrotstr - library call for vrotstr
*
* Purpose:
*    
*    Rotates a structuring element.
*    
*    

* Input:
*    
*    b_in           input structuring element (mat_str object)
*    
*    step           rotation step
*    
*    d              a flag set if clockwise struct. el. rotation
*    
*    

* Output:
*    
*    b_out          output structuring element (mat_str object)
*    
*    

*
* Written By: Junior Barrera & Adriano Nagelschmidt Rodrigues
*    
*    Adriano Nagelschmidt Rodrigues: added  generic  struct.  el.
*    rotation.
*    
*    

****************************************************************/


/* -library_def */
int
lvrotstr (b_in, step, d, b_out)
mat_str *b_in, *b_out;
int step, d;
/* -library_def_end */

/* -library_code */
{
   int i, j, k, rr, nc, nr, nb;
   short *s1, *s2;

   if ((step < 0) || (step > 7)) {
     fprintf(stderr, "lvrotstr: rotation step out of range\n");
     return(0);
   }

   if (!d && (step != 0))
     step = 8 - step;

   b_out->m = b_in->m;
   b_out->n = b_in->n;
   b_out->p = b_in->p;
   b_out->tag = b_in->tag;

   switch (b_in->tag) {
      
      /* Generic struct. el. rotation */
      case MM_IMG:

         if (step != 0 && step != 4) {
           fprintf(stderr, "lvrotstr: the only valid rot. step with generic struct. el. is 0 or 4\n");
           return(0);
         }
  
         nc = b_in->n;
         nr = b_in->m;
         nb = b_in->p;
         k = nr*nc;

         s2 = (short *)malloc(k*nb*sizeof(short));
         if (s2 == NULL) { 
           fprintf(stderr,"lvrotstr: Could not allocate enough memory!\n");
           return(0);
         }

         s1 = b_in->dat;
         for (j = 0; j < nb; j++) {
           rr = j*k;
           for (i = 0; i < k; i++)
             s2[k - 1 - i + rr] = s1[i + rr];
         }
         b_out->dat = s2;
         break;


      case MM_MAT:
        switch (step) {
 
          case 0:
             b_out->x[2][2] = b_in->x[2][2];
             b_out->x[2][1] = b_in->x[2][1];
             b_out->x[2][0] = b_in->x[2][0]; 
             b_out->x[1][2] = b_in->x[1][2];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[1][0];
             b_out->x[0][2] = b_in->x[0][2];
             b_out->x[0][1] = b_in->x[0][1];
             b_out->x[0][0] = b_in->x[0][0];
             break;

          case 1: 
             b_out->x[2][2] = b_in->x[2][1];
             b_out->x[2][1] = b_in->x[2][0];
             b_out->x[2][0] = b_in->x[1][0]; 
             b_out->x[1][2] = b_in->x[2][2];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[0][0];
             b_out->x[0][2] = b_in->x[1][2];
             b_out->x[0][1] = b_in->x[0][2];
             b_out->x[0][0] = b_in->x[0][1];
             break;

          case 2:
             b_out->x[2][2] = b_in->x[2][0];
             b_out->x[2][1] = b_in->x[1][0];
             b_out->x[2][0] = b_in->x[0][0]; 
             b_out->x[1][2] = b_in->x[2][1];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[0][1];
             b_out->x[0][2] = b_in->x[2][2];
             b_out->x[0][1] = b_in->x[1][2];
             b_out->x[0][0] = b_in->x[0][2];
             break;

          case 3: 
             b_out->x[2][2] = b_in->x[1][0];
             b_out->x[2][1] = b_in->x[0][0];
             b_out->x[2][0] = b_in->x[0][1]; 
             b_out->x[1][2] = b_in->x[2][0];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[0][2];
             b_out->x[0][2] = b_in->x[2][1];
             b_out->x[0][1] = b_in->x[2][2];
             b_out->x[0][0] = b_in->x[1][2];
             break;

          case 4: 
             b_out->x[2][2] = b_in->x[0][0];
             b_out->x[2][1] = b_in->x[0][1];
             b_out->x[2][0] = b_in->x[0][2]; 
             b_out->x[1][2] = b_in->x[1][0];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[1][2];
             b_out->x[0][2] = b_in->x[2][0];
             b_out->x[0][1] = b_in->x[2][1];
             b_out->x[0][0] = b_in->x[2][2];
             break;

          case 5:
             b_out->x[2][2] = b_in->x[0][1];
             b_out->x[2][1] = b_in->x[0][2];
             b_out->x[2][0] = b_in->x[1][2]; 
             b_out->x[1][2] = b_in->x[0][0];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[2][2];
             b_out->x[0][2] = b_in->x[1][0];
             b_out->x[0][1] = b_in->x[2][0];
             b_out->x[0][0] = b_in->x[2][1];
             break;

          case 6:
             b_out->x[2][2] = b_in->x[0][2]; 
             b_out->x[2][1] = b_in->x[1][2];
             b_out->x[2][0] = b_in->x[2][2]; 
             b_out->x[1][2] = b_in->x[0][1];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[2][1];
             b_out->x[0][2] = b_in->x[0][0];
             b_out->x[0][1] = b_in->x[1][0];
             b_out->x[0][0] = b_in->x[2][0];
             break;

          case 7: 
             b_out->x[2][2] = b_in->x[1][2]; 
             b_out->x[2][1] = b_in->x[2][2];
             b_out->x[2][0] = b_in->x[2][1]; 
             b_out->x[1][2] = b_in->x[0][2];
             b_out->x[1][1] = b_in->x[1][1];
             b_out->x[1][0] = b_in->x[2][0];
             b_out->x[0][2] = b_in->x[0][1];
             b_out->x[0][1] = b_in->x[0][0];
             b_out->x[0][0] = b_in->x[1][0];
             break;
 
        }
        break;

     case MM_GRAPH:
       if (step != 0 && step != 4) {
         fprintf(stderr, "lvrotstr: the only valid rot. step with graph struct. el. is 0 or 4\n");
         return(0);
       }  
       b_out->g.n = b_in->g.n;
       b_out->g.nodes = (mm_node *) malloc(b_in->g.n * sizeof(mm_node));
       if (b_out->g.nodes == NULL) {
         fprintf(stderr, "lvrotstr: not enough memory\n");
         return(0);
       }
       for (i = 0; i < b_in->g.n; i++) {
         b_out->g.nodes[i].n = b_in->g.nodes[i].n;
         b_out->g.nodes[i].val = b_in->g.nodes[i].val;
         b_out->g.nodes[i].x = b_in->g.nodes[i].x;
         b_out->g.nodes[i].y = b_in->g.nodes[i].y;
         b_out->g.nodes[i].neig = (int *) malloc(b_in->g.nodes[i].n * sizeof(int));
         if (b_out->g.nodes[i].neig == NULL) {
            fprintf(stderr, "lvrotstr: not enough memory\n");
            return(0);
         }
         for (j = 0; j < b_in->g.nodes[i].n; j++)
           b_out->g.nodes[i].neig[j] = b_in->g.nodes[i].neig[j];
       }
       break;
     
     default:
       fprintf(stderr, "lvrotstr: unrecognized struct. el. type\n");
       return(0);
       break;
   }
   return(1);

}
/* -library_code_end */
