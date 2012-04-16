/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: intersection_basis.c
 >>>>
 >>>> Date Last Updated: june 17 1995 
 >>>>
 >>>>          Routines: intersection_basis.
 >>>>                   
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "intersection_basis.h"


/****************************************************************
*
* Routine Name: intersection_basis
*
*         Date: june 15 1995
*
*      Purpose: Compute the intersection of two basis ("basis1" and "basis2"). 
*               The input basis must have equal window sizes.
*               
*  Input Rout.: basis1 -> pointer to basis structure, 
*               basis2 -> pointer to basis structure,     
*               basis3 -> pointer to basis structure.    
*
*        Input: "basis1" and "basis2". 
*
*       Output: "basis3", basis of the intersection of the basis of input.
*           
*       Return: 1 on success, 0 on failure.                          
*
*  Called from: vintbas, erosion_basis
*
* Rout. called: free_list_interval, function1, list_interv_to_basis.
*
*   Written By: Guilermo Pablo Salas
*         
****************************************************************/

int intersection_basis(basis1,basis2,basis3)
trv_struc *basis1, *basis2, *basis3;

{ 
   int k,l,i;
   int n4bytes;
   int total, total2;  
   int *A,*B_c;
   itr_struc head, head2; 
   int ind1,ind2;
   int flag;
   itr_struc *itr, *itr2;
   int st;
   int nv;

   n4bytes = basis1->nb;   
   A = (int *)malloc(sizeof(int) *n4bytes);
   if(A == NULL){
         fprintf(stderr, "intersection_basis() : memory allocation failed\n") ;
         return(0) ;
    }
    B_c = (int *)malloc(sizeof(int) *n4bytes);

    if(B_c == NULL){
         fprintf(stderr, "intersection_basis() : memory allocation failed\n") ;
         free(A);
         return(0) ;
    }
              
    total = 0;
    head.A = NULL;
    head.B_c = NULL;
    head.next = NULL;

    head2.A = NULL;
    head2.B_c = NULL;
    head2.next = NULL;
    total2 = 0;

    for(k = 0; k < basis1->total; k++){
         ind1 = basis1->ind[k];
         for(l = 0; l < basis2->total; l++){
              ind2 = basis2->ind[l];

              /* intersection of the intervals */

              for(i = 0; i < n4bytes; i++){
                     A[i] = basis1->A[i + ind1] | basis2->A[i + ind2];    
                     B_c[i] = basis1->B[i + ind1] | basis2->B[i + ind2];    
              }
              flag = 1;

              /* verify if is a interval */

              for(i = 0; i < n4bytes; i++){
                      if((A[i] & B_c[i]) != 0){
                              flag = 0;
                              break;
                      }
              }
                             
             if(flag == 1){

                     itr = (itr_struc *)malloc(sizeof(itr_struc ));
                     if(itr == NULL){
                                   fprintf(stderr, "intersection_basis() : memory allocation failed\n") ;
                                   free(A);
                                   free(B_c);
                                   free_list_interval(&head);                            
                                   return(0) ;
                     }

                     itr->A = A;
                     itr->B_c  = B_c;
                     itr->next = head2.next; 
                     head2.next = itr;
                     total2++;
                     function1(&head2,itr,&total2,n4bytes);  
                     A = (int *)malloc(sizeof(int) *n4bytes);
                     if(A == NULL){
                                   fprintf(stderr, "intersection_basis() : memory allocation failed\n") ;
                                   free_list_interval(&head);
                                   return(0) ;
                     }

                     B_c = (int *)malloc(sizeof(int) *n4bytes); 
                     if(B_c == NULL){
                                   fprintf(stderr, "intersection_basis() : memory allocation failed\n") ;
                                   free(A);
                                   free_list_interval(&head);
                                   return(0) ;
                     }
                                        
               }

          }

          itr2 = head2.next;

          for(i = 0; i < total2; i++){
              itr = itr2;
              itr2 = itr2->next;
              itr->next = head.next; 
              head.next = itr;
              total++;
              function1(&head,itr,&total,n4bytes); 
          }

          total2 = 0;
          head2.A = NULL;
          head2.B_c = NULL;
          head2.next = NULL; 

     }
     free(A);
     free(B_c);
     /* converte list of intervals in structure basis */
       
     st = list_interv_to_basis(&head, basis3, total, basis1->nv);

     free_list_interval(&head);

     if(st ==0){
          fprintf(stderr, "intersection_basis() : memory allocation failed\n");
          return(0) ;
     }

     return(1);

}   
 
