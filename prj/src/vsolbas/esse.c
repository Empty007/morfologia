int aha_W_opcional (trv_struc * sai, trv_struc * base, mat_str * str) {
  trv_struc elle, erre;
  int i, j, ind1, ind2, ii, jj, ind3, k;

  if (sai == NULL || base == NULL || str == NULL) {
    fprintf (stderr, "aha_W () pointers to NULL\n");
    return 0;
  }

  if (base->size != 1) {
    fprintf (stderr, "aha(): The basis must represent just one interval\n");
    return 0;
  }

  elle.total = elle.size = 0;
  elle.nv = elle.win.size = 0;
  elle.nb = elle.win.wsize = elle.win.hsize = 0;
  elle.A = elle.B = elle.ind = elle.win.data = NULL;

  erre.total = erre.size = 0;
  erre.nv = erre.win.size = 0;
  erre.nb = erre.win.wsize = erre.win.hsize = 0;
  erre.A = erre.B = erre.ind = erre.win.data = NULL;

  elle_W (&elle, base, str);
  erre_W (&erre, base, str);

  sai->total = sai->size = elle.total * erre.total * 2;
  sai->nv = elle.nv;
  sai->nb = (sai->nv+NB1)/NB;
  sai->A = sai->B = sai->ind = sai->win.data = NULL;
  copy_win (&elle.win, &sai->win);
  sai->A   = (int *) node_allocation (sizeof(int) * elle.total * 
                                      erre.total * elle.nb * 2);
  sai->B   = (int *) node_allocation (sizeof(int) * elle.total * 
                                      erre.total * elle.nb * 2);
  sai->ind = (int *) node_allocation (sizeof(int) * elle.total * 
                                      erre.total * 2);

  ind3 = 0; k = 0;
  for (i=0; i<elle.total; i++) {
    ind1 = elle.ind[i];
    for (j=0; j<erre.total; j++) {
      ind2 = erre.ind[j];
      sai->ind[k] = ind3;
      for (ii=0; ii<elle.nb; ii++) {
        sai->A[ind3+ii] = elle.A[ind1+ii];
        sai->B[ind3+ii] = elle.B[ind1+ii];
      }
      k++; ind3 += elle.nb;
      sai->ind[k] = ind3;
      for (jj=0; jj<erre.nb; jj++) {
        sai->A[ind3+jj] = erre.A[ind2+jj];
        sai->B[ind3+jj] = erre.B[ind2+jj];
      }
      k++; ind3 += elle.nb;
    }
  }

  free_itrv (&elle);
  free_itrv (&erre);
  return 1;
}


int esse_W (trv_struc * * saida, trv_struc * base, mat_str * str, int * tam_sai) {
  trv_struc * quebrada;
  int i, j, k, jj, * vet_apt, total, ind, tem;
  aha_struc * aha;
  trv_struc * esse, base1, base2;
  trv_struc * sai;

  if (saida == NULL || base == NULL || str == NULL || tam_sai == NULL) {
    fprintf (stderr, "esse_W () pointers to NULL\n");
    return 0;
  }

  quebrada = (trv_struc *) node_allocation (sizeof (trv_struc) * 
                                            base->total);
  quebra_base_intervalos (quebrada, base);
  aha = (aha_struc *) node_allocation (sizeof (aha_struc) * base->total);
  vet_apt = (int *) node_allocation (sizeof (int) * base->total);
  for (i=0; i<base->total; i++) {
    aha_W (&aha[i], &quebrada[i], str);
    printf ("i = %02d => %03d Pairs\n", i, aha[i].total);
    free_itrv (&quebrada[i]);
  }

  if (quebrada) free (quebrada);

  vet_apt = (int *) node_allocation (sizeof (int) * base->total);
  for (i=0, total=1; i<base->total; i++) {
    vet_apt[i] = 0;
    if (aha[i].total) total *= aha[i].total;
    printf ("total = %d aha[%d].total = %d\n", total, i, aha[i].total);
  }

  esse = (trv_struc *) node_allocation (sizeof (trv_struc) * base->total);
  for (i=0; i<base->total; i++) { /* para cada conjunto H */
    copy_win (&aha[i].win, &esse[i].win);
    esse[i].total = esse[i].size = 2;
    esse[i].nv = aha[i].nv;
    esse[i].nb = aha[i].nb;
    esse[i].ind = (int *) node_allocation (sizeof(int)*esse[i].total);
    esse[i].A = (int *) node_allocation (sizeof(int)*esse[i].total*esse[i].nb);
    esse[i].B = (int *) node_allocation (sizeof(int)*esse[i].total*esse[i].nb);
    for (j=0; j<esse[i].total; j++) esse[i].ind[j] = j*esse[i].nb;
  }

  printf ("chegou %d\n", total);

  *saida = (trv_struc *) node_allocation (sizeof (trv_struc) * total);
  sai = *saida;
  *tam_sai = 0;

  for (i=0; i<total; i++) { /* num. total de prod. cartesiano */
    for (j=0; j<base->total; j++) { /* para cada conjunto H */
      if (aha[j].total) {
        for (k=0; k<aha[j].intervs[vet_apt[j]].total; k++) {
          ind = aha[j].intervs[vet_apt[j]].ind[k];
          for (jj=0; jj<aha[j].nb; jj++) {
            esse[j].A[ind+jj] = aha[j].intervs[vet_apt[j]].A[ind+jj];
            esse[j].B[ind+jj] = aha[j].intervs[vet_apt[j]].B[ind+jj];
          }
        }
      }
    }

    base1.total = base1.size = 0;
    base1.nv = base1.win.size = 0;
    base1.nb = base1.win.wsize = base1.win.hsize = 0;
    base1.A = base1.B = base1.ind = base1.win.data = NULL;

    for (j=0; j<base->total; j++) { /* para cada conjunto H */
      if (aha[j].total) break;
    }

    if (j<base->total) copy_basis_by_ron (&(esse[j]), &base1);
    // disp_basis_by_ron (&base1);

    for (j++; j<base->total; j++) { /* para cada conjunto H */
      // disp_basis_by_ron (&base1);
      // printf ("j = %d\n", j);
      if (aha[j].total) {
        union_basis_by_ron (&base1, &esse[j], &base2);
        free_itrv (&base1);
        copy_basis_by_ron (&base2, &base1);
        free_itrv (&base2);
      }
    }

    printf ("i = %d, base1.total = %d total = %d\n", i, base1.total, total);
    if (base1.total) {
      for (j=0, tem=0; j<*tam_sai && !tem; j++) {
        tem = igual_basis_by_ron (&sai[j], &base1);
      }
      printf ("tem = %d\n", tem);
      if (!tem) {
        copy_basis_by_ron (&base1, &sai[*tam_sai]);
        printf ("Base %d:\n", *tam_sai+1);
        disp_basis_by_ron (&sai[*tam_sai]);
        (*tam_sai)++;
      }
    }

    free_itrv (&base1);

    for (j=0; j<base->total; j++) /* para cada conjunto H */
      printf ("%d ", vet_apt[j]);
    printf ("\n");

    for (j=base->total-1; j>=0; j--) { /* para cada conjunto H */
      if (vet_apt[j] < aha[j].total-1) {vet_apt[j]++; break;}
      vet_apt[j] = 0;
    }
    for (j=0; j<base->total; j++) /* para cada conjunto H */
      printf ("%d ", vet_apt[j]);
    printf ("\n");
  }


  for (i=0; i<base->total; i++) {
    free_aha (&aha[i]);
    free_itrv (&esse[i]);
  }

  if (esse) free (esse);
  if (aha) free (aha);
  if (vet_apt) free (vet_apt);
  return 1;
}

// ==============================================================


int gera_um_Z (trv_struc * Z, int * vet_apt, aha_struc aha, 
               int tam_aha, trv_struc * esse) {
  trv_struc base1, base2;
  int j, k, jj, ind;

  for (j=0; j<base->total; j++) { /* para cada conjunto H */
    if (aha[j].total) {
      for (k=0; k<aha[j].intervs[vet_apt[j]].total; k++) {
        ind = aha[j].intervs[vet_apt[j]].ind[k];
        for (jj=0; jj<aha[j].nb; jj++) {
          esse[j].A[ind+jj] = aha[j].intervs[vet_apt[j]].A[ind+jj];
          esse[j].B[ind+jj] = aha[j].intervs[vet_apt[j]].B[ind+jj];
        }
      }
    }
  }

  base1.total = base1.size = 0;
  base1.nv = base1.win.size = 0;
  base1.nb = base1.win.wsize = base1.win.hsize = 0;
  base1.A = base1.B = base1.ind = base1.win.data = NULL;

  for (j=0; j<base->total; j++) { /* para cada conjunto H */
    if (aha[j].total) break;
  }

  if (j<base->total) copy_basis_by_ron (&(esse[j]), &base1);

  for (j++; j<base->total; j++) { /* para cada conjunto H */
    if (aha[j].total) {
      union_basis_by_ron (&base1, &esse[j], &base2);
      free_itrv (&base1);
      copy_basis_by_ron (&base2, &base1);
      free_itrv (&base2);
    }
  }

  copy_basis_by_ron (&base1, Z);
  free_itrv (&base1);

  for (j=0; j<base->total; j++) /* para cada conjunto H */
    printf ("%d ", vet_apt[j]);
  printf ("\n");

  for (j=base->total-1; j>=0; j--) { /* para cada conjunto H */
    if (vet_apt[j] < aha[j].total-1) {vet_apt[j]++; break;}
    vet_apt[j] = 0;
  }

  for (j=0; j<base->total; j++) /* para cada conjunto H */
    printf ("%d ", vet_apt[j]);
  printf ("\n");
}

typedef struct {
  str_struc base;
  str_struc * prox;
} trv_struc_list;

int chama_tudo (trv_struc * base, mat_str * str) {
  trv_struc * quebrada;
  int i, j, * vet_apt, total, tem;
  aha_struc * aha;
  trv_struc * esse, base1;
  trv_struc_list * sai, *apt_list;
  int tam_sai;
  // trv_struc_list * * saida;

  if (base == NULL || str == NULL || tam_sai == NULL) {
    fprintf (stderr, "esse_W () pointers to NULL\n");
    return 0;
  }

  quebrada = (trv_struc *) node_allocation (sizeof (trv_struc) * 
                                            base->total);
  quebra_base_intervalos (quebrada, base);
  aha = (aha_struc *) node_allocation (sizeof (aha_struc) * base->total);
  vet_apt = (int *) node_allocation (sizeof (int) * base->total);
  for (i=0; i<base->total; i++) {
    aha_W (&aha[i], &quebrada[i], str);
    printf ("i = %02d => %03d Pairs\n", i, aha[i].total);
    free_itrv (&quebrada[i]);
  }

  if (quebrada) free (quebrada);

  /* os conjuntos aha's ja' foram montados */

  vet_apt = (int *) node_allocation (sizeof (int) * base->total);
  for (i=0, total=1; i<base->total; i++) {
    vet_apt[i] = 0;
    if (aha[i].total) total *= aha[i].total;
    printf ("total = %d aha[%d].total = %d\n", total, i, aha[i].total);
  }

  esse = (trv_struc *) node_allocation (sizeof (trv_struc) * base->total);
  for (i=0; i<base->total; i++) { /* para cada conjunto H */
    copy_win (&aha[i].win, &esse[i].win);
    esse[i].total = esse[i].size = 2;
    esse[i].nv = aha[i].nv;
    esse[i].nb = aha[i].nb;
    esse[i].ind = (int *) node_allocation (sizeof(int)*esse[i].total);
    esse[i].A = (int *) node_allocation (sizeof(int)*esse[i].total*esse[i].nb);
    esse[i].B = (int *) node_allocation (sizeof(int)*esse[i].total*esse[i].nb);
    for (j=0; j<esse[i].total; j++) esse[i].ind[j] = j*esse[i].nb;
  }

  sai = NULL;
  *tam_sai = 0;

  for (i=0; i<total; i++) { /* num. total de prod. cartesiano */
    gera_um_Z (&base1, vet_apt, aha, base->total, esse);
    if (base1.total) {
      for (apt_list = sai, tem=0; apt_list && !tem; apt_list = apt_list->prox) {
        tem = igual_basis_by_ron (&apt_list->base, &base1);
      }
      if (!tem) {
        apt_list = (trv_struc_list *) node_allocation (sizeof (trv_struc_list));
        copy_basis_by_ron (&base1, &apt_list->base);
        apt_list->prox = sai;
        sai = apt_list;
        printf ("Base %d:\n", *tam_sai+1);
        disp_basis_by_ron (&sai->base);
        (*tam_sai)++;
      }
    }
  }

  /* finalizando e matando o que tem que matar */

  while (sai) {
    apt_list = sai;
    sai = sai->prox;
    free_itrv (&apt_list->base);
  }

  for (i=0; i<base->total; i++) {
    free_aha (&aha[i]);
    free_itrv (&esse[i]);
  }

  if (esse) free (esse);
  if (aha) free (aha);
  if (vet_apt) free (vet_apt);
  return 1;
}
