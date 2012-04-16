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

/*=*/ int aha_W_novo_imprime (aha_struc * sai, 
                              trv_struc * base, mat_str * str) {
  trv_struc elle, erre;
  int i, j, ind1, ind2, ii, jj, ind3, k, tem, kk, tam;
  trv_struc base1, base2, base3, base4;
  int_list_struc *list, *apt_list;

  if (sai == NULL || base == NULL || str == NULL) {
    fprintf (stderr, "aha_W_novo () pointers to NULL\n");
    return 0;
  }

  if (base->size != 1) {
    fprintf (stderr, "aha_W_novo: The basis must represent just ");
    fprintf (stderr, "one interval\n");
    return 0;
  }

  zera_basis_by_ron (&elle);
  zera_basis_by_ron (&erre);
  list = NULL;

  elle_W_novo (&elle, base, str);
  printf ("ELLE GERADO\n");
  disp_basis_by_ron (&elle);
  erre_W_novo (&erre, base, str);
  printf ("ERRE GERADO\n");
  disp_basis_by_ron (&erre);
  printf ("=============================\n");

  if (elle.total * erre.total == 0) {
    zera_aha (sai);
    free_basis_by_ron (&elle);
    free_basis_by_ron (&erre);
    return 1;
  }

  // sai->total = elle.total * erre.total;
  // sai->intervs = (int_struc *) node_allocation
  // (sizeof(int_struc)*sai->total);
  sai->total = 0;
  sai->nv = elle.nv;
  sai->nb = (sai->nv+NB1)/NB;
  copy_win (&elle.win, &sai->win);

  zera_basis_by_ron (&base1);
  zera_basis_by_ron (&base2);
  zera_basis_by_ron (&base3);
  zera_basis_by_ron (&base4);

  base1.total = base1.size = 1; /* somente 1 intervalo */
  copy_win (&elle.win, &base1.win);
  base1.nv = elle.nv;
  base1.nb = elle.nb;
  base1.ind = (int *) node_allocation (sizeof(int) * base1.total);
  base1.A = (int *) node_allocation (sizeof(int) * base1.total * base1.nb);
  base1.B = (int *) node_allocation (sizeof(int) * base1.total * base1.nb);
  base1.ind[0] = 0;

  base2.total = base2.size = 1; /* somente 1 intervalo */
  copy_win (&elle.win, &base2.win);
  base2.nv = erre.nv;
  base2.nb = erre.nb;
  base2.ind = (int *) node_allocation (sizeof(int) * base2.total);
  base2.A = (int *) node_allocation (sizeof(int) * base2.total * base2.nb);
  base2.B = (int *) node_allocation (sizeof(int) * base2.total * base2.nb);
  base2.ind[0] = 0;

  sai->total = 0;
  for (i=0, k=0; i<elle.total; i++) {
    ind1 = elle.ind[i];
    for (j=0; j<erre.total; j++) {
      ind2 = erre.ind[j];

      for (ii=0; ii<elle.nb; ii++) {
        base1.A[ii] = elle.A[ind1+ii];
        base1.B[ii] = elle.B[ind1+ii];
      }

      for (jj=0; jj<erre.nb; jj++) {
        base2.A[jj] = erre.A[ind2+jj];
        base2.B[jj] = erre.B[ind2+jj];
      }

      free_basis_by_ron (&base3);
      union_basis_by_ron (&base1, &base2, &base3);

      tem = 0;
      for (apt_list = list; apt_list && !tem; apt_list=apt_list->prox) {
        base4.total = base4.size = apt_list->parte.total;
        copy_win (&elle.win, &base4.win);
        base4.nv = sai->nv;
        base4.nb = sai->nb;
        base4.ind = (int *) node_allocation (sizeof(int) * base4.total);
        tam = base4.total * base4.nb;
        base4.A = (int *) node_allocation (sizeof(int) * tam);
        base4.B = (int *) node_allocation (sizeof(int) * tam);

        for (jj=0; jj<apt_list->parte.total; jj++) {
          base4.ind[jj] = ind3 = apt_list->parte.ind[jj];
          for (kk=0; kk<base4.nb; kk++) {
            base4.A[ind3+kk] = apt_list->parte.A[ind3+kk];
            base4.B[ind3+kk] = apt_list->parte.B[ind3+kk];
          }
        }

        tem = igual_basis_by_ron (&base3, &base4);
        //if (k==1) {
        //  printf ("k==1, tem == %d\n", tem);
        //  disp_basis_by_ron (&base3);
        //  disp_basis_by_ron (&base4);
	// }
        free_basis_by_ron (&base4);
      }

      if (!tem) {
        apt_list = (int_list_struc *) node_allocation (sizeof(int_list_struc));
        apt_list->parte.total = base3.total;
        tam = base3.total * sai->nb;
        apt_list->parte.A = (int *) node_allocation (sizeof(int) * tam);
        apt_list->parte.B = (int *) node_allocation (sizeof(int) * tam);
        tam = base3.total;
        apt_list->parte.ind = (int *) node_allocation (sizeof(int) * tam);
 
        for (jj=0; jj<base3.total; jj++) {
          ind3 = apt_list->parte.ind[jj] = base3.ind[jj];
          for (kk=0; kk<base3.nb; kk++) {
            apt_list->parte.A[ind3+kk] = base3.A[ind3+kk];
            apt_list->parte.B[ind3+kk] = base3.B[ind3+kk];
          }
        }
        apt_list->prox = list;
        list = apt_list;
        k++;
        sai->total = k;
      }
      free_basis_by_ron (&base3);
    }
  }

  if (sai->total) {
     tam = sai->total;
     sai->intervs = (int_struc *) node_allocation (sizeof(int_struc)*tam);
     i = 0;
     while (list) {
       apt_list = list;
       list = list->prox;
       sai->intervs[i].total = apt_list->parte.total;
       sai->intervs[i].A = apt_list->parte.A;
       sai->intervs[i].B = apt_list->parte.B;
       sai->intervs[i].ind = apt_list->parte.ind;
       i++;
       free (apt_list);
     }
  }

  free_basis_by_ron (&elle);
  free_basis_by_ron (&erre);
  free_basis_by_ron (&base1);
  free_basis_by_ron (&base2);

  return 1;
}

/*=*/ int gera_lower_bounds_novo_imprime (trv_struc_list **s, 
                                          trv_struc * base, 
                                          mat_str * str) {
  trv_struc * quebrada;
  int i, j, * vet_apt, total, tem;
  aha_struc * aha;
  trv_struc * esse, base1;
  trv_struc_list * sai, *apt_list;
  int tam_sai;
  FILE * fd;

  if (s == NULL || base == NULL || str == NULL) {
    fprintf (stderr, "gera_lower_bounds () pointers to NULL\n");
    return 0;
  }

  // disp_win_by_ron (&base->win);

  quebrada = (trv_struc *) node_allocation (sizeof (trv_struc) * 
                                            base->total);

  for (i=0; i<base->total; i++) zera_basis_by_ron (&quebrada[i]);
  quebra_base_intervalos (quebrada, base);

  aha = (aha_struc *) node_allocation (sizeof (aha_struc) * base->total);
  for (i=0; i<base->total; i++) zera_aha (&aha[i]);

  geral = 0;
  for (i=0; i<base->total; i++) {
    aha_W_novo_imprime (&aha[i], &quebrada[i], str);
    printf ("AHA i = %02d => %03d Pairs\n", i, aha[i].total);
    disp_aha_by_ron (&aha[i]);
    free_basis_by_ron (&quebrada[i]);
  }

  if (quebrada) free (quebrada);
  quebrada = NULL;

  /* os conjuntos aha's ja' foram montados */
  /* cada casa de vet_apt[j] guarda um indice de um intervalo de aha[j] */
  vet_apt = (int *) node_allocation (sizeof (int) * base->total);

  for (i=0, total=1; i<base->total; i++) {
    vet_apt[i] = 0;
    if (aha[i].total) total *= aha[i].total;
  }

  esse = (trv_struc *) node_allocation (sizeof (trv_struc) * base->total);
  for (i=0; i<base->total; i++) zera_basis_by_ron (&esse[i]);

  sai = NULL; tam_sai = 0;
  zera_basis_by_ron (&base1);
  if (beatriz) printf ("total de possibilidades = %d\n", total);
  for (i=0; i<total; i++) { /* num. total de prod. cartesiano */
    // printf ("%s\r", " ");
    // printf ("%d\r", i);
    
    free_basis_by_ron (&base1);
    // printf ("Vetor de Indices ");
    // printf ("%02d => ", i+1);
    // for (j=0; j<base->total; j++) printf ("%02d ", vet_apt[j]); printf ("\n");
    gera_um_Z (&base1, vet_apt, aha, base->total, esse);
    // printf ("Lower Bound Gerada:\n");
    // disp_basis_by_ron (&base1);
    if (base1.total) {
      for (apt_list = sai, tem=0; apt_list && !tem; apt_list = apt_list->prox) {
        tem = igual_basis_by_ron (&apt_list->base, &base1);
      }
      if (!tem) {
        apt_list = (trv_struc_list *) node_allocation (sizeof (trv_struc_list));
        copy_basis_by_ron (&base1, &apt_list->base);
        apt_list->prox = sai;
        sai = apt_list;
        tam_sai++;
        // printf ("tam_sai = %d\n", tam_sai);
      }
    }
  }

  *s = sai;

  /* finalizando e matando o que tem que matar */
  i = 0;
  printf ("Temos %d Lower Bounds:\n", tam_sai);
  while (sai) {
    apt_list = sai;
    sai = sai->prox;
    // printf ("Basis %d:\n", ++i);
    // disp_basis_by_ron (&apt_list->base);
  }

  for (i=0; i<base->total; i++) {
    free_aha (&aha[i]);
    free_basis_by_ron (&esse[i]);
  }

  // printf ("Temos %d Lower Bounds\n", tam_sai);

  if (esse) free (esse);
  if (aha) free (aha);
  if (vet_apt) free (vet_apt);
  return 1;
}


int main (int argc, char * argv[]) {
  trv_struc base;
  char * fname1, *fname2;
  int lado;
  mat_str   str, str_rot;
  win_struc win_ero;
  trv_struc_list * lower, * apt_list;

  fname1 = argv[1];
  fname2 = argv[2];

  zera_basis_by_ron (&base);
  read_itrv (fname1, &base);
  read_str (fname2, &str);

  gera_lower_bounds_novo_imprime (&lower, &base, &str);

  lado = 0;
  while (lower) { 
    apt_list = lower;
    lower = lower->prox;
    printf ("Lower Basis %03d:\n", ++lado);
    disp_basis_by_ron (&apt_list->base);
    free_basis_by_ron (&apt_list->base);
    free (apt_list);
  }


  free_basis_by_ron (&base);
  if (str.tag == MM_IMG && str.dat) free (str.dat);
}
