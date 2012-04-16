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


/*=*/ int r_testa_equacao_dilatacao_r (trv_struc * base, mat_str * str, 
                                       trv_struc_list * Z) {
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

  if (base == NULL || str == NULL || Z == NULL) {
    fprintf (stderr, "r_testa_equacao_dilatacao_r () pointers to NULL\n");
    return 0;
  }

  sol = NULL;
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
    // printf ("Upper Bound:\n");
    // disp_basis_by_ron (&upper);

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

    // printf ("Upper Bound Diminuido\n");
    // disp_basis_by_ron (&upper);

    if (detalhe) disp_basis_by_ron (&upper);
    
    zera_basis_by_ron (&diferenca);
    cont_Z = 1;
    for (apt_Z = Z; apt_Z; apt_Z = apt_Z->prox) {

      /* Verifica se Z <= upper */
      free_basis_by_ron (&base_Z);
      copy_basis_by_ron (&(apt_Z->base), &base_Z);
      // printf ("Lower Bound %d:\n", cont_Z);
      // disp_basis_by_ron (&base_Z);
      // transfer_basis_according_window (&base_Z, &W_duas_linhas);
      /* aqui base_Z esta' definido em W'' */
      retorno = verifica_bases_menor (&base_Z, &upper);
      // printf ("retorno = %d\n", retorno);

      printf ("========================================================\n");
      /* Se Z nao e' menor ou igual a upper, passe para o proximo Z */
      if (!retorno) {
        printf ("Lower Bound %d ignorado\n", cont_Z++);
        continue;
      }

      printf ("Lower Bound %d considerado: %d intervalos\n", cont_Z++, apt_Z->base.total);
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

      /* Agora vamos ver as bases intermediarias entre base_Z e upper */
      // free_basis_by_ron (&base_Z);
      // copy_basis_by_ron (&(apt_Z->base), &base_Z);
      // transfer_basis_according_window (&base_Z, &W_duas_linhas);
      /* aqui base_Z esta' definido em W'' */

      /* Diminui o base_W de acordo com a janela W */
      // diminui_desmonta (&base_Z, &apt_Z->base.win);
      /* Calcula a diferenca upper - Z */
      free_basis_by_ron (&diferenca);
      // printf ("Calculando a diferenca\n");
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
      // printf ("Desmontando a diferenca\n");
      desmonta_uma_base (&desmonta, &diferenca);
      // printf ("desmonta = %d\n", desmonta.total);
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
          // printf ("j = %d e k = %d/%d\n", j, k+1, num_comb);
          // if (k==num_comb-1) printf ("j = %d k = %d sol = %d\n", j, num_comb, cont_sol);
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

    // fd = fopen ("X.txt", "w");
    // fprintf (fd, "%d\n", cont_sol);
    // fclose (fd);

    printf ("========================================================\n");
    i = 0;
    printf ("Temos %d Solucoes\n", cont_sol);
    while (sol) {
      apt_sol = sol;
      sol = sol->prox;
      printf ("Solucao %d:\n", ++i);
      disp_basis_by_ron (&apt_sol->base);
      // append_basis_by_ron ("X.txt", &apt_sol->base);
      free_basis_by_ron (&apt_sol->base);
      free (apt_sol);
    }
  }

  return 1;
}

/*=*/ int main (int argc, char * * argv) {
  trv_struc basis1, base_sai;
  mat_str   str;
  trv_struc_list * lower;
  output_search * sai, * apt_sai;
  trv_struc_list * apt_trv, * aux_trv;
  char * fname1, * fname2, * fname3;

  fname1 = argv[1];
  fname2 = argv[2];
  fname3 = argv[3];
  beatriz = 0;

  zera_basis_by_ron (&basis1);
  read_itrv (fname1, &basis1);
  read_str (fname2, &str);
  
  detalhe = 0;

  /* Caso Geral */
  gera_lower_bounds_novo  (&lower, &basis1, &str);
  r_testa_equacao_dilatacao_r (&basis1, &str, lower);

  free_basis_by_ron (&basis1); 
  if (str.tag == MM_IMG && str.dat) free (str.dat);

  return 1;
}

