/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: desc_basis.c
 >>>>
 >>>> Date Last Updated: june 17 1995 
 >>>>
 >>>>          Routines: transfer_basis_according_window, copy_basis, 
 >>>>                    reset_basis, free_list_interval, free_interval, 
 >>>>                    free_vect_basis, alloc_interval, maximal_list, 
 >>>>                    function1, list_interv_to_basis.
 >>>>        
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"

static int  mask[32] =
     { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
       0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 
       0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
       0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 
       0x20000000, 0x40000000, 0x80000000  } ;

/****************************************************************
*
* Routine Name: transfer_basis_according_window
*
*         Date: june 15 1995.
* 
*      Purpose: This routine is used to transfer a basis structure ("basis"),
*               according to window of input ("win_sup").
*               The window of the "basis" must be included window "win_sup".
*
*  Input rout.: basis -> pointer to basis structure,
*               win_sup -> pointer to window struture.
*          
*        Input: "basis", basis input, 
*               "win_sup", widow input
*              
*       output: basis, basis transfer, according to "win_sup". 
*               
*       Return: 1 on success, 0 on failure.                          
*
*  Called from: dilate_basis
*
* Rout. called: calculate_position_win, free_win, copy_win.
*
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

int transfer_basis_according_window(basis,win_sup)
trv_struc *basis; 
win_struc *win_sup;
{
    int i,j,k_1,k_2,k_3;
    int nv;
    int n4bytes_2;
    int *A;
    int *B_c;
    int *position;
    int ind2;
    int st;

    position =(int *)malloc(sizeof(int) * basis->nv);
    if( position == NULL){
        fprintf(stderr, "transf_basis() : Memory allocation failed\n") ;
        return(0);
    }
    
    calculate_position_win (&(basis->win), win_sup, position);
    // printf ("===\n");
    // disp_win (&(basis->win));
    // printf ("\n");
    // disp_win (win_sup);
    // for(j = 0; j < basis->nv;  j++)  printf ("%d ", position[j]);
    // printf ("\n");
    nv = win_sup->size;
    n4bytes_2 = (nv +31)/32;

    A = (int *)malloc(sizeof(int) * basis->total * n4bytes_2);
    if(  A == NULL){
        fprintf(stderr, "transf_basis() : Memory allocation failed\n") ;
        free(position);
        return(0);
    }

    B_c = (int *)malloc(sizeof(int) * basis->total * n4bytes_2);
    if(  B_c == NULL){
        fprintf(stderr, "transf_basis() : Memory allocation failed\n") ;
        free(position);
        free(A);
        return(0);
    }

    for(i = 0; i < basis->total * n4bytes_2; i++){
        A[i] = 0;
        B_c[i] = 0;
    }

    /* transfer each intervals */

    for(i = 0; i < basis->total ; i++){
        for(j = 0; j < basis->nv;  j++){
              k_1 = j % 32;
              k_2 = j / 32 +  basis->ind[i]; 
              if((basis->A[k_2] & mask[k_1]) != 0){
                    k_3 = position[j];
                    ind2 = (i * n4bytes_2) + (k_3 / 32) ;
                    A[ind2] |= mask[k_3 % 32];
              }
              if((basis->B[k_2] & mask[k_1]) != 0){
                    k_3 = position[j];
                    ind2 =( i * n4bytes_2) + (k_3 / 32) ;
                    B_c[ind2] |= mask[k_3 % 32];
              }
        }
    }
    
    free(basis->A);
    free(basis->B);
    basis->A = A;
    basis->B = B_c;
    basis->nv = win_sup->size;
    basis->nb = n4bytes_2;
    basis->size = basis->total;

    for(i = 0 ;i < basis->total;i++){
        basis->ind[i] = i * n4bytes_2;
    }
    
    free_win(&(basis->win));

    st = copy_win(win_sup,&(basis->win));
    if(st == 0) {
        fprintf(stderr, "transf_basis() : copy_win failed\n") ;
        free(position);
        return(0);
    }
    
    free(position);

    return(1);
}
 
/****************************************************************
*
* Routine Name: copy_basis
*
*         Date: june 15 1995
*
*      Purpose: copy a basis.
*
*  Input Rout.: basis1 -> pointer to basis structure,
*               basis2 -> pointer to basis structure. 
*
*        Input: "basis1" to copy.
*          
*       Output: "basis2" copy of the basis of input .
*
*       Return: 1 on success, 0 on failure
*
*  Called from: erosion_basis, dilate_basis 
*
* Rout. called: copy_win.
*
*   Written By: Guillermo Pablo Salas
*        
****************************************************************/

int copy_basis(basis1,basis2)
trv_struc *basis1, *basis2;
{
   int i, j, st;

   basis2->nv = basis1->nv;
   st = basis1->nb;
   basis2->nb = st;
   basis2->total = basis1->total;

   basis2->A = (int *) malloc( sizeof(int) * basis1->nb * basis1->total);
   if(basis2->A == NULL){
      fprintf(stderr,"copy_basis(): memory allocation failed\n");
      return(0);
   }

   basis2->B = (int *) malloc( sizeof(int) * basis1->nb * basis1->total);
   if(basis2->B == NULL){
      fprintf(stderr,"copy_basis(): memory allocation failed\n");
      free(basis2->A);
      return(0);
   }

   basis2->size = basis1->total * basis1->nb;

   basis2->ind  = (int *) malloc(sizeof(int) * basis1->total);
   if(basis2->ind == NULL){
      fprintf(stderr,"copy_basis(): memory allocation failed\n"); 
      free(basis2->A);
      free(basis2->B);
      return(0);
   }

   /* copy each intervalo */
   for( i = 0; i < basis1->total; i++){
       basis2->ind[i] =  basis1->ind[i];
       for(j = 0; j < basis1->nb; j++){
           basis2->A[j + basis1->ind[i]] = basis1->A[j + basis1->ind[i] ];
           basis2->B[j + basis1->ind[i]] = basis1->B[j + basis1->ind[i] ];
        }
   }

   st = copy_win(&(basis1->win), &(basis2->win));
   if(st == 0){
      fprintf(stderr,"copy_basis(): copy_win failed\n");
      free(basis2->A);
      free(basis2->B); 
      free(basis2->ind);
      return(0);
   }

   return(1);
}

/****************************************************************
*
* Routine Name: reset_basis
*
*         Date: june 15 1995
*
*      Purpose: reset a basis structure
*
*  Input Rout.: basis -> pointer basis structure.         
*               
*        Input: "basis" basis to reset. 
*               
*       Output: "basis" basis reseted.        
*
*  Called from: union_basis, erosion_basis
*
* Rout. called: reset_win.
*
*   Written By: Guillermo Pablo Salas
*
****************************************************************/

void reset_basis(basis)
trv_struc *basis;
{
    basis->A = NULL;
    basis->B = NULL;
    basis->ind = NULL;
    basis->size = 0;
    basis->total = 0;
    basis->nb = 0;
    basis->nv = 0;
    reset_win(&(basis->win));
}

/********************* function of list ************************/

/****************************************************************
*
* Routine Name: free_list_interval
*
*         Date: june 15 1995
*
*      Purpose: free a list of intervals from memory.  
*
*  Input Rout.: head -> pointer to interval structure.
*
*        Input: "head" : head of list of intervals.          
*
*  Called from: intersection_basis, negation_basis
*
* Rout. called: free_interval.
*
*   Written By: Guillermo Pablo Salas
*
****************************************************************/

void free_list_interval(head)

itr_struc *head;
{
   itr_struc *P,*Q;

   if(head != NULL){
       P =head->next;
       while(P != NULL){
           Q = P->next;
           free_interval(P);
           free(P);
           P = Q;
       }
   }
   head->next = NULL;
}

/****************************************************************
*
* Routine Name: free_interval
*
*         Date: june 15 1995
*
*      Purpose: free a interval from memory. 
*
*  Input Rout.: itr -> pointer to intervals structure.
*
*        Input: "itr" interval . 
*
*  Called from: free_list_interval, negation_basis.
*
* Rout. called: ---o---.
*
*   Written By: Guillermo Pablo Salas
*        
****************************************************************/

void free_interval(itr)
itr_struc *itr;
{
   if(itr->A != NULL)
        free(itr->A);
   if(itr->B_c != NULL)
        free(itr->B_c);
}

/****************************************************************
*
* Routine Name: free_vect_basis
*               
*
*         Date: june 15 1995
*
*      Purpose: free a vector of basis from memory.
*
*  Input rout.: basis -> pointer to basis structure, 
*                qt -> integer.
*          
*        Input: "basis" vector of basis,
*                "qt"   size of the vector.            
*
*  Called from: erosion_basis, dilate_basis 
*
* Rout. called: free_itrv.
*
*   Written By: Guillermo Pablo Salas
*        
****************************************************************/

void free_vect_basis(basis, qt)
trv_struc *basis;
int qt;
{
   int i;
   for(i = 0; i < qt; i++)     
       free_itrv(basis + i);
}

/****************************************************************
*
* Routine Name: alloc_interval
*               
*         Date: june 15 1995
*
*      Purpose:
*
*  Input rout.:
*
*        Input:
*
*       Output:
*
*       Return: 1 on success, 0 on failure                           
*
*  Called from: negation_basis.
*
* Rout. called: ---o---.
*
*   Written By: Guillermo Pablo Salas
*        
****************************************************************/

int alloc_interval(itr,n4bytes)
itr_struc *itr;
int n4bytes;
{

   itr->A = (int *)malloc(sizeof(int) * n4bytes);

   if(itr->A == NULL){
       fprintf(stderr, "alloc_interval() : Memory allocation failed\n") ;
       return(0);
   }
   itr->B_c = (int *)malloc(sizeof(int) * n4bytes);

    if(itr->B_c == NULL){
       fprintf(stderr, "alloc_interval() : Memory allocation failed\n") ;
       free(itr->A);
       itr->A = NULL; 
       return(0);
   }
   return(1);

}


/****************************************************************
*
* Routine Name: function1
*
*         Date: june 15 1995
*
*      Purpose: comparate the interval "itr2" with the next intervals 
*               in the list :
*                             - remove of the list the intervals included 
*                               in interval "itr2".
*                             - remove the interval "itr2" if it is included
*                               other interval.   
*
*                   itr1        itr2 
*      ______      ______      ______      ______
*     |      |    |      |    |      |    |      | 
* ... |______|--->|______|--->|______|--->|______|---> ....
*
*                                       |---> compare
*
*  Input Rout.: "itr1"   -> pointer interval,
*               "itr2"   -> pointer interval,
*               "total"  -> pointer integer,
*               "n4bytes" ->integer.
*
*        Input: "itr1",   interval anterior to itr2 in the list,
*               "itr2",   interval to comparet
*               "total",  number of intervals of the list.
*               "n4bytes", number of integers for to represent the 
*                          extremities of the intervals,
*       output: lista modify.
*              "total" size of the list modify.
*
*  Called from: intersection_basis, negation_basis.
*
* Rout. called:contido, free_interval.
*
*   Written By: Guillermo Pablo Salas
*
****************************************************************/

void function1(itr1,itr2,total,n4bytes)
itr_struc *itr1,*itr2;
int *total;
int n4bytes;
{
  itr_struc *P,*Q;   
  
  if(itr2 != NULL){
     
      P = itr2->next;
      Q = itr2;
      while(P != NULL){
            if(contido(itr2->A,itr2->B_c,n4bytes,P->A,P->B_c)){ 
                    /* "itr2" is included in the "P"  
                       "itr2" is remove of the list */
                    itr1->next = itr2->next;
                    free_interval(itr2);
                    free(itr2);
                    (*total)--;
                    P = NULL;
            }
            else{
                if(contido(P->A,P->B_c,n4bytes,itr2->A,itr2->B_c)){
                     /* "Q" is included in the "itr2" 
                        "Q" is remeved of the lista */
                     Q->next = P->next;
                     free_interval(P);
                     free(P);
                     (*total)--;
                     P = Q->next;
                 }
                 else{
                      Q = P;
                      P = Q->next;
                 }
            }
       
      }
  }

}

/****************************************************************
*
* Routine Name: list_interv_to_basis 
*
*
*         Date: june 15 1995
*
*      Purpose: this routine transforms a list of intervals in a 
*               structure of basis.
*
*  Input Rout.: basis -> pointer to basis structure,
*               head ->  pointer to intervals struture, 
*               total -> integer,
*               nv ->    integer
* 
*       Input :"head" head of the list of intervals ,
*              "total" number of intervals in the list,
*              "nv" number of variable.
*        
*       Output: "basis" .        
*               
*       Return: 1 on success, 0 on failure
*
*  Called from: intersection_basis, negation_basis.
*
* Rout. called: ---o---.
*
*   Written By: Guillermo Pablo Salas
*        
****************************************************************/

int list_interv_to_basis(head, basis, total, nv)
itr_struc *head;
trv_struc *basis;
int total;
int nv;
{
   int ind;
   int i,k;
   int n4bytes;
   itr_struc *itr;
  
   if(head == NULL){
        fprintf(stderr, "list_interv_to_basis() : list no exist\n");
        return(0);
   }
   
   n4bytes = (nv + 31)/32;
   basis->total = total;
   basis->size = total;
   basis->nv = nv;
   basis->nb = n4bytes;

   basis->A = NULL;
   basis->B = NULL;

   if(total == 0){
                   return(1);
   }
 
   basis->A = (int *)malloc(sizeof(int) * n4bytes * total);
   if(basis->A == NULL){
           fprintf(stderr, "list_interv_to_basis() : memory allocation failed\n");
           return(0);
   }
   
   basis->B = (int *)malloc(sizeof(int) * n4bytes * total);
   if(basis->B == NULL){
           fprintf(stderr, "list_interv_to_basis() : memory allocation failed\n");
           free(basis->A);
           basis->A = NULL;
           return(0);
   }
   basis->ind = (int *)malloc(sizeof(int) * total);
   if(basis->ind == NULL){
           fprintf(stderr, "list_interv_to_basis() : memory allocation failed\n");
           free(basis->A);
           basis->A = NULL;
           free(basis->B);
           basis->B = NULL;
           return(0);
   }
   itr = head;

   for(i = 0; i < total; i++){
            basis->ind[i] = i * n4bytes;
            itr = itr->next;
            for(k = 0; k < n4bytes; k++){
                   basis->A[k + basis->ind[i] ] = itr->A[k];
                   basis->B[k + basis->ind[i] ] = itr->B_c[k];
            }
   }

   return(1);
}
