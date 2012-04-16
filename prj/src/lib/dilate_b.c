/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: dilate_basis.c
 >>>>
 >>>> Date Last Updated: june 17 1995 
 >>>>
 >>>>          Routines: dilate_basis.
 >>>>                   
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h>
#include "dilate_basis.h"
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "negation_basis.h"
#include "mat_str.h"
#include "vec_str.h"

/****************************************************************
*
* Routine Name: dilate_basis 
*
*         Date: june 15 1995
*
*      Purpose: Compute the basis of the composition of the operator,
*               whose basis is "basis1", with the dilation operator, whose
*               structuring element is "str". 
*
*  Input Rout.: basis1 -> pointer to the basis structure, 
*               str -> pointer to the structuring element,
*               basis2 -> pointer to the basis structure     
*
*        Input: "basis1", basis of the operator,
*               "str", structuring element of the dilation operator. 
*
*       Output: "basis2", basis of the composition with the dilation
*               operator. 
*               
*       Return: 1 on success, 0 on failure.                          
*
*  Called from:
*
* Rout. called:  lvrotstr, dilate_win, free_win, free_vect_win, copy_basis, 
*                free_vect_basis, transfer_basis_according_window, copy_win, 
*                negation_basis, free_itrv.

*
*   Written By: Guillermo Pablo Salas
*        
****************************************************************/

int dilate_basis(basis1,str,basis2)
trv_struc *basis1, *basis2;
mat_str *str;
{
   win_struc  win_dil, *win_vector;
   trv_struc  *basis_corr, *basis_vector, basis11, basis22;
   mat_str str_rot; 
   int points;
   int i, j, k;
   int st;
   int *ba, *be;
   
   /* transposed structuring element */
 
   st = lvrotstr(str,4,0,&str_rot);
   if(st == 0){
      fprintf(stderr,"dilate_basis(): lvrotstr() failed\n");
      return(0);
   } 
 
   win_vector = (win_struc *) malloc(sizeof(win_struc) * str_rot.m * str_rot.n);
   if(win_vector == NULL){
      fprintf(stderr,"dilate_basis(): lvrotstr() failed\n");
      free_str(&str_rot);
      return(0);
   }
   
   /* dilate the window of input basis and 
      vector  translate the window of the basis input for all points 
      of the input structuring element (transposed). In "win_vector" 
      win_dil : dilated window
      win_vector : vector of window of the basis input translate
      points : number of points of the structuring element */ 

   st = dilate_win(&(basis1->win), &str_rot, &win_dil, win_vector, &points);

   free_str(&str_rot);
   if(st == 0){
      fprintf(stderr,"dilate_basis(): dilate_win() failed\n");
      free(win_vector);
      return(0);
   }

   if(points == 0){
         /*structuring element empty */ 
         basis2->A = NULL;
         basis2->B = NULL;
         basis2->ind = NULL;
         basis2->size = 0;
         basis2->total = 0;
         basis2->nv = 0;
         basis2->nb = 0;
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
      (transpote).
      "basis_vector" : vector of the translated input basis*/
 
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
       if(st == 0){
          fprintf(stderr,"dilate_basis(): transf2_basis() failed\n");
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

   basis11.total =  points * basis1->total;
   basis11.nb = basis_vector[0].nb;
   basis11.nv = basis_vector[0].nv;
   basis11.size  =  basis11.total * basis11.nb;

   basis11.A = (int *)malloc(sizeof(int) * basis11.size);
   if(basis11.A == NULL){
      fprintf(stderr, "dilate_basis(): memory allocation failed\n");
      free_vect_basis(basis_vector,points);
      free(basis_vector);
      free_win(&win_dil);
      return(0);
   }

   basis11.B = (int *)malloc(sizeof(int) * basis11.size);
   if(basis11.B == NULL){
      fprintf(stderr, "dilate_basis(): memory allocation failed\n");
      free_vect_basis(basis_vector,points);
      free(basis_vector); 
      free(basis11.A);
      free_win(&win_dil);
      return(0);
   }

   /*union of the vector of the translated basis */

   for(i = 0; i < points; i++){
       k = i * basis_vector[i].total * basis_vector[i].nb;
       for(j = 0; j < basis_vector[i].total * basis_vector[i].nb; j++){
           basis11.A[j + k] = basis_vector[i].A[j]; 
           basis11.B[j + k] = basis_vector[i].B[j];
       }
   } 

   free_vect_basis(basis_vector,points);
   free(basis_vector); 

   basis11.ind = (int *)malloc(sizeof(int) * basis11.total);
   if(basis11.ind == NULL){
      fprintf(stderr, "dilate_basis(): memory allocation failed\n"); 
      free(basis11.A);
      free(basis11.B);
      free_win(&win_dil);
      return(0);
   }


   for(i = 0; i < basis11.total; i++){
       basis11.ind[i] = i * basis11.nb;
   }
 
   st = copy_win(&win_dil, &(basis11.win));
   free_win(&win_dil);
   if(st == 0){
      fprintf(stderr,"dilate_basis():copy_win() failed\n");    
      free(basis11.A);
      free(basis11.B);     
      free(basis11.ind);
      return(0);
   }  

   st = copy_win(&(basis11.win), &(basis22.win));
   if(st == 0){
       fprintf(stderr,"dilate_basis():copy_win() failed\n");    
       free_itrv(&basis11);     
       return(0);
   }

  st = negation_basis(&basis11, &basis22);

  free_itrv(&basis11);     
  if(st == 0){
       fprintf(stderr,"dilate_basis():negation() failed\n");    
       return(0);
   }

   st = copy_win(&(basis22.win), &(basis2->win));
   if(st == 0){
       fprintf(stderr,"dilate_basis():copy_win() failed\n");    
       free_itrv(&basis22);     
       return(0);
   }

   st = negation_basis(&basis22, basis2);
  
   free_itrv(&basis22);     
   if(st == 0){
       fprintf(stderr,"dilate_basis():negation() failed\n");    
       free_itrv(basis2);
       return(0);
   } 
 
   return(1);
}

