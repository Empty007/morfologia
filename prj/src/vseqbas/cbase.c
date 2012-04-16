# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

extern int mask[NB];
extern int imprime, geral, detalhe, beatriz;

typedef struct {
  int total;
  int *A;
  int *B;
  int *ind;
} int_struc;

typedef struct parte {
  int_struc parte;
  struct parte * prox;
} int_list_struc;

typedef struct {
  int total;
  int nv;
  int nb;
  win_struc win;
  int_struc * intervs;
} aha_struc;

typedef struct base_list {
  trv_struc base;
  struct base_list * prox;
} trv_struc_list;

typedef struct win_list {
  win_struc win;
  struct win_list * prox;
} win_struc_list;

typedef struct saida_lista_search {
  trv_struc_list * list_base;
  win_struc C;
  struct saida_lista_search * prox;
} output_search;


/*=*/ int complexidade_da_extremidade_esquerda (trv_struc * base) {

  int i, j, cont;
  int nshifts, section;
  int *winidx, pos;

  if (base == NULL) {
    fprintf (stderr, "complexidade_da_extremidade_esquerda ");
    fprintf (stderr, " pointers to NULL\n");
    return 0;
  }

  if (base->size != 1) {
    // fprintf (stderr, "conta_pontos_da_extremidade_esquerda: ");
    // fprintf (stderr, "The basis must represent just one interval\n");
    return 0;
  }

  i = (base->win.hsize-1)/2;
  j = (base->win.wsize-1)/2;

  pos = (i * base->win.wsize) + j;

  winidx = NULL;
  win_index (&base->win, &winidx);

  for (i=0, cont=0; i<base->nv; i++) {
    if (winidx[i] != pos) {
      nshifts = i%NB;
      section = i/NB;
      cont += ((base->A[section] & mask[nshifts]) == mask[nshifts]);
    }
  }
  /* cont contem o numero de shifts */
  cont += cont-1;
  /* cont contem o numero de shifts + numero de unioes desta extremidade */
  if (winidx) free (winidx);
  return cont;
}

/*=*/ int cbase (trv_struc * base) {
  trv_struc * quebrada;
  int i, cont;

  if (base == NULL) {
    fprintf (stderr, "cbase () pointers to NULL\n");
    return 0;
  }

  quebrada = (trv_struc *) node_allocation (sizeof (trv_struc) * 
                                            base->total);

  for (i=0; i<base->total; i++) zera_basis_by_ron (&quebrada[i]);
  quebra_base_intervalos (quebrada, base);

  for (i=0, cont=0; i<base->total; i++) {
    cont += complexidade_da_extremidade_esquerda (&quebrada[i]);
    free_basis_by_ron (&quebrada[i]);
  }

  if (quebrada) free (quebrada);
  quebrada = NULL;
  /* adiciona o numero de unioes entre intervalos */
  cont += base->total-1;
  return cont;
}

/*=*/ int main (int argc, char * * argv) {

  trv_struc basis1;
  int cont;;

  zera_basis_by_ron (&basis1);
  read_itrv (argv[1], &basis1);
  cont = cbase (&basis1);
  printf ("Complexidade = %d\n", cont);
  free_basis_by_ron (&basis1); 
  return 0;
}
