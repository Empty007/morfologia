# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

typedef struct elem_pilha {
  int dil;
  win_struc C;
} _elem_pilha_;

extern int mask[NB];
extern int imprime, geral, detalhe, beatriz;
extern _elem_pilha_ pilha[5000];
extern int topo;
extern int num_sol;
int imprime_crescente = 0;

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


/*=*/ int search_increasing_all_direto (trv_struc * base, output_search * * sai) {
  trv_struc minimo;
  win_struc S, C;
  int * comb, num_comb, tam_idx, *winidx, i, j, k, tem;
  int section, nshifts, tam, invariante;
  mat_str str;
  trv_struc_list * lower;
  win_struc_list * Lista_C, * apt_Lista_C;
  int tam_Lista_C;
  output_search * apt_sai;
  trv_struc_list * apt_trv, * solucao;

  zera_basis_by_ron (&minimo);
  zera_win_by_ron (&S);
  zera_win_by_ron (&C);
  str.dat = NULL;
  Lista_C = NULL;
  *sai = NULL;

  // disp_basis_by_ron (base);

  minimo_lado_esquerdo (&minimo, base);

  // disp_basis_by_ron (&minimo);

  esse_min_lado_esquerdo_min (&S, &minimo, base);

  // printf ("Menor Elemento Estruturante: N. de Pontos = %d\n", S.size);
  // disp_win_by_ron (&S);

  win_index (&S, &winidx);
  tam_idx = S.size;
  copy_win (&S, &C);
  tam = (S.hsize*S.wsize + NB1) / NB;
  tam_Lista_C = 0;
  for (i=1; i<=tam_idx; i++) {
    comb = NULL; num_comb = numero_de_combinacoes (tam_idx, i);
    for (j=0; j<num_comb; j++) {
      _combinacoes_ (tam_idx, i, &comb);
      if (i>1) {
        for (k=0; k<tam; k++) C.data[k] = 0;
        C.size = 0;
        for (k=0; k<i; k++) {
          section = winidx[comb[k]] / NB;
          nshifts = winidx[comb[k]] % NB;
          C.data[section] |= mask[nshifts];
          (C.size)++;
        }
        /* Neste ponto temos um C gerado */
        /* verifica se C e' invariante de S */
        invariante = invariante_window (&S, &C);
        if (invariante) {
          tem = 0;
          for (apt_Lista_C = Lista_C; apt_Lista_C && !tem; apt_Lista_C =
               apt_Lista_C->prox) {
            if (C.size == apt_Lista_C->win.size) {
              tem = invariante_window (&C, &apt_Lista_C->win);
            }
          }
          if (!tem) {
            apt_Lista_C = (win_struc_list *) 
                          node_allocation (sizeof(win_struc_list));
            copy_win (&C, &apt_Lista_C->win);
            apt_Lista_C->prox = Lista_C;
            Lista_C = apt_Lista_C;
            tam_Lista_C++;
          }
        }
      }
    }
    if (comb) free (comb);
  }

  while (Lista_C) {
    apt_Lista_C = Lista_C;
    Lista_C = Lista_C->prox;
    if (str.dat) {free (str.dat); str.dat = NULL;}
    window_to_str (&apt_Lista_C->win, &str);
    if (beatriz) {
      printf ("Structuring Element:\n");
      disp_win_by_ron (&apt_Lista_C->win);
      printf ("\n");
    }
    gera_lower_bounds_crescente (&lower, base, &str);
    testa_equacao_dilatacao_crescente (base, &str, lower, &solucao);
    apt_sai = (output_search *) node_allocation (sizeof(output_search));
    copy_win (&apt_Lista_C->win, &apt_sai->C);
    apt_sai->list_base = solucao;
    apt_sai->prox = * sai;
    * sai = apt_sai;
    free_win_by_ron (&apt_Lista_C->win);
    free (apt_Lista_C);
  }

  free_basis_by_ron (&minimo);
  free_win_by_ron (&S);
  free_win_by_ron (&C);
  if (winidx) free (winidx);
  if (str.dat) free (str.dat);
  return 1;
}

/*=*/ int paralelo_para_sequencial_direto (trv_struc * base) {
  output_search * sai1, * apt_sai, * sai2, * sai;
  trv_struc_list * solucao, * apt_sol;
  int i, lado, dil, ori;
  trv_struc base1;
  win_struc win_ori;
  int conta_pontos;

  if (imprime_crescente) printf ("Ini Base de Entrada da Funcao:\n");
  if (imprime_crescente) disp_basis_by_ron (base);
  if (imprime_crescente) printf ("Fim Base de Entrada da Funcao:\n");

  zera_win_by_ron (&win_ori);
  zera_basis_by_ron (&base1);

  lado = menor_lado_possivel_da_janela (&base->win);
  transform_to_small_window (&base->win, lado);
  if (base_identidade (base)) {
    /* encontrou uma solucao */
    num_sol++;
    printf ("Ini Solucao %d =================================================\n", num_sol);
    for (i=0; i<topo; i++) {
      if (pilha[i].dil == 1) 
        printf ("dil\n");
      else
        printf ("ero\n");
      lado = menor_lado_possivel_da_janela (&pilha[i].C);
      transform_to_small_window (&pilha[i].C, lado);
      disp_win_by_ron (&pilha[i].C);
    }
    printf ("Fim Solucao %d =================================================\n", num_sol);
    return 1;
  }

  conta_pontos = conta_pontos_da_extremidade_esquerda (base);
  if (base->total == 1 && conta_pontos == 1) {
    pilha[topo].dil = 1;
    copy_win (&base->win, &pilha[topo].C);
    transpoe_window (&pilha[topo++].C);
    if (imprime_crescente) printf ("Empilhou: topo = %d\n", topo);
    if (imprime_crescente) printf ("Elem. Estruturante:\n");
    if (imprime_crescente) disp_win_by_ron (&pilha[topo-1].C);
    if (imprime_crescente) printf ("\n");
    free_basis_by_ron (&base1);
    copy_basis_by_ron (base, &base1);
    free_win_by_ron (&win_ori);
    copy_win (&base->win, &win_ori);
    for (i=0; i<(win_ori.wsize*win_ori.hsize+NB1)/NB; i++) 
      win_ori.data[i] = 0;
    ori = ((win_ori.hsize - 1) / 2) * win_ori.hsize + ((win_ori.wsize - 1)/2);
    win_ori.data[ori/NB] |= mask[ori%NB];
    free_win_by_ron (&base1.win);
    copy_win (&win_ori, &base1.win);
    free_win_by_ron (&win_ori);
    paralelo_para_sequencial_direto (&base1);
    free_basis_by_ron (&base1);
    free_win_by_ron (&pilha[topo].C);
    topo--;
    if (imprime_crescente) {
      printf ("*********************************************\n");
      printf ("* Desempilhou: topo = %02d                    *\n", topo);
      printf ("*********************************************\n");
    }
    if (imprime_crescente) printf ("Saiu base com 1 elemento deslocado\n");
    return 1;
  }

  /* pode ser uma dilatacao */
  sai1 = NULL;
  if (imprime_crescente) 
    printf ("Comeca Search_Increasing_All para dilatacao\n");
  search_increasing_all_direto (base, &sai1);
  if (imprime_crescente) 
    printf ("Termina Search_Increasing_All para dilatacao\n");

  if (sai1 == NULL) {
    if (imprime_crescente) printf ("Deu Dilatacao Vazia\n");
  }

  free_basis_by_ron (&base1);
  basis_dual_operator (base, &base1);
  if (imprime_crescente) printf ("Disp base dual\n");
  if (imprime_crescente) disp_basis_by_ron (&base1);
  if (imprime_crescente) printf ("\n");

  sai2 = NULL;
  if (imprime_crescente) 
    printf ("Comeca Search_Increasing_All para erosao\n");
  search_increasing_all_direto (&base1, &sai2);
  if (imprime_crescente) 
    printf ("Termina Search_Increasing_All para erosao\n");

  if (sai2 == NULL) {
    if (imprime_crescente) printf ("Deu Erosao Vazia\n");
  }

  if (sai1 == NULL && sai2 == NULL) {
    if (imprime_crescente) printf ("Sem saida: chegou a uma folha\n");
    return 1;
  }

  if (imprime_crescente) {
    printf ("Ini -----------------------------------\n");
    printf ("Saida da Dilatacao:\n");
    imprime_output_search (sai1); 
    printf ("Saida da Erosao\n");
    imprime_output_search (sai2);
    printf ("Fim -----------------------------------\n");
  }

  for (i=0; i<2; i++) {
    if (i==0) {sai = sai1; dil = 1;}
    if (i==1) {sai = sai2; dil = 0;}
    while (sai) {
      apt_sai = sai;
      sai = sai->prox;
      solucao = apt_sai->list_base;
      while (solucao) {
        apt_sol = solucao;
        solucao = solucao->prox;
        free_basis_by_ron (&base1);
        if (dil) {
          // printf ("Dilatacao\n");
          copy_basis_by_ron (&apt_sol->base, &base1);
        }
        else {
          // printf ("Erosao\n");
          basis_dual_operator (&apt_sol->base, &base1);
        }

        lado = menor_lado_possivel_da_janela (&apt_sai->C);
        transform_to_small_window (&apt_sai->C, lado);
        copy_win (&apt_sai->C, &pilha[topo].C);
        if (dil == 0) transpoe_window (&pilha[topo].C);
        pilha[topo++].dil = dil;
        if (imprime_crescente) 
          printf ("Empilhou: topo = %d dil = %d\n", topo, dil);

        paralelo_para_sequencial_direto (&base1);

        free_win_by_ron (&pilha[topo].C);
        topo--;

        if (imprime_crescente) {
          printf ("*********************************************\n");
          printf ("* Desempilhou: topo = %02d                    *\n", topo);
          printf ("*********************************************\n");
        }

        free_basis_by_ron (&apt_sol->base);
        free_basis_by_ron (&base1);
      }
      free_win_by_ron (&apt_sai->C);
    }
  }

  free_win_by_ron (&win_ori);
  if (imprime_crescente) printf ("Saiu do final\n");
  return 1;
}

/*=*/ int lvsolbas_crescente_paralelo_sequencial (char * fname1, 
                                                  char * fname2, 
                                                  char * fname3) {
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
  // printf ("Base de Entrada:\n");
  // disp_basis_by_ron (&basis1);

  paralelo_para_sequencial_direto (&basis1);

  free_basis_by_ron (&basis1); 
  if (str.tag == MM_IMG && str.dat) free (str.dat);

  return 1;
}

/*=*/ int main (int argc, char * * argv) {
  int detalhe = 1;
  beatriz = 0;

  printf ("Paralelo para Sequencial\n");
  if (detalhe) {
    if (!lvsolbas_crescente_paralelo_sequencial (argv[1], argv[2], argv[3])) {
      fprintf(stderr, "vlsol : lvsolbas() failed\n");
      return 1;
    }
  }
  
  if (!detalhe) testa_gera_lower_bounds ();
  return 0;
}
