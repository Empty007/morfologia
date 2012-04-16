 /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>
   >>>>         File Name: erosion_basis.c
   >>>>
   >>>> Date Last Updated: june 17 1995
   >>>>
   >>>>          Routines: erosion_basis.
   >>>>
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h>
#include "erosion_basis.h"
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "intersection_basis.h"
#include "mat_str.h"
#include "vec_str.h"

/****************************************************************
*
* Routine Name: erosion_basis 
*
*         Date: june 15 1995
*
*      Purpose: Compute the basis of the composition of operator
*               with basis "basis1" with the erosion operator with
*               structuring element "str". 
*
*  Input Rout.: basis1 -> pointer to basis structure, 
*               str -> pointer to structuring element,
*               basis2 -> pointer to basis structure     
*
*        Input: "basis1", basis of the operator,
*               "str", structuring element of the erosion operator. 
*
*       Output: "basis2", basis of the composition with the erosion
*               operator. 
*               
*       Return: 1 on success, 0 on failure.                          
*
*  Called from:
*
* Rout. called: dilate_win, copy_basis, free_win, free_vect_win,
*               free_vect_basis, free_win, copy_win, alloc_itrv
*               intersection_basis, free_itrv, reset_basis
*
*   Written By: Guilermo Pablo Salas
*
****************************************************************/

int erosion_basis(basis1, str, basis2)
trv_struc *basis1, *basis2;
mat_str *str;
{

   win_struc  win_dil, *win_vector;
   trv_struc  *basis_corr, *basis_vector, basis11, basis22;
   int points;
   int i, j, k;
   int st;

   win_vector = (win_struc *) malloc(sizeof(win_struc) * str->m * str->n);
   if(win_vector == NULL){
      fprintf(stderr,"dilate_basis(): lvrotstr() failed\n");
      return(0);
   }
   /* dilate the window of input basis and 
      vector  translate the window of the basis input for all points 
      of the input structuring element . In "win_vector" 
      win_dil : dilated window
      win_vector : vector of window of the basis input translate
      points : number of points of the structuring element */ 
  
   st = dilate_win(&(basis1->win), str, &win_dil, win_vector, &points);
   if(st == 0){
      free(win_vector);
      fprintf(stderr,"erosin_basis(): dilate_win() failed\n");
      return(0);
   }

   if(points == 0){
         /*structuring element empty */ 
         st = copy_win(&(basis1->win), &(basis2->win));
         if(st == 0){
            fprintf(stderr,"dilate_basis():copy_win() failed\n");    
            return(0);
         }
                
         st = alloc_itrv(basis2, basis2->win.size, 1);
         if(st == 0){
            fprintf(stderr,"dilate_basis():copy_win() failed\n");    
            free_win(&(basis2->win));
            return(0);
         }

         for( i = 0; i < basis2->nb; i++){
             basis2->A[i] = 0;
             basis2->B[i] = 0;
         }
         basis2->ind[0] = 0;
         basis2->total = 1;
         return(1);
   }

   basis_vector = (trv_struc *)malloc( sizeof(trv_struc) * points);
   if(basis_vector == NULL){
      fprintf(stderr, "dilate_basis(): memory allocation failed\n");
      free_win(&win_dil); 
      free_vect_win(win_vector, points);
      free(win_vector);
      return(0);
   }

   /* translate the input basis for each point of the input structuring element
      "basis_vector" : vector of the translated input basis */
 
   for(i = 0; i < points; i++){
     st = copy_basis(basis1, &(basis_vector[i]));
       if(st == 0){
          fprintf(stderr,"dilate_basis(): copy_basis() failed\n");
          free_win(&win_dil);
          free_vect_win(win_vector, points);
          free_vect_basis(basis_vector, i);
          free(basis_vector);
          free(win_vector);
          return(0);
       }

       free_win( &(basis_vector[i].win));
       copy_win(&(win_vector[i]), &(basis_vector[i].win));
       if(st == 0){
          fprintf(stderr,"dilate_basis(): copy_basis() failed\n");
          free_itrv(&basis_vector[i]);
          free_win(&win_dil); 
          free_vect_win(win_vector, points);
          free_vect_basis(basis_vector, i);
          free(basis_vector);  
          free(win_vector);
          return(0);
       } 
   }

   
   /* transfer the structure of the vector of translated basis "basis_vector"
      according to the window "win_dil" */ 

   for(i = 0; i < points; i++){
       st = transfer_basis_according_window(&(basis_vector[i]), &(win_dil));
       // printf ("%d/%d st = %d\n", i, points-1, st);
       // disp_basis_by_ron (&(basis_vector[i]));
       // disp_win (&basis_vector[i].win); printf ("\n");
       if(st == 0){
          fprintf(stderr,"erosin_basis(): transf2_basis() failed\n");
          free_win(&win_dil);
          free_vect_win(win_vector, points);
          free_vect_basis(basis_vector, points);
          free(basis_vector); 
          free(win_vector);
          return(0);
       }
   }
 
   free_vect_win(win_vector, points);
   free(win_vector);
    
   basis11.A = basis_vector[0].A;
   basis11.B = basis_vector[0].B;
   basis11.ind = basis_vector[0].ind;
   basis11.nv = basis_vector[0].nv;
   basis11.nb = basis_vector[0].nb;
   basis11.total = basis_vector[0].total;
   basis11.size = basis_vector[0].size; 

   reset_basis(&basis22);

   for(i = 1; i < points; i++){
 
       st = intersection_basis(&basis11, &(basis_vector[i]), &basis22);
       if(st == 0){
          fprintf(stderr,"erosin_basis(): intersection() failed\n");
          free_vect_basis(basis_vector + i, points);
          free_win(&win_dil);
          free(basis_vector); 
          return(0);
       }

       free_itrv(basis_vector + i);
       free(basis11.A);
       free(basis11.B);
       free(basis11.ind);

       basis11.A = basis22.A;
       basis11.B = basis22.B;
       basis11.ind = basis22.ind;
       basis11.size = basis22.size;
       basis11.total = basis22.total;
   }

   free(basis_vector); 
   basis2->A = basis11.A;
   basis2->B = basis11.B;
   basis2->ind = basis11.ind;
   basis2->nv = basis11.nv;
   basis2->nb = basis11.nb;
   basis2->size = basis11.size;
   basis2->total = basis11.total;

   st = copy_win(&win_dil, &(basis2->win));
   free_win(&win_dil);
   if(st == 0){
       fprintf(stderr,"erosion_basis():copy_win() failed\n");
       free(basis11.A);
       free(basis11.B);
       free(basis11.ind); 
       return(0);
   }
   return(1);
}

