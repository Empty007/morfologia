/*
 *--------------------------------------------------------------------------
 * COPYRIGHT
 *
 * A Mathematical Morphology Toolbox for the Khoros System 
 *
 * Copyright 1993 - University of Sao Paulo - USP
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
 >>>>         File Name: tools.c
 >>>>
 >>>> Date Last Updated: January 10 1995 
 >>>>
 >>>>          Routines: breverse, bitreverse, format, unformat,
 >>>>                    read_str, write_str, free_str, mat2vec_str,
 >>>>                    copyimagedata1, setimage;
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include "vinclude.h"
#include <stdio.h>
#include "mat_str.h"
#include "vdefines.h"	 	
#include "viff.h"	 	
#include "vec_str.h"

/****************************************************************
*
* Routine Name: breverse
*               This routine is used in the format and unformat routines.
*
*         Date: September 24 1993
*
*      Purpose: reverse bits in 8 bit clusters (32 bit words) 
*
*   Input data: bit image data formatted as following:
*               7..0 15..8 23..16 31..24  (32 bit word)
*          
*  Output data: bit image data formatted as following:    
*               0..7 8..15 16..23 24..31  (32 bit word)
*
*        Input: l2 - pointer to BIT imagedata
*               nr - total number of rows (of all image bands)
*               col32 - number columns of 32 bits
*
*   Written By: Adriano Nagelschmidt Rodrigues & Luiz Fernando Vieira
*        
****************************************************************/

void breverse(l2, nr, col32)
unsigned long *l2;
int nr, col32;
{
  int i, j, jj, count, row;
  unsigned long xx, mask, aux;

  row = 0;         
  for (i=0; i<nr; i++) {
    for (j=0; j<col32; j++) {
      jj = row + j;
      xx = 0;
      mask = 0x1010101;
      for (count=7; count>=-7; count-=2) {
        aux = mask & l2[jj];
        if (count>0) aux <<= count;
        else aux >>= (-count);
        xx |= aux;
        mask <<= 1;             
      }
      l2[jj] = xx; 
    }
    row += col32;
  }
}


/***********************************************************************
*
*  Routine Name: format
*
*          Date: October 20 1993
*        
*       Purpose: transforms image width to 32-multiple ready to be
*	  	 processed in 32 bit words  
*
*         Input: viff image
*
*        Output: 32-multiple number of columns viff image
*                right border mask (to the rightmost pixel) 
*
*    Written By: Luiz Fernando Vieira & Adriano Nagelschmidt Rodrigues  
*
* Modifications:
*
***********************************************************************/

struct xvimage *format(img1, border)
struct xvimage *img1;
unsigned long *border;
{
  struct xvimage *imgout, *createimage();
  int col32,		/* number of columns of 32 pixels */
      rwtot;            /* total number of rows */
  int nbl,              /* number of bytes per line */
      tsize,            /* total size of output image */
      new_row,          /* number of columns of output image */
      tofill,           /* number of bytes to fill */
      i, j;             /* auxiliar */
          
  rwtot = img1->col_size*img1->num_data_bands;
  col32 = (img1->row_size + 31) >> 5; 
         
  if (!(img1->row_size % 32))
    *border = 1;
  else
    *border = ((unsigned long) 0x80000000L) >> ((img1->row_size % 32) - 1);

  if (((img1->row_size % 32)<25) && (img1->row_size % 32)) {

    nbl = img1->row_size/8 + ((img1->row_size % 8) ? 1 : 0);
    tsize = nbl * rwtot;
    tofill = 4 - (nbl % 4);
    new_row = col32 * 32;

    imgout = createimage(img1->col_size, new_row, img1->data_storage_type, img1->num_of_images, img1->num_data_bands, img1->comment, img1->map_row_size, img1->map_col_size, img1->map_scheme, img1->map_storage_type, img1->location_type, img1->location_dim);

    if (imgout == NULL) {
      (void) fprintf(stderr, "createimage Failed\n");
      exit(1);
    }

    j = 1;
    imgout->imagedata[0] = img1->imagedata[0];
    for (i=1; i<tsize; i++) {
      if (!(i % nbl))
        j += tofill;
      imgout->imagedata[j++] = img1->imagedata[i];
    }
    (void) breverse((unsigned long *)imgout->imagedata, rwtot, col32);
    return(imgout); 
  }
  else {
    (void) breverse((unsigned long *)img1->imagedata, rwtot, col32);
    return(img1);
  }
}

/***********************************************************************
*
*  Routine Name: unformat
*
*          Date: October 20 1993
*        
*       Purpose: undo modifications made by the format routine 
*
*         Input: formatted size image (output of the above routine)
*
*        Output: original size image
*
*    Written By: Luiz Fernando Vieira & Adriano Nagelschmidt Rodrigues 
*
* Modifications:
*
***********************************************************************/

void unformat(img1, img2)
struct xvimage *img1, *img2;
{
  int col32,		/* number of columns of 32 pixels */
      rwtot;            /* total number of rows */
  int nbl,              /* number of bytes per line */
      tsize,            /* total size of output image */
      tofill,           /* number of bytes to fill */
      i, j;             /* auxiliar */
  
  rwtot = img1->col_size*img1->num_data_bands;
  col32 = (img2->row_size + 31) >> 5;

  (void) breverse((unsigned long *)img1->imagedata, rwtot, col32);

   if (img1 != img2) {
     nbl = img2->row_size/8 + ((img2->row_size % 8) ? 1 : 0);
     tsize = nbl * rwtot;
     tofill = 4 - (nbl % 4);
     j = 1;
     img2->imagedata[0] = img1->imagedata[0];
     for (i=1; i<tsize; i++) {
       if (!(i % nbl))
         j += tofill;
       img2->imagedata[i] = img1->imagedata[j++];
     }
   }
}


/********************************************************
*
* Routine Name: read_str
*
*      Purpose: read a structuring element file. 
*
*        Input: file name
*
*       Output: structuring element in the generic form.
*      
*       Return: 1 on success, 0 on failure,  
* 
*   Written by: Adriano Nagelschmidt Rodrigues
*
**********************************************************/

int 
read_str(filename, ker)
char *filename;
mat_str *ker;
{
   char buf[512];
   char sig[20];
   int i, j, size;
   int in;
   FILE *file;
   
   /* get file complete path */
   if (!vfullpath(filename, NULL, buf))
      return(0);

   if ((file = fopen(buf, "r")) == NULL) {
      fprintf(stderr, "read_str: cannot open file\n");
      return(0);
   }

   fscanf(file, "%s\n", sig);
   if (strcmp(sig, "MM_STRUCT") != 0) {
      fprintf(stderr, "read_str: not a MMach structuring el. file\n");
      fclose(file);
      return(0);
   }

   /* read str type */
   fscanf(file, "%d", &ker->tag);

   /* read dimensions */
   fscanf(file, "%d", &ker->m);
   fscanf(file, "%d", &ker->n);
   fscanf(file, "%d\n", &ker->p); 

   switch (ker->tag) {
      case MM_IMG:
         size = ker->m * ker->n * ker->p;
         ker->dat = (short *) malloc(size * sizeof(short));
         for (i = 0; i < size; i++)
            fscanf(file, "%hd", &ker->dat[i]);
         break;
      case MM_MAT:
         for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++) {
               fscanf(file, "%d", &in);
               ker->x[j][i] = (in ? 1 : 0);
            } 
         break;
      case MM_GRAPH:
         fscanf(file, "%d\n", &ker->g.n);
         ker->g.nodes = (mm_node *) malloc(ker->g.n * sizeof(mm_node));
         if (ker->g.nodes == NULL) {
            fprintf(stderr, "read_str: not enough memory\n");
            return(0);
         }
         /* read nodes */
         for (i = 0; i < ker->g.n; i++) {
            fscanf(file, "%d", &ker->g.nodes[i].n);
            fscanf(file, "%d", &ker->g.nodes[i].val);
            fscanf(file, "%d", &ker->g.nodes[i].x);
            fscanf(file, "%d", &ker->g.nodes[i].y);
            ker->g.nodes[i].neig = (int *) malloc(ker->g.nodes[i].n * sizeof(int));
            if (ker->g.nodes[i].neig == NULL) {
               fprintf(stderr, "read_str: not enogh memory\n");
               return(0);
            }
            for (j = 0; j < ker->g.nodes[i].n; j++)
               fscanf(file, "%d", &ker->g.nodes[i].neig[j]);
         }
         break;
   }

   fclose(file);
   return(1);
}


/********************************************************
*
* Routine Name: write_str
*
*      Purpose: write a structuring element file. 
*
*        Input: structuring element
*
*       Output: file
*      
*       Return: 1 on success, 0 on failure,  
* 
*   Written by: Adriano Nagelschmidt Rodrigues
*
**********************************************************/

int 
write_str(filename, ker)
char *filename;
mat_str *ker;
{
   char buf[512];
   int i, j, size;
   FILE *file;
   
   /* get file complete path */
   if (!vfullpath(filename, NULL, buf))
      return(0);

   if ((file = fopen(buf, "w")) == NULL) {
      fprintf(stderr, "write_str: cannot create file\n");
      return(0);
   }

   /* file signature */
   fprintf(file, "MM_STRUCT\n");

   /* write str type */
   fprintf(file, "%d ", ker->tag);

   /* write dimensions */
   fprintf(file, "%d ", ker->m);
   fprintf(file, "%d ", ker->n);
   fprintf(file, "%d\n", ker->p); 

   switch (ker->tag) {
      case MM_IMG:
         size = ker->m * ker->n * ker->p;
         for (i = 0; i < size; i++)
            fprintf(file, "%hd ", ker->dat[i]);
         break;
      case MM_MAT:
         for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
               fprintf(file, "%d ", ker->x[j][i]);
         break;
      case MM_GRAPH:
         fprintf(file, "%d\n", ker->g.n);
         /* write nodes */
         for (i = 0; i < ker->g.n; i++) {
            fprintf(file, "%d ", ker->g.nodes[i].n);
            fprintf(file, "%d ", ker->g.nodes[i].val);
            fprintf(file, "%d ", ker->g.nodes[i].x);
            fprintf(file, "%d ", ker->g.nodes[i].y);
            for (j = 0; j < ker->g.nodes[i].n; j++)
               fprintf(file, "%d ", ker->g.nodes[i].neig[j]);
            fprintf(file, "\n");
         }
         break;
   }

   fclose(file);
   return(1);
}


/********************************************************
*
* Routine Name: free_str
*
*      Purpose: free a structuring element from memory. 
*
*        Input: structuring element
*
*       Output: none
*      
*       Return: 1 upon success, 0 on failure  
* 
*   Written by: Adriano Nagelschmidt Rodrigues
*
**********************************************************/

int 
free_str(ker)
mat_str *ker;
{
   int i;
   
   if (ker == NULL) {
      fprintf(stderr, "free_str: can't free struct. el.\n");
      return(0);
   }
   
   switch (ker->tag) {
      case MM_IMG:
         free(ker->dat);
         break;
      case MM_GRAPH:
         for (i = 0; i < ker->g.n; i++)
            free(ker->g.nodes[i].neig);
         free(ker->g.nodes);
         break;
   }
}


/********************************************************
*
* Routine Name: mat2vec_str
*
*      Purpose: Converts a structuring element from the matrix form to the
*               vector form.
*
*        Input: structuring element in the mat form.
*
*               number of the image region (1 to 9)
*
*                   1 !       2         ! 3
*                 -------------------------
*                     !                 ! 
*
*                   4 !       5         ! 6 
*
*                     !                 !
*                 -------------------------
*                   7 !       8         ! 9        
*
*      Output: structuring element in the vectorial form. 
*
*      Return: 1 on success, 0 on failure.
*
*  Written By: Junior Barrera
*
****************************************************************/

mat2vec_str(b_mat, region, b_vec)
mat_str *b_mat;
int region; 
vec_str *b_vec;

{
 int i, j;
 int imin, imax;
 int jmin, jmax;

 switch(region) {

    case 1: imin = 1;
            imax = 2;
            jmin = 1;
            jmax = 2;
            break;

    case 2: imin = 0;
            imax = 2;
            jmin = 1;
            jmax = 2;
            break;
      
   case 3:  imin = 0;
            imax = 1;
            jmin = 1;
            jmax = 2;
            break;

     
   case 4:  imin = 1;
            imax = 2;
            jmin = 0;
            jmax = 2;
            break;

     
   case 5:  imin = 0;
            imax = 2;
            jmin = 0;
            jmax = 2;
            break;

     
   case 6:  imin = 0;
            imax = 1;
            jmin = 0;
            jmax = 2;
            break;

     
   case 7:  imin = 1;
            imax = 2;
            jmin = 0;
            jmax = 1;
            break;

     
   case 8:  imin = 0;
            imax = 2;
            jmin = 0;
            jmax = 1;
            break;

   case 9:  imin = 0;
            imax = 1;
            jmin = 0;
            jmax = 1;
            break;
  }


b_vec->n = 0;

for (i = imin; i <= imax; i++)
  for (j = jmin; j <= jmax; j++) { 

      if (b_mat->x[i][j] == 1) {
          switch(i) {

             case 0:  switch(j) {

                        case 0:  b_vec->x[b_vec->n] = -1;
                                 b_vec->y[b_vec->n] = 1;
                                 break;

                        case 1:  b_vec->x[b_vec->n] = -1;
                                 b_vec->y[b_vec->n] = 0;
                                 break;

                        case 2:  b_vec->x[b_vec->n] = -1;
                                 b_vec->y[b_vec->n] = -1;
                                 break;
                       }
                       break; 



             case 1:  switch(j) {

                        case 0:  b_vec->x[b_vec->n] = 0;
                                 b_vec->y[b_vec->n] = 1;
                                 break;

                        case 1:  b_vec->x[b_vec->n] = 0;
                                 b_vec->y[b_vec->n] = 0;
                                 break;

                        case 2:  b_vec->x[b_vec->n] =  0;
                                 b_vec->y[b_vec->n] = -1;
                                 break;
                       }
                       break; 


             case 2:  switch(j) {

                        case 0:  b_vec->x[b_vec->n] = 1;
                                 b_vec->y[b_vec->n] = 1;
                                 break;

                        case 1:  b_vec->x[b_vec->n] = 1;
                                 b_vec->y[b_vec->n] = 0;
                                 break;

                        case 2:  b_vec->x[b_vec->n] = 1;
                                 b_vec->y[b_vec->n] = -1;
                                 break;
                       }
                       break; 

             }

       b_vec->n = (b_vec->n) + 1;
    }
  }
  return (1);
}  




/**************************************************************
*
* Routine Name: copyimagedata1
*
*      Purpose: Copy image data
*
*        Input: img1 -- original image
*
*       Output: img2 -- new image
*
*       Return: 1 upon success and 0 upon failure.
* 
*   Written by: Junior Barrera
*               Adriano 
*
**************************************************************/

copyimagedata1(img1, img2)
struct
xvimage	*img1,
	*img2;
{
long	lsize;
int     i, n;

/* room for the image data */

    switch (img1->data_storage_type)
    {
	case VFF_TYP_BIT:
	    lsize = ((img1->row_size+7)/8)*img1->col_size;
	    break;
	case VFF_TYP_1_BYTE:
	    lsize = img1->col_size*img1->row_size;
	    break;
	case VFF_TYP_2_BYTE:
	    lsize = img1->col_size*img1->row_size*sizeof(short);
	    break;
	case VFF_TYP_4_BYTE:
	    lsize = img1->col_size*img1->row_size*sizeof(int);
	    break;
	case VFF_TYP_FLOAT:
	    lsize = img1->col_size*img1->row_size*sizeof(float);
	    break;
	case VFF_TYP_DOUBLE:
	    lsize = img1->col_size*img1->row_size*sizeof(double);
	    break;
	case VFF_TYP_COMPLEX:
	    lsize = img1->col_size*img1->row_size*sizeof(float)*2;
	    break;
	case VFF_TYP_DCOMPLEX:
	    lsize = img1->col_size*img1->row_size*sizeof(double)*2;
	    break;
	default:
	    (void) fprintf(stderr,"copyimagedata: Not valid data \
storage type\n");
	    return(0);
	    break;
    }
    lsize = lsize*img1->num_of_images*img1->num_data_bands;

    bcopy(img1->imagedata, img2->imagedata, (int) lsize);

    img2->data_storage_type = img1->data_storage_type;
    img2->row_size = img1->row_size;
    img2->col_size = img1->col_size;
    img2->num_of_images = img1->num_of_images;
    img2->num_data_bands = img1->num_data_bands;

    /* copy location data */
    if (img1->location_type == VFF_LOC_EXPLICIT) {
       n = img1->col_size * img1->row_size * img1->location_dim;
       for (i = 0; i < n; i++)
          img2->location[i] = img1->location[i];
    }  

    return(1);
}

/**************************************************************
*
* Routine Name: setimage
*
*      Purpose: write maxval to all image pixels
*
*        Input: img -- xv image
*
*       Output: img -- set xv image
*
* 
*   Written by: Adriano Nagelschmidt Rodrigues
*
**************************************************************/

int setimage(img)
struct xvimage *img;
{
  int    j, col32, k;
  unsigned char *c;
  unsigned long *l;

  switch (img->data_storage_type) {
    case VFF_TYP_BIT :
      col32 = (img->row_size + 31) >> 5;
      k = img->col_size * col32;
      l = (unsigned long *)(img->imagedata);
      for(j=0; j<k; j++) {
        *l = 0xffffffffL;
        l++;
      }
      return(1);
      break;

    case VFF_TYP_1_BYTE : 
      k = img->row_size * img->col_size * img->num_data_bands;
      c = (unsigned char *)(img->imagedata);     
      for (j=0; j<k; j++) {
        *c = 255;
        c++;
      }     
      return(1);
      break;
 
    case VFF_TYP_2_BYTE :
      fprintf(stderr,"setimage: BYTE2 not implemented\n");
      return(0);
      break;

    case VFF_TYP_4_BYTE :
      fprintf(stderr,"setimage: BYTE4 not implemented\n");
      return(0);
      break;

    case VFF_TYP_FLOAT :
      fprintf(stderr,"setimage: FLOAT not implemented\n");
      return(0);
      break;

    case VFF_TYP_COMPLEX :
      fprintf(stderr,"setimage: COMPLEX not implemented\n");
      return(0);
      break;

    case VFF_TYP_DOUBLE :
      fprintf(stderr,"setimage: DOUBLE not implemented\n");
      return(0);
      break;

    case VFF_TYP_DCOMPLEX :
      fprintf(stderr,"setimage: COMPLEXD not implemented\n");
      return(0);
      break;

    default:
      fprintf(stderr,"setimage: Unknown data storage type, %d\n",img->data_storage_type);
      return(0);
      break;
  }
}

/**************************************************************
*
* Routine Name: clearimage
*
*      Purpose: write 0 to all image pixels
*
*        Input: img -- xv image
*
*       Output: img -- cleared xv image
*
* 
*   Written by: Adriano Nagelschmidt Rodrigues
*
**************************************************************/

int clearimage(img)
struct xvimage *img;
{
  int    j, col32, k;
  short *s;
  unsigned char *c;
  unsigned long *l;

  switch (img->data_storage_type) {
    case VFF_TYP_BIT :
      col32 = (img->row_size + 31) >> 5;
      k = img->col_size * col32;
      l = (unsigned long *)(img->imagedata);
      for(j=0; j<k; j++)
        l[j] = 0;
      return(1);
      break;

    case VFF_TYP_1_BYTE : 
      k = img->row_size * img->col_size * img->num_data_bands;
      c = (unsigned char *)(img->imagedata);     
      for (j=0; j<k; j++)
        c[j] = 0;
      return(1);
      break;
 
    case VFF_TYP_2_BYTE :
      k = img->row_size * img->col_size * img->num_data_bands;
      s = (short *)(img->imagedata);     
      for (j=0; j<k; j++)
        s[j] = 0;
      return(1);
      break;

    case VFF_TYP_4_BYTE :
      fprintf(stderr,"setimage: BYTE4 not implemented\n");
      return(0);
      break;

    case VFF_TYP_FLOAT :
      fprintf(stderr,"setimage: FLOAT not implemented\n");
      return(0);
      break;

    case VFF_TYP_COMPLEX :
      fprintf(stderr,"setimage: COMPLEX not implemented\n");
      return(0);
      break;

    case VFF_TYP_DOUBLE :
      fprintf(stderr,"setimage: DOUBLE not implemented\n");
      return(0);
      break;

    case VFF_TYP_DCOMPLEX :
      fprintf(stderr,"setimage: COMPLEXD not implemented\n");
      return(0);
      break;

    default:
      fprintf(stderr,"setimage: Unknown data storage type, %d\n",img->data_storage_type);
      return(0);
      break;
  }
}
