# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

extern int mask[NB];

int function_size_win ( win_struc  *win) {
  int  nshifts;
  int  mask;
  int  i, j, section;
  int  data, cont=0;

  for(i=0; i<win->hsize; i++) {
    for(j=0; j<win->wsize; j++) {
      nshifts = ((i*win->wsize)+j)%NB ;
      mask = 1<<nshifts ;
      section = ((i*win->wsize)+j)/NB ;
      data = win->data[section] ;
      if((data & mask) == mask) cont++;
    }
  }
  return cont;
}

int mata_vetor_de_basis_quebrado (trv_struc * ent, trv_struc * base) {
  int i;
  for (i=0; i<base->total; i++) {
    free (ent[i].A);
    free (ent[i].B);
    free (ent[i].ind);
    free (ent[i].win.data);
  }
  return 1;
}

/* ainda em desenvolvimento ... */
int quebra_base_intervalos (trv_struc * sai, trv_struc * base) {
  int i, ind, j;
  for (i=0; i<base->total; i++) {
    // sai[i] = (trv_struc *) node_allocation (sizeof (trv_struc));
    sai[i].A = (int *) node_allocation (sizeof (int) * base->nb);
    sai[i].B = (int *) node_allocation (sizeof (int) * base->nb);
    sai[i].ind = (int *) node_allocation (sizeof (int));
    sai[i].ind[0] = 0;
    sai[i].size = sai[i].total = 1; /* 1 intervalo */
    sai[i].nv = base->nv;
    sai[i].nb = base->nb;
    copy_win (&(base->win), &(sai[i].win));
  }
  for (i=0; i<base->total; i++) {
    ind = base->ind[i];
    for (j=0; j<base->nb; j++)
      sai[i].A[j]=base->A[ind+j];
      sai[i].B[j]=base->B[ind+j];
  }
  return 1;
}

int pontos_elemento_estruturante (int * X_ker, int * Y_ker, mat_str * ker) {
  int size, m, n, points, i, j;

  size = ker->m * ker->n;
  m = ker->m; n = ker->n;

  // Alocacao em antes da chamada
  // X_ker = (int *) node_allocation (sizeof (int) * size);
  // Y_ker = (int *) node_allocation (sizeof (int) * size);

  points = 0;
  switch (ker->tag) {
    case MM_IMG:
      for (i=0; i<size; i++)
        if (ker->dat[i] == 1) {
          X_ker[points] = i%m - (m-1)/2;
          Y_ker[points++] = (n-1)/2 - i/n;
        }
        break;
    case MM_MAT:
      for (i=0; i<3; i++)
        for (j=0; j<3; j++)
          if (ker->x[i][j] == 1) {
            X_ker[points] = i-1;
            Y_ker[points++] = 1-j;
          }
       break;
  }

  // free (X_ker);
  // free (Y_ker);

  return 1;
}

/* Translada uma base: usa erosao
 * By Ronaldo Fumio Hashimoto
 * 21, 22 e 23.01.2000
 */
int translada_basis (trv_struc *basis_out, trv_struc *basis_in, int x, int y) {
  mat_str str;
  int n_lin, n_col, l_ori, c_ori, lin, col, i, j;
  int maior, k_1, k_2;

  n_lin = 2*(y>0?y:-y)+1;
  n_col = 2*(x>0?x:-x)+1;

  lin = (y>=0?0:(n_lin-1));
  col = (x<=0?0:(n_col-1));

  maior = n_lin;
  if (maior<n_col) maior = n_col;
  if (maior%2==0) maior++;
  k_1 = (maior-1)/2 - (n_lin-1)/2;
  k_2 = (maior-1)/2 - (n_col-1)/2;

  lin += k_1;
  col += k_2; 

  /* construindo o elemento estruturante */
  str.tag = MM_IMG;
  str.m = maior; str.n = maior; str.p = 1;
  str.dat = (short *) node_allocation (sizeof (short) * maior * maior);
  for (i=0; i<maior*maior; i++) str.dat[i]=0;
  str.dat[lin*maior+col]=1;

  reset_basis (basis_out);
  erosion_basis (basis_in, &str, basis_out);
  
  free (str.dat);
  return 1;
}

/* Erosao de uma janela: baseado na funcao dilate_win
 * By Ronaldo Fumio Hashimoto
 * 21, 23 e 24.01.2000
 */
int erode_win (win_struc *win, win_struc *win_ero, mat_str *str) {
  int size, m, n, points, i, j;
  int *X_ker, *Y_ker;
  int *X_win, *Y_win, w, h, winsize, k, k_1, k_2;
  int  m_dil, n_dil, size_dil, x_dil, y_dil;
  mat_str str_rot, * ker;
  win_struc * win_vector;

  if (str == NULL) {
    fprintf (stderr, "There is no Structuring Element\n");
    return 0;
  }

  if (str->tag == MM_GRAPH) {
    fprintf (stderr, "Structuring Element Type MM_GRAPH is not supported\n");
    return 0;
  }

  /* transpondo o elemento struturante */
  lvrotstr (str, 4, 0, &str_rot);
  ker = &str_rot;
  // ker = str;
 
  size = ker->m * ker->n;
  m = ker->m; n = ker->n;

  X_ker = (int *) node_allocation (sizeof (int) * size);
  Y_ker = (int *) node_allocation (sizeof (int) * size);
  points = 0;
  switch (ker->tag) {
    case MM_IMG:
      for (i=0; i<size; i++)
        if (ker->dat[i] == 1) {
          X_ker[points] = i%n - (n-1)/2;
          Y_ker[points++] = (m-1)/2 - i/n;
        }
        break;
    case MM_MAT:
      for (i=0; i<3; i++)
        for (j=0; j<3; j++)
          if (ker->x[i][j] == 1) {
            X_ker[points] = i-1;
            Y_ker[points++] = 1-j;
          }
       break;
  }

  winsize = win->wsize * win->hsize;
  w = win->wsize; h = win->hsize;
  X_win = (int *) node_allocation (sizeof (int) * winsize);
  Y_win = (int *) node_allocation (sizeof (int) * winsize);

  for (i=0, k=0; i<winsize; i++) {
    k_1 = i/32;
    k_2 = i%32; 

    if (((win->data[k_1]) & (mask[k_2]))!= 0) {
      X_win[k] = i % w - (w-1)/2; 
      Y_win[k] = (h-1)/2 - i / h;
      k++;
    } 
  }

  // win_ero->wsize = win->wsize - 2 * (ker->m/2);
  // win_ero->hsize = win->hsize - 2 * (ker->n/2);

  win_ero->wsize = win->wsize>ker->m?win->wsize:ker->m;
  win_ero->hsize = win->hsize>ker->n?win->hsize:ker->n;

  // printf ("%d %d\n", win->hsize, win->wsize);
  // printf ("%d %d\n", ker->n, ker->m);
  // printf ("%d %d\n", win_ero->hsize, win_ero->wsize);

  m_dil = win_ero->wsize;
  n_dil = win_ero->hsize;
  size_dil = m_dil * n_dil;

  k_1 = (size_dil+31)/32;
  win_ero->data = (int *) node_allocation (sizeof(int)*k_1);
  for (j=0; j<k_1; j++) win_ero->data[j] = -1;

  win_vector = (win_struc *) node_allocation (sizeof(win_struc)*points);
  for (i=0; i<points; i++) {
    win_vector[i].data = (int *) node_allocation (sizeof(int)*k_1);
    for (j=0; j<k_1; j++) win_vector[i].data[j] = 0;
    win_vector[i].wsize = m_dil;
    win_vector[i].hsize = n_dil;
    win_vector[i].size  = win->size;
  }

  x_dil = (m_dil-1)/2;
  y_dil = (n_dil-1)/2;

  for (i=0; i<points; i++) {
    // printf ("i = %d: ", i);
    for (j=0; j<win->size; j++) {
      k = (x_dil+X_win[j]+X_ker[i])+(y_dil-Y_win[j]-Y_ker[i])*m_dil;
      k_1 = k/32;
      k_2 = k%32;
      // printf ("(%02d %1d %02d)\n", k, k_1, k_2);
      win_vector[i].data[k_1] |= mask[k_2];
      // disp_win (&win_vector[i]);
      // printf ("\n");
    }
    // printf ("xxxxxxxxxxxxxxxxxxxx\n");
  }

  // for (i=0; i<points; i++) {
  //     printf ("i = %d\n", i);
  //     disp_win (&win_vector[i]);
  //     printf ("\n");
  //   }

  k_1 = (size_dil+31)/32;
  for (i=0; i<points; i++) {
    for (j=0; j<k_1; j++) {
      win_ero->data[j] &= win_vector[i].data[j];
    }
  }

  win_ero->size  = function_size_win (win_ero);

  // disp_win (win); printf ("\n");
  // disp_win (win_ero);
  // printf ("\n");

  for (i=0; i<points; i++) free (win_vector[i].data);
  free (win_vector);
  free (X_win); free (Y_win);
  free (X_ker); free (Y_ker);
  free (str_rot.dat);
  return 1;
}

/* ainda em desenvolvimento ... */
int ele_W (trv_struc * sai, trv_struc * base, mat_str * str) {
  /* trv_struc * base represents only one interval */
  win_struc * win = &(base->win);
  mat_str str_rot;
  win_struc * win_ero, win_in;
  int i;

  if (base->size != 1) {
    fprintf (stderr, "The input basis must have just one interval\n");
    return 0;
  }

  /* transpondo o elemento struturante */
  lvrotstr (str, 4, 0, &str_rot);

  copy_win (&(base->win), &win_in);
  win_ero = &(sai->win);

  disp_win (&win_in);
  printf ("\n");

  /* construindo a janela $W=W'\ominus C^t$ */
  erode_win (&win_in, win_ero, &str_rot);

  disp_win (win_ero);

  free (win_in.data);
  if (str_rot.tag == MM_IMG) free (str_rot.dat);
  return 1;
}

int lvsolbas (char * fname1, char * fname2) {
  trv_struc basis1, basis2, basis3;
  int       st;
  mat_str   str;
  win_struc win;

  reset_basis (&basis1);
  st = read_itrv (fname1, &basis1);
  if (st == 0) {
    fprintf (stderr, "lvsolbas : read_itrv() failed\n");
    return 0;
  }

  // para testar erode_win
  // {
  //   read_str (fname2, &str);
  //   erode_win (&(basis1.win), &win, &str);
  // }

  { trv_struc * sai; int i;
    sai = (trv_struc *) node_allocation (sizeof (trv_struc) * basis1.total);
    quebra_base_intervalos (sai, &basis1);
    // for (i=0; i<basis1.total; i++) {
    //   disp_basis_by_ron (&sai[i]);
    // }
    read_str (fname2, &str);
    ele_W (&basis2, &sai[0], &str);
    // disp_basis_by_ron (&basis2);
    // free_itrv (&basis2); 
    mata_vetor_de_basis_quebrado (sai, &basis1);
  }

  // ainda testando: ele_W (&basis2, &basis1, &str);
  /*
  { int x, y;
    reset_basis (&basis2);
    printf ("x = "); scanf ("%d", &x);
    printf ("y = "); scanf ("%d", &y);
    translada_basis (&basis2, &basis1, x, y);
    write_itrv (fname2, &basis2);
    free_itrv (&basis2);
  }
  */

  // free_itrv (&basis1);
  // write_itrv (fname2, &basis1);
  free_itrv (&basis1); 

  // free_itrv (&basis2);
  if (str.tag == MM_IMG) free (str.dat);
  // if (basis2.win.data != 0) free (basis2.win.data);

  return 1;
}


int main (int argc, char * * argv) {

    if (!lvsolbas(argv[1], argv[2])) {
       fprintf(stderr, "vlsol : lvsolbas() failed\n") ;
       return 1;
    }

    return 0;
}
