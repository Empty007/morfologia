/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: negation_basis.c
 >>>>
 >>>> Date Last Updated: june 17 1995 
 >>>>
 >>>>          Routines: negation_basis .
 >>>>                   
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#include <stdio.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "negation_basis.h"

static int  mask[32] =
     { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
       0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000, 
       0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
       0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 
       0x20000000, 0x40000000, 0x80000000  } ;

int function2();

/****************************************************************
*
* Routine Name: negation_basis
*
*         Date: june 15 1995
*
*      Purpose: Compute the basis of the composition of a operator
*               (with basis basis1) with the operador negation". 
*               this routine is an implementation (see Thesis de 
*               G.P.S and ...)
*
*
*  Input Rout.: basis1 -> pointer to basis structure, 
*               basis2 -> pointer to basis structure.    
*
*        Input: "basis1", basis of the operator,
*
*       Output: "basis2", basis of the infimo. 
*               
*       Return: 1 on success, 0 on failure.                          
*
*  Called from: union_basis, dilation_basis.
*
* Rout. called: alloc_interval, free_list_interval, funtion3, free_interval, 
*               list_interv_to_basis.
*
*
*   Written By: Guilermo Pablo Salas
*        
****************************************************************/

int negation_basis(basis1,basis2)
trv_struc *basis1, *basis2;
{
   int st;
   int n4bytes;
   itr_struc *itr_1, *an_itr_1, *an_itr_2, *itr_2,*itr_3, *itr_4 ,head1, head2 ,head3, head4;
   int total, ind;
   int k_1, k_2;
   int i,j,k,l;
   int counter;
   int *ref;
   int flag, *A, *B_c; 
  
   n4bytes = basis1->nb;
   head1.A = NULL;
   head1.B_c = NULL;
   
   itr_1 = (itr_struc *)malloc(sizeof(itr_struc ));
   if(itr_1 == NULL){
       fprintf(stderr, "negation_basis() : Memoria allocation failed\n") ;
       return(0);                   
   }
    
   n4bytes = basis1->nb;
 
   st = alloc_interval(itr_1,n4bytes);

   if(st == 0){
         fprintf(stderr, "negation_basis() : alloc_interval() failed\n") ;
         free(itr_1);
         return(0) ;
   }
   
   for(i = 0; i < n4bytes; i++){
         itr_1->A[i]   = 0;
         itr_1->B_c[i] = 0;
   }

   head1.next = itr_1;
   itr_1->next = NULL;

   head2.A = NULL;
   head2.B_c = NULL;
   head2.next = NULL;

   total = 1;

   ref = (int *)malloc(sizeof(int) * n4bytes);

   if(ref == NULL){
         fprintf(stderr, "negation_basis() : alloc_interval() failed\n") ;
         free(itr_1);
         return(0) ;
   }
   
   A   = (int *)malloc(sizeof(int) * n4bytes);
   if(A == NULL){
         fprintf(stderr, "negation_basis() : alloc_interval() failed\n") ;
         free(itr_1);
         free(ref);
         return(0) ;
   }
   B_c = (int *)malloc(sizeof(int) * n4bytes);
   if(B_c == NULL){
         fprintf(stderr, "negation_basis() : alloc_interval() failed\n") ;
         free(itr_1);
         free(A);
         free(ref);
         return(0) ;
   }

   head3.A = NULL;
   head3.B_c = NULL;
   head3.next = NULL;
   for(k = 0; k < basis1->total; k++){ /* #1 */
        counter = 0;
        an_itr_1 = &head1;
        ind = basis1->ind[k];
        for(l = 0; l < total; l++){ /* #2 */
              itr_1 = an_itr_1->next;
              flag = 1;
              for(i = 0; i < n4bytes; i++){
                   A[i]   = basis1->A[ind + i] | itr_1->A[i];
                   B_c[i] = basis1->B[ind + i] | itr_1->B_c[i];
                   if(( A[i] & B_c[i]) != 0){
                                flag = 0;
                                break;
                   }
              }
              if( flag == 1){   /* #3 */
                  for(i = 0; i < n4bytes; i++)
                     ref[i] =   itr_1->A[i] | itr_1->B_c[i];
                  for(j = 0; j < basis1->nv; j++){ /* #4 */
                     k_1 = j % 32;
                     k_2 = j/32 ;
                     if((ref[k_2] & mask[k_1]) == 0){  /* #5 */
                        if((A[k_2] & mask[k_1]) != 0){ /* #6 */
                            itr_2 = (itr_struc *)malloc(sizeof(itr_struc ));
                            if(itr_2 == NULL){
                                   fprintf(stderr, "negation_basis() :  Memoria allocation failed\n") ;
                                   free_list_interval(&head1);
                                   free_list_interval(&head2);
                                   free_list_interval(&head3);
                                   free(A);
                                   free(B_c);
                                   free(ref);
                                   return(0) ;
                            }
                            st = alloc_interval(itr_2,n4bytes);
                            if(st == 0){
                                   fprintf(stderr, "negation_basis() :  alloc_interval() failed\n") ; 
                                   free_list_interval(&head1);
                                   free_list_interval(&head2);
                                   free_list_interval(&head3);
                                   free(itr_2);
                                   free(A);
                                   free(B_c);
                                   free(ref);
                                   return(0) ;
                            }
                            for(i = 0; i < n4bytes; i++){
                                 itr_2->A[i]   = itr_1->A[i];
                                 itr_2->B_c[i] = itr_1->B_c[i];
                            }
                            itr_2->B_c[k_2]|=mask[k_1];
                            itr_2->next = head2.next;
                            head2.next = itr_2;
                        } /* #6 */
                        else{ /* 7 */
                            if((B_c[k_2] & mask[k_1]) != 0){ /* #8 */
                                itr_2 = (itr_struc *)malloc(sizeof(itr_struc ));
                                if(itr_2 == NULL){
                                    fprintf(stderr, "negation_basis() :  Memoria allocation failed\n") ;
                                    free_list_interval(&head1);
                                    free_list_interval(&head2);
                                    free_list_interval(&head3);
                                    free(A);
                                    free(B_c);
                                    free(ref);

                                    return(0) ;
                               }
                               st = alloc_interval(itr_2,n4bytes);
                               if(st == 0){
                                    fprintf(stderr, "negation_basis() :  alloc_interval() failed\n") ; 
                                    free_list_interval(&head1);
                                    free_list_interval(&head2);
                                    free_list_interval(&head2);
                                    free(itr_2);
                                    free(A);
                                    free(B_c);
                                    free(ref);
 
                                    return(0) ;
                               }
                               for(i = 0; i < n4bytes; i++){
                                    itr_2->A[i]   = itr_1->A[i];
                                    itr_2->B_c[i] = itr_1->B_c[i];
                               }
  
                               itr_2->A[k_2]|=mask[k_1];
                               itr_2->next = head2.next;
                               head2.next = itr_2;
                             }  /* #8 */
                        } /*  #7 */

                    }/* #5 */        
                } /* #4 */
                an_itr_1 = itr_1;   
             }  /* #3 */
             else{ 
                  an_itr_1->next = itr_1->next;
                  itr_1->next = head3.next;
                  head3.next = itr_1; 
                  counter++;
             }              
        } /* #2 */ 
        itr_2 = head2.next;
        an_itr_2 = &head2;
        while(itr_2 != NULL){
                 if(function2(itr_2,&head3,n4bytes)){
                    an_itr_2->next = itr_2->next;
                    free_interval(itr_2);
                    free(itr_2);
                 }
                 else
                   an_itr_2 = itr_2;
                 itr_2 = an_itr_2->next;
        }
        itr_2 = &head2;
        while(itr_2->next != NULL){
              itr_2 = itr_2->next;
              counter++;         
        }

        if(itr_2 != &head2){
              itr_2->next = head3.next;
              head3.next = head2.next;
        }       

        free_list_interval(&head1);

        head2.next = NULL;
        head1.next = head3.next;
        head3.next = NULL;
        total = counter;
   } /* #1 */ 

   st = list_interv_to_basis(&head1, basis2, total, basis1->nv);
   free_list_interval(&head1);  
   free(A);
   free(B_c);
   free(ref);

   if(st == 0){
       fprintf(stderr, "negation_basis() : list_interv_to_basis() failed\n") ;
       return(0) ;
   }
   return(1);
}

/****************************************************************
*
* Routine Name: function2
*
*         Date: june 15 1995
*
*      Purpose: return "1" if the interval "itr" is include in some interval
*               of a list. return "0" case contrary.
*
*       itr
*      ______ 
*     |      | 
*     |______|
*
*       head             
*      ______      ______      ______      ______
*     |      |    |      |    |      |    |      | 
*     |______|--->|______|--->|______|--->|______|---> ....
*
*                |---> compare
*
*  Input Rout.: "itr"   -> pointer interval,
*               "head"   -> pointer interval,
*               "n4bytes", -> integer.
*
*        Input: "itr",     interval to verify,
*               "head",    head of the list,
*               "n4bytes", number of integers for to represent the 
*                          extremities of the intervals.
*
*
*  Called from: negation_basis .
*
* Rout. called:contido, free_interval.
*
*   Written By: Guillermo Pablo Salas
*
****************************************************************/


int function2(itr,head,n4bytes)
itr_struc *itr,*head;
int n4bytes;
{
  itr_struc *P;   
  
  P = head->next;
  while(P != NULL){
       if(contido(itr->A,itr->B_c,n4bytes,P->A,P->B_c)) 
          return(1);
       
       P = P->next;
  }
  return(0);
}
