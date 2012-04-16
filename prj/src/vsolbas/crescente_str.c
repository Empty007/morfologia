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

/* Nota: base deve conter somente um unico intervalo */
/* Nota: base esta' definida em W' */
/* Nota: sai deve estar definida em W=\erosion(W')_{str} */
/* gera somente as translacoes do intervalo por pontos de str */
/*=*/ int base_ele_erre_W_imp (trv_struc * sai, trv_struc * base, mat_str * str) {
  /* trv_struc * base represents only one interval */
  win_struc * win = &(base->win);
  mat_str str_rot, * ker;
  win_struc * win_ero, win_in;
  int i, * X_ker, * Y_ker, size, m, n, points;
  trv_struc base_aux;
  int retorno;

  if (sai == NULL || base == NULL || str == NULL) {
    fprintf (stderr, "base_ele_erre_W () pointers to NULL\n");
    return 0;
  }

  if (base->size != 1) {
    fprintf (stderr, "The basis must represent just one interval\n");
    return 0;
  }


  /* transpondo o elemento struturante */
  str_rot.dat = NULL;
  lvrotstr (str, 4, 0, &str_rot);

  copy_win (&(base->win), &win_in);

  win_ero = &(sai->win);
  win_ero->size = win_ero->hsize = win_ero->wsize = 0;
  if (sai->win.data) free (sai->win.data);
  sai->win.data = NULL;

  /* construindo a janela $W=W'\ominus C^t$ */
  erode_win (&win_in, &(sai->win), &str_rot);
  win_ero = &(sai->win);

  sai->total = sai->size = 0;
  sai->nv = win_ero->size;
  sai->nb = (sai->nv+NB1)/NB;
  sai->A = sai->B = sai->ind = NULL;

  ker = str;
  size = ker->m * ker->n;
  m = ker->m; n = ker->n;

  X_ker = (int *) node_allocation (sizeof (int) * size);
  Y_ker = (int *) node_allocation (sizeof (int) * size);

  pontos_elemento_estruturante (X_ker, Y_ker, ker, &points);

  zera_basis_by_ron (&base_aux);

  // printf("Input Window\n");
  // disp_win_by_ron (&win_in);
  // printf("Eroded Window\n");
  // disp_win_by_ron (win_ero);
  // printf("\n");

  for (i=0; i<points; i++) {
    // printf ("Base a ser transladada\n");
    // disp_basis_by_ron (base);
    translada_basis (&base_aux, base, X_ker[i], Y_ker[i]);
    printf ("Translacao %d por (%d,%d)\n", i+1, X_ker[i], Y_ker[i]);
    disp_basis_by_ron (&base_aux);
    intersection_B_win (&base_aux, win_ero);
    if (base_aux.total == 1) {
      printf ("Deu Interseccao\n");
      retorno = verifica_se_a_base_ja_tem (sai, &base_aux);
      if (!retorno) {
        coloca_na_base (sai, &base_aux);
      }
    }
    else printf ("Nao deu Interseccao\n");
    free_basis_by_ron (&base_aux);
  }

  if (sai->total == 0) zera_basis_by_ron (sai);
  if (X_ker) free (X_ker); 
  if (Y_ker) free (Y_ker);
  if (win_in.data) free (win_in.data);
  if (str_rot.tag == MM_IMG && str_rot.dat) free (str_rot.dat);
  return 1;
}

/*=*/ int aha_crescente_W_imp (aha_struc * sai, trv_struc * base, mat_str * str) {
  trv_struc elle;
  int i, ii, tam, ind1;

  if (sai == NULL || base == NULL || str == NULL) {
    fprintf (stderr, "aha_W () pointers to NULL\n");
    return 0;
  }

  if (base->size != 1) {
    fprintf (stderr, "aha(): The basis must represent just one interval\n");
    return 0;
  }

  zera_basis_by_ron (&elle);

  base_ele_erre_W_imp (&elle, base, str);
  printf ("elle:\n");
  disp_basis_by_ron (&elle);

  if (elle.total==0) {
    zera_aha (sai);
    free_basis_by_ron (&elle);
    return 1;
  }

  tam = sai->total = elle.total;
  sai->nv = elle.nv;
  sai->nb = (sai->nv+NB1)/NB;
  copy_win (&elle.win, &sai->win);
  sai->intervs = (int_struc *) node_allocation (sizeof(int_struc)*tam);

  for (i=0; i<elle.total; i++) {
    ind1 = elle.ind[i];
    sai->intervs[i].total = 1;
    sai->intervs[i].A = (int *) node_allocation (sizeof(int)*sai->nb);
    sai->intervs[i].B = (int *) node_allocation (sizeof(int)*sai->nb);
    sai->intervs[i].ind = (int *) node_allocation (sizeof(int));
    sai->intervs[i].ind[0] = 0;
    for (ii=0; ii<elle.nb; ii++) {
      sai->intervs[i].A[ii] = elle.A[ind1+ii];
      sai->intervs[i].B[ii] = elle.B[ind1+ii];
    }
  }

  free_basis_by_ron (&elle);

  return 1;
}

/*=*/ int gera_lower_bounds_crescente_imp (trv_struc_list **s, 
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

  quebrada = (trv_struc *) node_allocation (sizeof (trv_struc) * 
                                            base->total);

  for (i=0; i<base->total; i++) zera_basis_by_ron (&quebrada[i]);
  quebra_base_intervalos (quebrada, base);

  aha = (aha_struc *) node_allocation (sizeof (aha_struc) * base->total);
  for (i=0; i<base->total; i++) zera_aha (&aha[i]);

  geral = 0;
  for (i=0; i<base->total; i++) {
    aha_crescente_W_imp (&aha[i], &quebrada[i], str);
    printf ("i = %02d => %03d Pairs\n", i, aha[i].total);
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
  printf ("total de possibilidades = %d\n", total);
  for (i=0; i<total; i++) { /* num. total de prod. cartesiano */
    if (beatriz) {
      printf ("%s\r", " ");
      printf ("%d\r", i);
    }
    
    free_basis_by_ron (&base1);
    // printf ("Vetor de Indices ");
    // printf ("%02d => ", i+1);
    // for (j=0; j<base->total; j++) printf ("%02d ", vet_apt[j]); printf ("\n");
    gera_um_Z (&base1, vet_apt, aha, base->total, esse);
    // printf ("Base Gerada:\n");
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

  fd = fopen ("Z.txt", "w");
  fprintf (fd, "%d\n", tam_sai);
  fclose (fd);

  *s = sai;

  /* finalizando e matando o que tem que matar */
  i = 0;
  printf ("Temos %d Lower Bounds:\n", tam_sai);
  while (sai) {
    apt_list = sai;
    sai = sai->prox;
    printf ("Lower Bound %d:\n", ++i);
    disp_basis_by_ron (&apt_list->base);
    append_basis_by_ron ("Z.txt", &apt_list->base);
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

/*=*/ int testa_equacao_dilatacao_crescente_imp (trv_struc * base, mat_str * str, 
                                             trv_struc_list * Z,
					     trv_struc_list * * sai) {
  trv_struc_list * apt_sol, * sol;
  trv_struc_list * apt_Z;
  trv_struc upper, diferenca, desmonta, base_X, base_Y;
  win_struc W_duas_linhas;
  int * idx, i, j, k, kk, ind, * comb, num_comb, tam_idx, tem, cont_sol;
  int rr, ind1, ind2;
  int igual, retorno;
  trv_struc base_D, base_E, base_Z;
  int lado, cont_Z;
  FILE * fd;

  if (base == NULL || str == NULL) {
    fprintf (stderr, "testa_equacao_dilatacao_crescente () pointers to NULL\n");
    return 0;
  }

  /* calcula Z */
  // le_os_Z (fname, &Z);

  sol = NULL; *sai = NULL;
  if (Z) {
    zera_basis_by_ron (&base_X);
    zera_basis_by_ron (&base_Y);
    zera_basis_by_ron (&base_D);
    zera_basis_by_ron (&base_E);
    zera_basis_by_ron (&desmonta);
    zera_basis_by_ron (&base_Z);
    cont_sol = 0;

    lado = menor_lado_possivel_da_janela (&(base->win));
    transform_to_small_window (&(base->win), lado);

    /* calcula o upper bound */
    zera_basis_by_ron (&upper);
    upper_bound_by_ron (&upper, base, str);
    lado = menor_lado_possivel_da_janela (&(upper.win));
    transform_to_small_window (&(upper.win), lado);
    printf ("Upper bound\n");
    disp_basis_by_ron (&upper);

    /* transfere upper para a janela W */
    // transfer_basis_according_window_menor (&upper, &(Z->base.win));

    /* calcula a janela W'' */
    zera_win_by_ron (&W_duas_linhas);
    calcula_W_duas_linhas (&W_duas_linhas, &(Z->base.win), str);
    lado = menor_lado_possivel_da_janela (&W_duas_linhas);
    transform_to_small_window (&W_duas_linhas, lado);

    /* transfere upper para a janela W'' */
    // transfer_basis_according_window (&upper, &W_duas_linhas);

    /* Diminui o upper de acordo com a janela W */
    diminui_desmonta (&upper, &Z->base.win);
    printf ("Upper bound diminuido\n");
    disp_basis_by_ron (&upper);

    if (detalhe) disp_basis_by_ron (&upper);
    
    zera_basis_by_ron (&diferenca);
    cont_Z = 1;

    for (apt_Z = Z; apt_Z; apt_Z = apt_Z->prox) {

      /* Verifica se Z <= upper */
      free_basis_by_ron (&base_Z);
      disp_basis_by_ron (&(apt_Z->base));
      copy_basis_by_ron (&(apt_Z->base), &base_Z);
      // transfer_basis_according_window (&base_Z, &W_duas_linhas);
      /* aqui base_Z esta' definido em W'' */
      retorno = verifica_bases_menor (&base_Z, &upper);
      // printf ("retorno = %d\n", retorno);

      if (beatriz) 
      printf ("========================================================\n");

      /* Se Z nao e' menor ou igual a upper, passe para o proximo Z */
      if (!retorno) {
        if (beatriz) printf ("Lower Bound %d ignorado\n", cont_Z++);
        continue;
      }

      if (beatriz) {
        printf ("Lower Bound %d considerado: %d intervalos\n", 
                 cont_Z++, apt_Z->base.total);
      }

      /* Neste ponto Z<=upper */
      /* Verifica se Z e' uma solucao */
      free_basis_by_ron (&base_X);
      copy_basis_by_ron (&(apt_Z->base), &base_X);

      free_basis_by_ron (&base_Y);
      dilate_basis (&base_X, str, &base_Y);

      /* transfere base_Y para a janela W' */
      transfer_basis_according_window (&base_Y, &base->win);

      igual = igual_basis_by_ron (&base_Y, base);
      if (igual) {
        // printf ("Tem uma solucao\n");
        tem = 0;
        for (apt_sol = sol; apt_sol && !tem; apt_sol = apt_sol->prox) {
          tem = igual_basis_by_ron (&base_X, &apt_sol->base);
        }
        if (!tem) {
          apt_sol = (trv_struc_list *) 
                     node_allocation (sizeof(trv_struc_list));
          zera_basis_by_ron (&apt_sol->base);
          copy_basis_by_ron (&base_X, &apt_sol->base);
          apt_sol->prox = sol;
          sol = apt_sol; cont_sol++;
        }
      }
      else { /* igual == 0 */
        if (beatriz) 
          printf ("Lower Bound %d nao considerado: %d intervalos\n", 
                   cont_Z++, apt_Z->base.total);
        continue;
      }

      /* Agora vamos ver as bases intermediarias entre base_Z e upper */
      // free_basis_by_ron (&base_Z);
      // copy_basis_by_ron (&(apt_Z->base), &base_Z);
      // transfer_basis_according_window (&base_Z, &W_duas_linhas);
      /* aqui base_Z esta' definido em W'' */

      /* Diminui o base_W de acordo com a janela W */
      // diminui_desmonta (&base_Z, &apt_Z->base.win);
      /* Calcula a diferenca upper - Z */
      free_basis_by_ron (&diferenca);
      if (beatriz) printf ("Calculando a diferenca\n");
      diff_basis_by_ron (&diferenca, &base_Z, &upper);
      // diminui_diferenca (&diferenca, &apt_Z->base.win);

      // printf ("========================================================\n");
      // printf ("Base Z:\n");
      // disp_basis_by_ron (&base_Z);
      // printf ("Base UPPER:\n");
      // disp_basis_by_ron (&upper);
      // printf ("Base Diferenca:\n");
      // disp_basis_by_ron (&diferenca);

      free_basis_by_ron (&desmonta);
      if (beatriz) printf ("Desmontando a diferenca\n");
      desmonta_uma_base (&desmonta, &diferenca);
      if (beatriz) printf ("desmonta = %d\n", desmonta.total);
      // printf ("Deu %d\n", testa_desmonta (&desmonta, &diferenca));

      // disp_basis_by_ron (&desmonta);

      // diminui_desmonta (&desmonta, &apt_Z->base.win);
      // printf ("desmonta diminuido = %d\n", desmonta.total);
      // printf ("Deu diminuido %d\n", testa_desmonta (&desmonta, &diferenca));

      // disp_desmonta_by_ron (&desmonta);

      // continue; // para ser retirado

      for (j=1; j<=desmonta.total; j++) {
        comb = NULL; num_comb = numero_de_combinacoes (desmonta.total, j);
        for (k=0; k<num_comb; k++) {
          if (beatriz) printf ("j = %d e k = %d/%d\n", j, k+1, num_comb);
          if (beatriz) {
            if (k==num_comb-1) {
              printf ("j = %d k = %d sol = %d\n", j, num_comb, cont_sol);
            }
          }
          _combinacoes_ (desmonta.total, j, &comb);
          free_basis_by_ron (&base_D);
          copy_win (&upper.win, &base_D.win);
          base_D.total = base_D.size = j;
          base_D.nv = upper.win.size;
          base_D.nb = (base_D.nv + NB1) / NB;
          base_D.ind = (int *) node_allocation (sizeof(int) * base_D.total);
          base_D.A = (int *) node_allocation (sizeof(int) * base_D.total * 
                                              base_D.nb);
          base_D.B = (int *) node_allocation (sizeof(int) * base_D.total * 
                                              base_D.nb);
          for (kk=0; kk<j; kk++) {
            base_D.ind[kk] = kk * base_D.nb;
          }
          for (kk=0; kk<j; kk++) {
            ind1 = desmonta.ind[comb[kk]];
            ind2 = base_D.ind[kk];
            for (rr=0; rr<base_D.nb; rr++) {
              base_D.A[ind2+rr] = desmonta.A[ind1+rr];
              base_D.B[ind2+rr] = (~desmonta.A[ind1+rr]);
            }
          }
          free_basis_by_ron (&base_E);
          copy_basis_by_ron (&base_Z, &base_E);
          free_basis_by_ron (&base_X);
          union_basis_by_ron (&base_E, &base_D, &base_X);
          free_basis_by_ron (&base_D);
          free_basis_by_ron (&base_E);
          /* aqui nos temos uma possivel solucao base_X */
          /* transfere base_X para a janela W */
          transfer_basis_according_window_menor (&base_X, &(apt_Z->base.win));

          /* testa se \delta_C base_X = base */
          free_basis_by_ron (&base_Y);
          dilate_basis(&base_X, str, &base_Y);

          /* transfere base_Y para a janela W' */
          transfer_basis_according_window (&base_Y, &base->win);

          igual = igual_basis_by_ron (&base_Y, base);
          if (igual) {
            tem = 0;
            for (apt_sol = sol; apt_sol && !tem; apt_sol = apt_sol->prox) {
              tem = igual_basis_by_ron (&base_X, &apt_sol->base);
            }
            if (!tem) {
              apt_sol = (trv_struc_list *) 
                        node_allocation (sizeof(trv_struc_list));
              zera_basis_by_ron (&apt_sol->base);
              copy_basis_by_ron (&base_X, &apt_sol->base);
              apt_sol->prox = sol;
              sol = apt_sol; cont_sol++;
            }
          }
        }
        if (comb) {
          free (comb);
          // printf ("Alguma coisa cheira mal\n");
        }
      }
    }

    free_basis_by_ron (&upper);
    free_basis_by_ron (&diferenca);
    free_basis_by_ron (&base_X);
    free_basis_by_ron (&base_D);
    free_basis_by_ron (&base_E);
    free_basis_by_ron (&desmonta);
    free_basis_by_ron (&base_Z);

    fd = fopen ("X.txt", "w");
    fprintf (fd, "%d\n", cont_sol);
    fclose (fd);

    if (beatriz) 
      printf ("========================================================\n");
    i = 0;
    if (beatriz) printf ("Temos %d Solucoes\n", cont_sol);
    * sai = sol;
    while (sol) {
      apt_sol = sol;
      sol = sol->prox;
      if (beatriz) {
        printf ("Solucao %d:\n", ++i);
        disp_basis_by_ron (&apt_sol->base);
      }
      append_basis_by_ron ("X.txt", &apt_sol->base);
      // free_basis_by_ron (&apt_sol->base);
      // free (apt_sol);
    }
  }

  return 1;
}


/*=*/ int lvsolbas_crescente (char * fname1, char * fname2, char * fname3) {
  trv_struc basis1;
  mat_str   str;
  trv_struc_list * lower;
  output_search * sai, * apt_sai;
  trv_struc_list * apt_trv, * aux_trv;
  trv_struc_list * solucao;
  int i;

  if (fname1 == NULL || fname2 == NULL) {
    fprintf (stderr, "lvsolbas () pointers to NULL\n");
    return 0;
  }

  zera_basis_by_ron (&basis1);
  read_itrv (fname1, &basis1);
  read_str (fname2, &str);

  detalhe = 0;
  printf ("Base de Entrada:\n");
  disp_basis_by_ron (&basis1);
  solucao = NULL;
  gera_lower_bounds_crescente_imp   (&lower, &basis1, &str);
  testa_equacao_dilatacao_crescente_imp (&basis1, &str, lower, &solucao);

  if (solucao == NULL) {printf ("Sem Solucao\n");}

  apt_trv = solucao; i =0 ;
  while (apt_trv) {
    printf ("Solucao %d\n", ++i);
    disp_basis_by_ron (&apt_trv->base);
    aux_trv = apt_trv;
    apt_trv = apt_trv->prox;
    free_basis_by_ron (&aux_trv->base);
    free (aux_trv);
  }


  /* Caso Crescente, com feasible sets */
  /*
   * sai = NULL;
   * search_increasing_all (&basis1, &sai);

   *  while (sai) {
   *   apt_sai = sai;
   *   sai = sai->prox;
   *   apt_trv = apt_sai->list_base;
   *   while (apt_trv) {
   *     aux_trv = apt_trv;
   *     apt_trv = apt_trv->prox;
   *     free_basis_by_ron (&aux_trv->base);
   *     free (aux_trv);
   *   }
   *   free_win_by_ron (&apt_sai->C);
   *   free (apt_sai);
   * }
   */
   /* fim caso crescente */

  /* Caso crescente 2 */
  // paralelo_para_sequencial (&basis1);
  /* fim caso Crescente 2 */

  free_basis_by_ron (&basis1); 
  if (str.tag == MM_IMG && str.dat) free (str.dat);

  return 1;
}

/*=*/ int main (int argc, char * * argv) {
  int detalhe = 1;
  beatriz = 0;

  if (detalhe) {
    if (!lvsolbas_crescente (argv[1], argv[2], argv[3])) {
      fprintf(stderr, "vlsol : lvsolbas() failed\n");
      return 1;
    }
  }
  
  if (!detalhe) testa_gera_lower_bounds ();
  return 0;
}
