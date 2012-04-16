/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: desc_win.c
 >>>>
 >>>> Date Last Updated: june 17 1995 
 >>>>
 >>>>          Routines: supremum_win, transfer_win, dilate_win, reset_win,
 >>>>                    free_vect_win, maximo
 >>>>                   
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h>
#include "pac_win.h"
#include "desc_win.h"
#include "mat_str.h"
#include "vec_str.h"

static int  mask[32] =
     { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
       0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 
       0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
       0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 
       0x20000000, 0x40000000, 0x80000000  } ;


/****************************************************************
*
* Routine Name: supremum_win
*
*         Date: june 15 1995
*
*      Purpose: This routine calculate the supremum of two windows.
*
*  Input Rout.:  win1 -> pointer window struture,
*                win2 -> pointer window struture,
*                win3 -> pointer window struture.
*
*        Input: "win1" and "win2" windows.
*               
*       Output: "win3" supremum window of the two windows of input. 
*             
*       Return: 1 on success, 0 on failure.   
*
*  Called from:
*
* Rout. called: maximo, transfer_win.
*
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

int supremum_win(win1,win2,win3)
win_struc *win1,
          *win2,
          *win3;
{
   int i;
   int *data;
   int n4bytes;

   win3->wsize = maximo(win1->wsize,win2->wsize);       
   win3->hsize = maximo(win1->hsize,win2->hsize); 
   n4bytes = ((win3->hsize * win3->wsize) + 31)/32;

   data = (int *) malloc(sizeof(int) * n4bytes);
   if (data == NULL) {
       fprintf(stderr, "supremum_win() : Memory allocation failed\n") ;
       return(0);
   }

   for( i = 0 ;i < n4bytes ;i++)
            data[i] = 0;
  
   win3->size = 0;
   win3->data = data;
   transfer_win(win1,win3);
   transfer_win(win2,win3);
   return(1);

}

/****************************************************************
*
* Routine Name: transfer_win
*
*         Date: june 15 1995
*
*      Purpose: win2 <- (win2 union win1),
*               the dimesions of the "win1" must be less or equal que the
*               dimensions of the win2. 
*      Example:
*
*        input:
* 
*             win1:               win2: 
*                  ---                  -----------
*                 | 1 |                | 0 | 0 | 0 |
*                  ---                  -----------
*                 | 1 |     and        | 1 | 1 | 1 |
*                  ---                  -----------   
*                 | 1 |                | 0 | 0 | 0 |
*                  ---                  -----------
*
*       output:
* 
*            win2:
*                  ----------- 
*                 | 0 | 1 | 0 |
*                  -----------
*                 | 1 | 1 | 1 |
*                  -----------         
*                 | 0 | 1 | 0 | 
*                  -----------
*
*  Input Rout.:  win1 -> pointer window struture,
*                win2 -> pointer window struture,
*
*        Input: "win1" 
*               "win2" 
*               
*       Output: "win2" 
*
*  Called from: supremum_win.
*
* Rout. called: ---o---.
*             
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

void transfer_win(win1,win2)
win_struc *win1,
          *win2;
{
    int k;
    int l;
    int x,y;
    int rect;
    int counter;
    int c_x1,c_y1;
    int c_x2,c_y2;
    int ind;
    
    counter = 0;

    c_x1 = (win1->hsize - 1)/ 2 ;
    c_y1 = (win1->wsize - 1)/ 2 ;
    c_x2 = (win2->hsize - 1)/ 2 ;
    c_y2 = (win2->wsize - 1)/ 2 ;
    
    rect = win1->hsize * win1->wsize;

    for( k = 0 ; k < rect ; k++){
         ind = k/32;
         if( (win1->data[ind] & mask[k % 32] ) != 0 ){
               x = (k % win1->wsize) - c_x1;
               y = (k / win1->hsize) - c_y1;
               x = c_x2 + x;
               y = c_y2 + y;
               l = x + (y * win2->hsize);
               ind = l/32;
               if((win2->data[ind] & mask[l % 32]) == 0){
                     win2->data[ind] |= mask[l % 32];
                     counter++;
               }
          }
    } 
    win2->size+=counter;
}

/****************************************************************
*
* Routine Name: calculate_position_win
*
*         Date: june 15 1995
*
*      Purpose: For each point of the window "win1", compute 
*               your correspondent point in the window "win2".
*               The points of the window "win1" are numerete of 
*               left to right , top to bottom.
*               The windows "win1" must be include in the window
*               win2. 
*
* Example:
*
*       input:
* 
*             win1:               win2: 
*                  ---                  -----------
*                 | 1 |                | 0 | 1 | 0 |
*                  ---                  -----------
*                 | 1 |     and        | 1 | 1 | 1 |
*                  ---                  -----------   
*                 | 1 |                | 0 | 1 | 0 |
*                  ---                  -----------
*
*       output:
*              position[0] = 0, position[1] = 2 , position[2] = 4 
*             
*                
* Input Rout.:  win1 -> pointer window struture,
*               win2 -> pointer window struture,
*               position -> pointer integer.
*
*        Input: "win1"
*               "win2"
*               
*       Output: "position" 
*
*  Called from: transfer_basis_according_window.
*
* Rout. called: ---o---.
*             
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

void calculate_position_win(win1,win2, position)
win_struc *win1,
          *win2;
int *position;
{
    int k1,k2,i;
    int l;
    int n;
    int x,y;
    int rect;
    int c_x1,c_y1;
    int c_x2,c_y2;
    int nv, j, achou;
   
    c_x1 = (win1->hsize - 1) / 2;
    c_y1 = (win1->wsize - 1) / 2;
    c_x2 = (win2->hsize - 1) / 2;
    c_y2 = (win2->wsize - 1) / 2;
    
    rect = win1->hsize * win1->wsize;
    n = 0;

    for (i=0; i<rect; i++) {
         k1 = i % 32;
         k2 = i / 32;
         if( (win1->data[k2] & mask[k1] ) != 0 ){
                 x = (i % win1->wsize) - c_x1;
                 y = (i / win1->hsize) - c_y1;
                 x = c_x2 + x;
                 y = c_y2 + y;
                 l = x + (y * win2->hsize);
                 nv=0; 
                 for(j=0; j<l; j++){
                    k1 = j % 32;  
                    k2 = j / 32;
                    if ((win2->data[k2] & mask[k1] ) != 0 ) nv++;
                 }
                 position[n] = nv;
                 n++;
         }
        
    }

}


/****************************************************************
*
* Routine Name: dilate_win
*
*         Date: june 15 1995
*
*      Purpose: dilate a windows and calculate all the translation
*               of the windows will be dilate.
*
*  Input Rout.:  win1 -> pointer to window struture,
*                ker -> pointer to struturing element,
*                win_dil ->  pointer to window struture,
*                win_vect ->  pointer to window struture,
                 points -> pointer to integer.
*
*        Input:  "win" windows to dilate,
*                "ker" structuring element.
* 
*       Output:  "win_dil" dilated window,
*                "win_vect" vector of the translate window. 
*                "points" number of points of the  structuring element. 
*
*       Return: 1 on success, 0 on failure.   
*
*  Called from: erosion_basis, dilation_basis
*
* Rout. called: free_vect_win, free_win.
*
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

int dilate_win(win, ker, win_dil, win_vect, points)
win_struc *win, *win_dil, *win_vect;
mat_str *ker;
int *points;

{
   int i, j, k, st;
   int m, n, size;
   int m_dil, n_dil, size_dil;
   int *X_win, *Y_win;
   int *X_ker, *Y_ker;
   int x_dil, y_dil;
   int k_1, k_2;

   if(ker == NULL){
      fprintf(stderr, "dilate_win(): can't  struct. el.\n");
      return(0);
   }
   if(ker->tag == MM_GRAPH){
      fprintf(stderr, "dilate_win(): error, struct. el. is type MM_GRAPH.\n");
      return(0);
   }

   size = ker->m * ker->n;
   m = ker->m;
   n = ker->n;

   X_ker = (int *) malloc(sizeof(int) * size);
   if(X_ker == NULL){
      fprintf(stderr,"dilata_win(): memory allocation failed\n");
      return(0);
   }

   Y_ker = (int *) malloc(sizeof(int) * size);
   if(Y_ker == NULL){
      fprintf(stderr,"dilata_win(): memory allocation failed\n");
      free(X_ker);
      return(0);
   }

   (*points) =  0;
   switch(ker->tag){
          case MM_IMG:
              for(i = 0; i < size; i++)
                  if(ker->dat[i] == 1){
		     /* X_ker[(*points)] = i % m - (m-1)/2; */
 		     /* Y_ker[(*points)] = (n-1)/2 - i / n; */
		     X_ker[(*points)] = i%n - (n-1)/2; 
                     Y_ker[(*points)] = (m-1)/2 - i/n;
                     (*points)++;
                  }
              
              break;
          case MM_MAT:
              for(i = 0; i < 3; i++){
                   for(j = 0; j < 3; j++){
                       if(ker->x[i][j] == 1){
                          X_ker[(*points)] = i - 1;
                          Y_ker[(*points)] = 1 - j;
                          (*points)++;   
                       }
                  }
              } 
              break;
   }

   size = win->wsize * win->hsize;
   m = win->wsize;
   n = win->hsize;
   X_win = (int *) malloc(sizeof(int) * size);
   if(X_win == NULL){
      fprintf(stderr,"dilata_win(): memory allocation failed\n");
      free(X_ker);
      free(Y_ker);
      return(0);
   }

   Y_win = (int *) malloc(sizeof(int) * size);
   if(Y_win == NULL){
      fprintf(stderr,"dilata_win(): memory allocation failed\n");
      free(X_win);
      free(X_ker);
      free(Y_ker);
      return(0);
   }
   
   k = 0;
   for(i = 0; i < size; i++){
       k_1 = i / 32;
       k_2 = i % 32; 

       if( ((win->data[k_1]) & (mask[k_2])) != 0){
          X_win[k] = i%m - (m-1)/2; 
          Y_win[k] = (n-1)/2 - i/m;
          k++;   
       } 
   }
 
   win_dil->wsize = win->wsize + 2 * (ker->m/2);
   win_dil->hsize = win->hsize + 2 * (ker->n/2);
   win_dil->size = 0;

   m_dil = win_dil->wsize;
   n_dil = win_dil->hsize;
   size_dil = m_dil * n_dil;
   
   k_1 = (size_dil + 31) / 32;

   win_dil->data = (int *)malloc(sizeof(int) * k_1);
   if(win_dil->data == NULL){
      fprintf(stderr,"dilate_win(): memory allocation failed\n");
      free(X_win);
      free(Y_win);
      free(X_ker);
      free(Y_ker);
      return(0);
   }
   

   for(i = 0; i < (*points); i++){
       win_vect[i].data = (int *)malloc(sizeof(int) * k_1);
       if(win_vect[i].data == NULL){
          fprintf(stderr,"dilate_win(): memory allocation failed\n");
          free(X_win);
          free(Y_win);
          free(X_ker);
          free(Y_ker);
          free_vect_win(win_vect, i);
          free_win(win_dil);
          return(0);
       }
       win_vect[i].wsize = m_dil;         
       win_vect[i].hsize = n_dil;
       win_vect[i].size = win->size;
   }
   
   for(i = 0; i <  (*points); i++){
       for(j = 0; j < k_1 ;j++){
           win_vect[i].data[j] = 0;
       }
   }
   x_dil = (m_dil - 1) / 2;
   y_dil = (n_dil - 1) / 2;

   for(j = 0; j < k_1;j++){
       win_dil->data[j] = 0;  
   }
 
   for(i = 0; i <  (*points); i++){   
        for(j = 0; j < win->size; j++){
            k = ( x_dil + X_win[j] + X_ker[i]) + ( y_dil - Y_win[j] - Y_ker[i]) * m_dil;
           k_1 = k / 32;
           k_2 = k % 32;
           win_vect[i].data[k_1]|= mask[k_2];
           if( (win_dil->data[k_1] & mask[k_2] ) == 0){
              win_dil->data[k_1]|= mask[k_2];
              win_dil->size++;
           }
        }
        // disp_win (& win_vect[i]); printf ("\n");
   }

   // disp_win (win_dil); printf ("\n");

   free(X_win);
   free(Y_win);
   free(X_ker);
   free(Y_ker);
   return(1);
}

/****************************************************************
*
* Routine Name: reset_win
*
*         Date: june 15 1995
*
*      Purpose: reseted a windows structure.
*
*  Input Rout.:  win1 -> pointer window struture,
*
*        Input: "win" windows to reset.
*               
*       Output: "win" reset windows. 
*
*  Called from: erosion_basis
*
* Rout. called: ---o---.
*
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

void reset_win(win)
win_struc *win;
{
  win->data = NULL;
  win->size = 0;
  win->wsize = 0;
  win->hsize = 0;
}

/****************************************************************
*
* Routine Name: free_vect_win
*
*         Date: june 15 1995
*
*      Purpose: free a vector of windows from memory.
*
*  Input Rout.:  win1 -> pointer window struture,
*                win2 -> pointer window struture,
*
*        Input: "win" vector of windows.
*               "qt" size of the vector.
*       Output:  ----o-----.
*
*  Called from: dilate_win, erosion_basis 
*
* Rout. called: free_win.
*
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/
 
void free_vect_win(win, qt)
win_struc *win;
int qt;
{
   int i;

   for(i = 0; i < qt; i++)     
       free_win(win + i);
}

/****************************************************************
*
* Routine Name: maximo
*
*         Date: june 15 1995
*
*      Purpose: calculate of maximo of two integer.
*
*  Input Rout.: a -> integer, b -> integer.
*
*        Input: a and b the integer to calculate of maximo.
*
*       Return: a if a > b, b otherwise.   
*
*  Called from: supremum_win 
*  
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/
         
int maximo(a,b)
int a,b;
{
  if(a > b) 
          return(a);
  return(b);
}    
        


