/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: union_basis.c
 >>>>
 >>>> Date Last Updated: june 17 1995 
 >>>>
 >>>>          Routines: union_basis.
 >>>>                   
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h> 
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "union_basis.h"
#include "negation_basis.h"

/****************************************************************
*
* Routine Name: union_basis
*
*         Date: june 15 1995
*
*      Purpose: Computa the union of two basis ("basis1" and "basis2"). 
*               The input basis must have equals windows.
*               It is computed made the union of the intervals and 
*               negation two . 
* 
*  Input Rout.: basis1 -> pointer to basis structure, 
*               basis2 -> pointer to basis structure,     
*               basis3 -> pointer to basis structure.    
*
*        Input: "basis1" and "basis2", basis of the operators,
*
*       Output: "basis3", basis of the union of the basis of input. 
*               
*       Return: 1 on success, 0 on failure.                          
*
*  Called from: vunibas.
*
* Rout. called: copy_win, reset_basis, negation_basis, free_itrv.
*
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

int union_basis(basis1,basis2,basis3)
trv_struc *basis1,*basis2,*basis3;

{
 
    int k,i;
    int n4bytes;
    int total;
    int st;
    int nv;
    int ind;
    trv_struc basis4;

    n4bytes = (basis3->win.size + 31)/32;
    total = basis1->total + basis2->total;


    basis3->A = (int *) malloc(sizeof(int) * total * n4bytes);
    if(st == 0){
          fprintf(stderr,"union_basis() : memoria allocation failed\n");
          free_itrv(basis3);
          return(0);
    }

    basis3->B = (int *) malloc(sizeof(int) * total * n4bytes);
    if(st == 0){
          fprintf(stderr,"union_basis() : memoria allocation failed\n");
          free(basis3->A);
          basis3->A = NULL;
          return(0);
    }
    basis3->ind = (int *) malloc(sizeof(int) * total);
    if(st == 0){
          fprintf(stderr,"union_basis() : memoria allocation failed\n");
          free(basis3->A);
          basis3->A = NULL;
          free(basis3->B);
          basis3->B = NULL;
          return(0);
    }

    k = basis1->total * basis1->nb;
    for(i = 0; i < k; i++){
            basis3->A[i] = basis1->A[i];
            basis3->B[i] = basis1->B[i];
    }         
    ind = k;
    
    k = basis2->total * basis2->nb;
    for(i = 0; i < k; i++){
            basis3->A[i + ind] = basis2->A[i];
            basis3->B[i + ind] = basis2->B[i];
    }
    for(i = 0; i < total; i++){
          basis3->ind[i] =  i * n4bytes;
    }

    basis3->nb = n4bytes;
    basis3->nv = basis3->win.size;
    basis3->total = total;
    basis3->size = total;

    reset_basis(&basis4); 
    st = copy_win(&(basis3->win),&(basis4.win));
    if(st == 0){
          fprintf(stderr,"union_basis() : copy_win() failed\n");
          free_itrv(&basis4);
          return(0);
    }

    st = negation_basis(basis3,&basis4);
    if(st == 0){
          fprintf(stderr,"union_basis() : negation3() failed\n");
          free_itrv(&basis4);
          return(0);
    }

    free_itrv(basis3);

    reset_basis(basis3);
    st = copy_win(&(basis4.win),&(basis3->win));
    if(st == 0){
          fprintf(stderr,"union_basis() : copy_win() failed\n");
          free_itrv(&basis4);
          return(0);
    }

    st = negation_basis(&basis4,basis3);
    free_itrv(&basis4);

    if(st == 0){
          fprintf(stderr,"union_basis() : negation3() failed\n");
          return(0);
    }

    return(1);

}   
 

