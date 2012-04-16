#include <stdio.h>
# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "desc_win.h"
# include "desc_bas.h"
# include "negation_basis.h"
# include "pac_def.h"
# include "pac_xpl.h"

extern int mask[32];

int str2cfg (int *cfg, int nvars, int * winidx, int m, int n, char * str, int strsize) {
	/*  int  *cfg: cfg[w]  where w=number of variables */
	/*  int  nvars: no. de variaveis */
	/*  int  *winidx:  winidx[w] */
	/*  int  m, n: m and n are the height and the width of the 
	 rectangle corresponding to window domain */
	/*  char *str ;  str[strsize] */
	
    int i ;
    int block, nshifts ;
    int n4bytes, compr;
	
    n4bytes = (nvars+31)/32;
	
    // inicializar cfg
    for (i=0; i<n4bytes; i++) cfg[i] = 0;
	
    for(i=0; i<nvars; i++) {
		nshifts = i%NB;
		block = i/NB;
		if (str[2*winidx[i]] == '1') {
			cfg[block] |= mask[nshifts];
		}
    }
    return 1;
}

int monta_basis_before (trv_struc *basis1, trv_struc *basis2) {
	int       st, i, ind;
	int       * winidx;
	char      strA[3000], strB[3000];
	
	st = win_index (&(basis1->win), &winidx);
	if (st == 0) {
		fprintf (stderr, "monta_basis_before (): win_index() failed\n");
		return 0;
	}
	
	st = copy_basis (basis1, basis2);
	if (st==0) {
		fprintf (stderr,"monta_basis_before (): copy_basis() failed\n");
		return 0;
	}
	
	for(i=0; i<basis1->total; i++) {
		ind = basis1->ind[i];
		cfg2str (&(basis1->A[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strA, 2500);
		cfg2str (&(basis1->B[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strB, 2500);
		str2cfg (&(basis2->A[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strB, 2500);
		str2cfg (&(basis2->B[ind]), basis1->nv, winidx, (basis1->win).hsize,
				 (basis1->win).wsize, strA, 2500);
		
	}
	return 1;
}

int chama_negation_basis (trv_struc *basis1, trv_struc *basis2) {
	int st;
	
	st = copy_win (&(basis1->win), &(basis2->win));
	
	if (st == 0) {
		fprintf(stderr, "chama_negation_basis : copy_win() failed\n");
		return 0;
	}
	
	st = negation_basis (basis1, basis2);
	
	if (st == 0){
		fprintf(stderr, "chama_negation_basis : negation_basis() failed\n") ;
		return 0;
	}
	
	return 1;
}

int lvastbas (char * fname1, char * fname2) {
  trv_struc basis1, basis2, basis3;
  int       st;

  reset_basis(&basis1);
  st = read_itrv(fname1, &basis1) ;
  if (st == 0) {
    fprintf (stderr, "lvastbas : read_itrv() failed\n");
    free_itrv (&basis1);
    return 0;
  }

  reset_basis (&basis2);
  st = monta_basis_before (&basis1, &basis2);
  if (st == 0) {
    fprintf(stderr, "lvastbas : monta_basis_before () failed\n");
    free_itrv (&basis1);
    return 0;
  }

  reset_basis (&basis3);
  st = chama_negation_basis (&basis2, &basis3);
  if (st == 0) {
    fprintf(stderr, "lvastbas : chama_negation_basis () failed\n");
    free_itrv (&basis1);
    free_itrv (&basis2);
    free_itrv (&basis3);
    return 0;
  }

  /*
   *  reset_basis (&basis3);
   *  st = chama_negation_basis (&basis2, &basis3);
   *  if (st == 0) {
   *    fprintf(stderr, "lvastbas : chama_negation_basis () failed\n");
   *    free_itrv (&basis1);
   *    free_itrv (&basis2);
   *    free_itrv (&basis3);
   *    return 0;
   *  }
   *
   */

  st = write_itrv (fname2, &basis3);
  free_itrv (&basis1);
  free_itrv (&basis2);
  free_itrv (&basis3);
 
  if (st == 0) {
    fprintf(stderr, "lvastbas: write_itrv() failed\n");
    return 0;
  }

  return 1;
}

int main (int argc, char * * argv) {
    if(!lvastbas(argv[1], argv[2])) {
       fprintf(stderr, "vlast3 : lvastbas() failed\n") ;
       return 1;
    }
    return 0;
}




