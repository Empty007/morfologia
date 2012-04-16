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
#include "equivalence_class.h"
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"

/****************************************************************
*
* Routine Name: equivalence_class 
*
*         Date: march 3 2011
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

int equivalence_class(basis1, basis2, translators)
trv_struc *basis1, *basis2;
equivalence_struct *equival;
{
    int nroIntervalos = basis1->total;
    int i, j, n4bits, st;
    /*if (nroIntervalos <= 1) {*/
    /*copy_basis(basis1, basis2);*/
    /*translators->trans[0] = 0;*/
    /*translators->tamanho = countinterv->ind[i] = b ; */
    /*return (1);*/
    /*}*/

    /*basis2->A = NULL;*/
    /*basis2->B = NULL;*/
    /*basis2->ind = NULL;*/
    /*basis2->size = 0;*/
    /*basis2->total = 0;*/
    /*basis2->nv = 0;    BASE (nv) (total) */
    /*basis2->nb = 0;*/

    int hsizewin = basis1->win.hsize;
    int wsizewin = basis1->win.wsize;    
    int n4bytesWin = ((hsizewin * wsizewin) + 31) / 32 ;
    int hexa;
    int mapaWin[basis1->win.size][2];
    int tamanhoWin = 0;

    for (i = 0; i < hsizewin * wsizewin; i++) {
        if (i % 32 == 0) hexa = basis1->data[i / 32]; 
        else hexa >>= 1;
        if (hexa & 1 != 0) {
            mapaWin[tamanhoWin][0] = i / wsizewin - wsizewin / 2;
            mapaWin[tamanhoWin][1] = i % wsizewin - wsizewin / 2;
            tamanhoWin++;
        }
    }

    int mapa[nroIntervalos][tamanhoWin][2];
    
    n4bytes = (nv + 31) / 32;
    bool vis[nroIntervalos];
    int tamanhos[nroIntervalos];
    int primeiroBit1[nroIntervalos];
    int b = 0;
    copy_basis(basis1, basis2);
    free(basis2->A);
    free(basis2->B);

    for (i = 0; i < nroIntervalos; i++) {
        primeiroBit1[i] = -1;
        int 
        for (j = 0; j < n4bytes; j++) {
            int nro = basis1->A[b + j];
            int cnt = j * 32;
            while (nro != 0) {
                if (nro & 1 != 0) {
                    if (primeiroBit1[i] == -1) primeiroBit1[i] = cnt;
                    mapa[i][tamanhos[i]][0] = mapaWin[cnt][0];
                    mapa[i][tamanhos[i]][1] = mapaWin[cnt][1];                    
                    tamanhos[i]++;
                }
                nro = nro >> 1;
                cnt++;
            }
        }
        
        vis[i] = false;
        b = b + n4bytes;
    }

    int nroNovosIntervalos = 0;
    int inf = (1 << 30);
    int intervalosCopiar[nroIntervalos];

    for (i = 0; i < nroIntervalos; i++) if (!vis[i]) {
        vis[i] = true;
        intervalosCopiar[nroNovosIntervalos] = i;
        equival[nroNovosIntervalos]->trans[0] = 0;
        equival[nroNovosIntervalos]->tamanho = tamanhos[i];
        int cnt = 1;
        int dify = inf;
        int difx = inf;
        for (j = 0; j < nroIntervalos; j++) if (!vis[j]) {
            if (tamanhos[i] != tamanhos[j]) continue;
            dify = mapa[i][0][0] - mapa[j][0][0];
            difx = mapa[i][0][1] - mapa[j][0][1];
            bool bo = true;
            for (k = 1; k < tamanhos[i]; k++) {
                if (dify != mapa[k][0][0] - mapa[k][0][0] || difx != mapa[i][k][1] - mapa[k][0][1]) {
                    bo = false;
                    break;
                }
            }
            if (bo) {
                vis[j] = true;
                equival[nroNovosIntervalos]->trans[cnt][0] = dify;
                equival[nroNovosIntervalos]->trans[cnt][1] = difx;
                cnt++;
            }
        }
        equival[nroNovosIntervalos]->nroTranslations = cnt;
        nroNovosIntervalos++;
    }

    /*copy basis */

   basis2->nv = basis1->nv;
   st = basis1->nb;
   basis2->nb = st;
   basis2->total = nroNovosIntervalos;

   basis2->A = (int *) malloc( sizeof(int) * basis2->nb * basis2->total);
   if(basis2->A == NULL){
      fprintf(stderr,"copy_basis(): memory allocation failed\n");
      return(0);
   }

   basis2->B = (int *) malloc( sizeof(int) * basis2->nb * basis2->total);
   if(basis2->B == NULL){
      fprintf(stderr,"copy_basis(): memory allocation failed\n");
      free(basis2->A);
      return(0);
   }

   basis2->size = basis2->total * basis2->nb;

   basis2->ind  = (int *) malloc(sizeof(int) * basis2->total);
   if(basis2->ind == NULL){
      fprintf(stderr,"copy_basis(): memory allocation failed\n"); 
      free(basis2->A);
      free(basis2->B);
      return(0);
   }

   /* copy each intervalo */
   for( i = 0; i < basis2->total; i++){
       basis2->ind[i] = n4bytes * i; 
       for(j = 0; j < basis2->nb; j++){
           basis2->A[j + basis2->ind[i]] = basis1->A[j + basis1->ind[intervalosCopiar[i]] ];
           basis2->B[j + basis2->ind[i]] = basis1->B[j + basis1->ind[intervalosCopiar[i]] ];
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

