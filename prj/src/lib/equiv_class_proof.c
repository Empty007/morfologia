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

static int comp(const void* p1, const void* p2) {
	int* arr1 = (int*)p1;
	int* arr2 = (int*)p2;
	int diff1 = arr1[0] - arr2[0];
	if (diff1) return diff1;
	return arr1[1] - arr2[1];
}

static int comp2(const void* p1, const void* p2) {
	int* arr1 = (int*)p1;
	int* arr2 = (int*)p2;
	int diff1 = arr2[0] - arr1[0];
	if (diff1) return diff1;
	return arr2[1] - arr1[1];
}

int equivalence_class(basis1, basis2, equival)
trv_struc *basis1, *basis2;
equivalence_struct *equival;
{
    int nroIntervalos = basis1->total;
    int i, j, k, t, n4bits, st;
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
    int mapaWin[basis1->win.size][2]; /* y, x das posicoes dos 1's na janela h * w */
    int tamanhoWin = 0; /* nro de 1's */

    //printf("%d %d %d %d\n", hsizewin, wsizewin, n4bytesWin, basis1->win.size); 


    for (i = 0; i < hsizewin * wsizewin; i++) {
        if (i % 32 == 0) hexa = basis1->win.data[i / 32]; 
        else hexa >>= 1;
        if (hexa & 1 != 0) {
            mapaWin[tamanhoWin][0] = i / wsizewin - wsizewin / 2;
            mapaWin[tamanhoWin][1] = i % wsizewin - wsizewin / 2;
            tamanhoWin++;
        }
    }

    int mapa[nroIntervalos][tamanhoWin][2]; /* y, x das posicoes dos 1's nos intervalos */
    
    int n4bytes = (basis1->nv + 31) / 32;
    int vis[nroIntervalos]; /*vetor de visitados de cada intervalo*/
    int tamanhos[nroIntervalos]; /* nro de 1's de cada intervalo*/
    /*int primeiroBit1[nroIntervalos];*/
    int b = 0;

    for (i = 0; i < nroIntervalos; i++) {
        /*primeiroBit1[i] = -1;*/
		tamanhos[i] = 0;
        for (j = 0; j < n4bytes; j++) {
            int nro = basis1->A[b + j];
            int cnt = j * 32;
            while (nro != 0) {
                if (nro & 1 != 0) {
                    /*if (primeiroBit1[i] == -1) primeiroBit1[i] = cnt;*/
                    mapa[i][tamanhos[i]][0] = mapaWin[cnt][0];
                    mapa[i][tamanhos[i]][1] = mapaWin[cnt][1];
                    tamanhos[i]++;
                }
                nro = nro >> 1;
                cnt++;
            }
        }
        vis[i] = 0;
        b = b + n4bytes;
    }

    int nroNovosIntervalos = 0;
    int inf = (1 << 30);
    int intervalosCopiar[nroIntervalos]; /*que intervalos devemos copiar*/
	int translate[nroIntervalos][tamanhoWin][2]; /* vetor de todas as translacoes [nroIntervalo real de base][nro de Translacao][x, y]*/ 
	int nrovars[nroIntervalos]; /*nro de 1's*/
	int nroTranslations[nroIntervalos][2]; /*nro de translacoes de cada elemento da base, indice para ordenar*/

    for (i = 0; i < nroIntervalos; i++) if (!vis[i]) {
        vis[i] = 1;
        intervalosCopiar[nroNovosIntervalos] = i;
		//printf("%d\n", intervalosCopiar[nroNovosIntervalos]);
        translate[nroNovosIntervalos][0][0] = 0;
        translate[nroNovosIntervalos][0][1] = 0;
        nrovars[nroNovosIntervalos] = tamanhos[i];
		//printf("%d %d %d\n", equival->translate[nroNovosIntervalos][0], equival->translate[nroNovosIntervalos][1], equival->nrovars[nroNovosIntervalos]);
        int cnt = 1;
        int dify = inf;
        int difx = inf;
        for (j = 0; j < nroIntervalos; j++) if (!vis[j]) {
            if (tamanhos[i] != tamanhos[j]) continue;
            dify = mapa[j][0][0] - mapa[i][0][0];
            difx = mapa[j][0][1] - mapa[i][0][1];
            int bo = 1;
            for (k = 1; k < tamanhos[i]; k++) {
                if (dify != mapa[j][k][0] - mapa[i][k][0] || difx != mapa[j][k][1] - mapa[i][k][1]) {
                    bo = 0;
                    break;
                }
            }
            if (bo) {
                vis[j] = 1;
                translate[nroNovosIntervalos][cnt][0] = dify;
                translate[nroNovosIntervalos][cnt][1] = difx;
                cnt++;
            }
        }
		qsort(translate[nroNovosIntervalos], cnt, 2 * sizeof(int), comp);
        nroTranslations[nroNovosIntervalos][0] = cnt;
		nroTranslations[nroNovosIntervalos][1] = nroNovosIntervalos; 
        nroNovosIntervalos++;
    }
	
	qsort(nroTranslations, nroNovosIntervalos, 2 * sizeof(int), comp2);
	
	equival->nrovars = (int *) malloc(sizeof(int) * nroNovosIntervalos);
	equival->nroTranslations = (int *) malloc(sizeof(int) * nroNovosIntervalos);
    equival->size = nroNovosIntervalos;
	
	equival->translate  = (int **) malloc(sizeof(int*) * nroNovosIntervalos);
	for (i = 0; i < nroNovosIntervalos; i++) 
		equival->translate[i] = (int *) malloc(sizeof(int) * nroTranslations[i][0] * 2);
	
	for (i = 0; i < nroNovosIntervalos; i++) {
		equival->nrovars[i] = nrovars[nroTranslations[i][1]];
		equival->nroTranslations[i] = nroTranslations[i][0];
		for (j = 0; j < nroTranslations[i][0] * 2; j++) {
			equival->translate[i][j] = translate[nroTranslations[i][1]][j / 2][j % 2];
		}
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
			basis2->A[j + basis2->ind[i]] = basis1->A[j + basis1->ind[intervalosCopiar[nroTranslations[i][1]]] ];
			basis2->B[j + basis2->ind[i]] = basis1->B[j + basis1->ind[intervalosCopiar[nroTranslations[i][1]]] ];
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
	
	
	/* fatorization */
	
	for (i = 0; i < nroNovosIntervalos; i++) {
		if (equival->nroTranslations[i] == 1) continue;
		for (j = i + 1; j < nroNovosIntervalos; j++) {
			if (equival->nroTranslations[i] == equival->nroTranslations[j]) { /* sao iguais o numero de translaçoes */
				dify = equival->translate[i][0] - equival->translate[j][0];
				dify = equival->translate[i][1] - equival->translate[j][1];
				int bo = 1;
				for (k = 1; k < nroTranslations[i][0]; k++) {
					if (equival->translate[i][k * 2] - equival->translate[j][k * 2] != dify || 
						equival->translate[i][k * 2 + 1] - equival->translate[j][k * 2 + 1] != difx) {
						bo = 0;
						break;
					}	
				}
				if (bo) {
					/* move base -t */
					
					/* change translations of base j */
					for (k = 0; k < nroTranslations[i][0] * 2; k++) {
						equival->translate[j][k] = equival->translate[i][k];
					}
				}
			}
			else { /* sao diferentes o numero de translacoes*/
				/* procurar se Cj esta incluido em Ci*/
				/* conseguir diferenças entre intervalos */
				
				if (nroTranslations[j][0] == 1) { /* sempre esta incluido */
					
				}
				else {
					int dif1[nroTranslations[i][0] - 1][2];
					int dif2[nroTranslations[j][0] - 1][2];
					
					for (k = 1; k < nroTranslations[i][0]; k++) {
						dif1[k - 1][0] = equival->translate[i][k * 2] - equival->translate[i][(k - 1) * 2];
						dif1[k - 1][1] = equival->translate[i][k * 2 + 1] - equival->translate[i][(k - 1) * 2 + 1];
						if (k < nroTranslations[j][0]) {
							dif2[k - 1][0] = equival->translate[j][k * 2] - equival->translate[j][(k - 1) * 2];
							dif2[k - 1][1] = equival->translate[j][k * 2 + 1] - equival->translate[j][(k - 1) * 2 + 1];
						}
					}
				}
				
				int bo = 1;
				int indice = -1;
				
				for (k = 0; k <= nroTranslations[i][0] - nroTranslations[j][0]; k++) {
					bo = 1;
					for (t = 0; t < nroTranslations[j][0]; t++) {
						if (dif1[t + k][0] == dif2[t][0] && dif1[t + k][1] == dif2[t][1]) { bo = 0; break; }
					}
					if (bo) { indice = k; break; }
				}
				
				int mtx, mty; /* movements se esta incluido */
				
				if (bo) { /* esta incluido */
					int mty = equival->translate[i][indice * 2] - equival->translate[j][0];
					int mtx = equival->translate[i][indice * 2 + 1] - equival->translate[j][1];
				}
				
			}
		}
	}
	
   return(1);
}

int write_equivalence(fname, equiclass)
 char     *fname ;
 equivalence_struct *equiclass;
{
	
	FILE  *fd ;
	
	fd = fopen(fname, "w") ;
	if(fd == NULL) {
        fprintf(stderr, "write_equivalence : fopen failed\n") ;
        return(0) ;
	}

    int i, j;

    fprintf(fd, "%d\n", equiclass->size);
	printf("%d\n", equiclass->size);

    for (i = 0; i < equiclass->size; i++) {
        fprintf(fd, "%d %d ", equiclass->nrovars[i], equiclass->nroTranslations[i]);
		printf("%d %d ", equiclass->nrovars[i], equiclass->nroTranslations[i]);
        for (j = 0; j < equiclass->nroTranslations[i]; j++) {
            fprintf(fd, "%d %d ", equiclass->translate[i][j * 2], equiclass->translate[i][j * 2 + 1]);
			printf("%d %d ", equiclass->translate[i][j * 2], equiclass->translate[i][j * 2 + 1]);
        }
        fprintf(fd, "\n");
		printf("\n");
    }
	return(1) ;
}




/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: dilate_basis.c
 >>>>
 >>>> Date Last Updated: june 13 2011
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
 *   Written By: Joel Edu Sánchez Castro
 *        
 ****************************************************************/

static int comp(const void* p1, const void* p2) {
	int* arr1 = (int*)p1;
	int* arr2 = (int*)p2;
	int diff1 = arr1[0] - arr2[0];
	if (diff1) return diff1;
	return arr1[1] - arr2[1];
}

static int comp2(const void* p1, const void* p2) {
	int* arr1 = (int*)p1;
	int* arr2 = (int*)p2;
	int diff1 = arr2[0] - arr1[0];
	if (diff1) return diff1;
	return arr2[1] - arr1[1];
}

int equivalence_class(basis1, basis2, equival)
trv_struc *basis1, *basis2;
equivalence_struct *equival;
{
    int nroIntervalos = basis1->total;
    int i, j, k, t, p, q, n4bits, st;
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
    int mapaWin[basis1->win.size][2]; /* y, x das posicoes dos 1's na janela h * w */
    int tamanhoWin = 0; /* nro de 1's */
	
    //printf("%d %d %d %d\n", hsizewin, wsizewin, n4bytesWin, basis1->win.size); 
	
	/* conseguir y, x das posicoes dos 1's na janela h * w */
    for (i = 0; i < hsizewin * wsizewin; i++) {
        if (i % 32 == 0) hexa = basis1->win.data[i / 32]; 
        else hexa >>= 1;
        if (hexa & 1 != 0) {
            mapaWin[tamanhoWin][0] = i / wsizewin - wsizewin / 2;
            mapaWin[tamanhoWin][1] = i % wsizewin - wsizewin / 2;
            tamanhoWin++;
        }
    }
	
    int mapa[nroIntervalos][tamanhoWin][2]; /* y, x das posicoes dos 1's nos intervalos */
    
    int n4bytes = (basis1->nv + 31) / 32;
    int vis[nroIntervalos]; /*vetor de visitados de cada intervalo*/
    int tamanhos[nroIntervalos]; /* nro de 1's de cada intervalo*/
    /*int primeiroBit1[nroIntervalos];*/
    int b = 0;
	
    for (i = 0; i < nroIntervalos; i++) {
        /*primeiroBit1[i] = -1;*/
		tamanhos[i] = 0;
        for (j = 0; j < n4bytes; j++) {
            int nro = basis1->A[b + j];
            int cnt = j * 32;
            while (nro != 0) {
                if (nro & 1 != 0) {
                    /*if (primeiroBit1[i] == -1) primeiroBit1[i] = cnt;*/
                    mapa[i][tamanhos[i]][0] = mapaWin[cnt][0];
                    mapa[i][tamanhos[i]][1] = mapaWin[cnt][1];
                    tamanhos[i]++;
                }
                nro = nro >> 1;
                cnt++;
            }
        }
        vis[i] = 0;
        b = b + n4bytes;
    }
	
    int nroNovosIntervalos = 0;
    int inf = (1 << 30);
    int intervalosCopiar[nroIntervalos]; /*que intervalos devemos copiar*/
	int translate[nroIntervalos][tamanhoWin][2]; /* vetor de todas as translacoes [nroIntervalo real de base][nro de Translacao][x, y]*/ 
	int nrovars[nroIntervalos]; /*nro de 1's*/
	int nroTranslations[nroIntervalos][2]; /*nro de translacoes de cada elemento da base, indice para ordenar*/
	
    for (i = 0; i < nroIntervalos; i++) if (!vis[i]) {
        vis[i] = 1;
        intervalosCopiar[nroNovosIntervalos] = i;
		//printf("%d\n", intervalosCopiar[nroNovosIntervalos]);
        translate[nroNovosIntervalos][0][0] = 0;
        translate[nroNovosIntervalos][0][1] = 0;
        nrovars[nroNovosIntervalos] = tamanhos[i];
		//printf("%d %d %d\n", equival->translate[nroNovosIntervalos][0], equival->translate[nroNovosIntervalos][1], equival->nrovars[nroNovosIntervalos]);
        int cnt = 1;
        int dify = inf;
        int difx = inf;
        for (j = 0; j < nroIntervalos; j++) if (!vis[j]) {
            if (tamanhos[i] != tamanhos[j]) continue;
            dify = mapa[j][0][0] - mapa[i][0][0];
            difx = mapa[j][0][1] - mapa[i][0][1];
            int bo = 1;
            for (k = 1; k < tamanhos[i]; k++) {
                if (dify != mapa[j][k][0] - mapa[i][k][0] || difx != mapa[j][k][1] - mapa[i][k][1]) {
                    bo = 0;
                    break;
                }
            }
            if (bo) {
                vis[j] = 1;
                translate[nroNovosIntervalos][cnt][0] = dify;
                translate[nroNovosIntervalos][cnt][1] = difx;
                cnt++;
            }
        }
		qsort(translate[nroNovosIntervalos], cnt, 2 * sizeof(int), comp);
        nroTranslations[nroNovosIntervalos][0] = cnt;
		nroTranslations[nroNovosIntervalos][1] = nroNovosIntervalos; 
        nroNovosIntervalos++;
    }
	
	qsort(nroTranslations, nroNovosIntervalos, 2 * sizeof(int), comp2);
	
	equival->nrovars = (int *) malloc(sizeof(int) * nroNovosIntervalos);
	equival->nroTranslations = (int *) malloc(sizeof(int) * nroNovosIntervalos);
    equival->size = nroNovosIntervalos;
	
	equival->translate  = (int **) malloc(sizeof(int*) * nroNovosIntervalos);
	for (i = 0; i < nroNovosIntervalos; i++) 
		equival->translate[i] = (int *) malloc(sizeof(int) * nroTranslations[i][0] * 2);
	
	for (i = 0; i < nroNovosIntervalos; i++) {
		equival->nrovars[i] = nrovars[nroTranslations[i][1]];
		equival->nroTranslations[i] = nroTranslations[i][0];
		for (j = 0; j < nroTranslations[i][0] * 2; j++) {
			equival->translate[i][j] = translate[nroTranslations[i][1]][j / 2][j % 2];
		}
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
			basis2->A[j + basis2->ind[i]] = basis1->A[j + basis1->ind[intervalosCopiar[nroTranslations[i][1]]] ];
			basis2->B[j + basis2->ind[i]] = basis1->B[j + basis1->ind[intervalosCopiar[nroTranslations[i][1]]] ];
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
	
	/* fatorization */
	
	for (i = 0; i < nroNovosIntervalos; i++) {
		if (equival->nroTranslations[i] == 1) continue;
		for (j = i + 1; j < nroNovosIntervalos; j++) {
			if (equival->nroTranslations[i] == equival->nroTranslations[j]) { /* sao iguais o numero de translaçoes */
				dify = equival->translate[i][0] - equival->translate[j][0];
				dify = equival->translate[i][1] - equival->translate[j][1];
				int bo = 1;
				for (k = 1; k < nroTranslations[i][0]; k++) {
					if (equival->translate[i][k * 2] - equival->translate[j][k * 2] != dify || 
						equival->translate[i][k * 2 + 1] - equival->translate[j][k * 2 + 1] != difx) {
						bo = 0;
						break;
					}	
				}
				if (bo) {
					/* move base -t */
					
					/* change translations of base j */
					for (k = 0; k < nroTranslations[i][0] * 2; k++) {
						equival->translate[j][k] = equival->translate[i][k];
					}
				}
			}
			else { /* sao diferentes o numero de translacoes*/
				/* procurar se Cj esta incluido em Ci*/
				/* conseguir diferenças entre intervalos */
				
				if (nroTranslations[j][0] == 1) { /* sempre esta incluido */
					break;
				}
				else {
					/* O(trans[n]*trans[m]*transladar) encontrar se existem translacoes, que sendo transladadas esta contida */
					/* O(trans[n]*trans[m]*(trans[n]-trans[m])*O(n)*W procurar se os elementos restantes estao contidos na base c*/
					/* O(trans[n]*trans[m]*trans[m] transladar o intervalo j com a translacao encontrada e verificar se esta contida na janela (pode que 
					 a janela seja modificada, entao temos que modificar todos os intervalos*/
					
					int dif1[nroTranslations[i][0] - 1][2]; /* conseguir diferenças entre intervalos */
					int dif2[nroTranslations[j][0] - 1][2];
					
					for (k = 1; k < nroTranslations[i][0]; k++) {
						dif1[k - 1][0] = equival->translate[i][k * 2] - equival->translate[i][(k - 1) * 2];
						dif1[k - 1][1] = equival->translate[i][k * 2 + 1] - equival->translate[i][(k - 1) * 2 + 1];
						if (k < nroTranslations[j][0]) {
							dif2[k - 1][0] = equival->translate[j][k * 2] - equival->translate[j][(k - 1) * 2];
							dif2[k - 1][1] = equival->translate[j][k * 2 + 1] - equival->translate[j][(k - 1) * 2 + 1];
						}
					}
					
					int bo = 1;
					int indice = -1;
					
					/* procurar para que intervalos não se conseguiram translacoes */
					
					for (k = 0; k <= nroTranslations[i][0] - nroTranslations[j][0]; k++) {
						
						bo = 1;
						
						for (t = 0; t < nroTranslations[j][0] - 1; t++) {
							if (dif1[t + k][0] == dif2[t][0] && dif1[t + k][1] == dif2[t][1]) { bo = 0; break; }
						}
						if (bo) {  /* esta incluido */
							int flag = 1; /* true */
							/* verificar se as translacoes que sobram, podem gerar outros intervalos que estão contidas por outro na base */
							/* conseguir a translação */
							/* int mtx, mty;  movements se esta incluido */
							int mty = equival->translate[j][0] - equival->translate[i][k * 2];
							int mtx = equival->translate[j][1] - equival->translate[i][k * 2 + 1];
							for (t = 0; t < nroTranslations[i][0]; t++) {
								if (t == k) t += nroTranslations[j][0];
								/* search interval in base */
								int translacaoy = equival->translate[i][t * 2] + mty;
								int translacaox = equival->translate[i][t * 2 + 1] + mtx;
								/* transladar o intervalo (no novo intervalo), mas já temos esse intervalo em mapa*/
								/* novoIndice (indice antigo em antigos intervalos */
								int novoIndice = intervalosCopiar[j];
								/*transladar de mapas*/
								/*tamanho de 1's em intervalo j tamanhos[novoIndice]*/
								int tamanhoNovo = tamanhos[novoIndice];
								int novoMapa[tamanhoNovo];
								for (p = 0; p < tamanhos[novoIndice]; p++) {
									novoMapa[p][0] = mapa[novoIndice][p][0] + translacaoy;
									novoMapa[p][1] = mapa[novoIndice][p][1] + translacaox;
									/* conseguir y, x das posicoes dos 1's na janela h * w */
									if (novoMapa[p][0] < 0 || novoMapa[p][0] > hsizewin || novoMapa[p][1] < 0 || novoMapa[p][1] > wsizewin) {
										flag = 0;
										break;
									}
								}
								/*ver se o novo mapa esta na base*/
								if (flag) {
									/*ver em cada intervalo se pode ser contida*/
									for (p = 0; p < nroIntervalos; p++) {
										if (tamanhos[p] > tamanhoNovo) { continue; }
										ind = 0;
										flag = 1;
										for (q = 0; q < tamanhos[p]; q++) {
											while (ind < tamanhoNovo && novoMapa[ind][0] == mapa[p][q][0] && novoMapa[ind][1] == mapa[p][q][1]) ind++;
											if (ind == tamanhoNovo) { flag = 0; break; } 
											ind++;
										}
										if (!flag) continue;
										else break; /* encontro intervalo */
									}
								}
								if (!flag) break;
							}
							if (!flag) bo = 0;
						}
						if (bo)
							break;
					}
					
					if (bo) {
						/* move base -t */
						
						/* change translations of base j */
						for (k = 0; k < nroTranslations[j][0] * 2; k++) {
							equival->translate[j][k] = equival->translate[i][k];
						}
					}
				}	
			}
		}
	}
	
	return(1);
}

int write_equivalence(fname, equiclass)
char     *fname ;
equivalence_struct *equiclass;
{
	/* write equivalence struct (numero de bases) (numero de 1s) (numero de translacoes) (translacoes y x) */
	FILE  *fd ;
	
	fd = fopen(fname, "w") ;
	if(fd == NULL) {
        fprintf(stderr, "write_equivalence : fopen failed\n") ;
        return(0) ;
	}
	
    int i, j;
	
    fprintf(fd, "%d\n", equiclass->size);
	printf("%d\n", equiclass->size);
	
    for (i = 0; i < equiclass->size; i++) {
        fprintf(fd, "%d %d ", equiclass->nrovars[i], equiclass->nroTranslations[i]);
		printf("%d %d ", equiclass->nrovars[i], equiclass->nroTranslations[i]);
        for (j = 0; j < equiclass->nroTranslations[i]; j++) {
            fprintf(fd, "%d %d ", equiclass->translate[i][j * 2], equiclass->translate[i][j * 2 + 1]);
			printf("%d %d ", equiclass->translate[i][j * 2], equiclass->translate[i][j * 2 + 1]);
        }
        fprintf(fd, "\n");
		printf("\n");
    }
	return(1) ;
}