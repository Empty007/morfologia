# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

extern int mask[NB];
extern int detalhe, beatriz;

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

/*=*/ int main (int argc, char * * argv) {
  trv_struc basis1, base_sai;
  mat_str   str;
  trv_struc_list * lower;
  output_search * sai, * apt_sai;
  trv_struc_list * apt_trv, * aux_trv;
  char * fname1, * fname2, * fname3;
  trv_struc_list * solucao;

  fname1 = argv[1];
  fname2 = argv[2];
  fname3 = argv[3];
  beatriz = 1;

  zera_basis_by_ron (&basis1);
  read_itrv (fname1, &basis1);
  read_str (fname2, &str);
  
  detalhe = 0;

  gera_lower_bounds_crescente (&lower, &basis1, &str);
  testa_equacao_dilatacao_crescente (&basis1, &str, lower, &solucao);

  free_basis_by_ron (&basis1); 
  if (str.tag == MM_IMG && str.dat) free (str.dat);

  return 1;
}

