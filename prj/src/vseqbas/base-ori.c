# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

extern int mask[NB];

int main (void) {
  win_struc win;
  int compr, i, j;
  int nshifts, section;
  trv_struc base;
  int ind;
  int * winidx;
  /* criando a janela 111 */
  win.size  = 3;
  win.hsize = 5;
  win.wsize = 5;

  compr = (win.hsize*win.wsize+NB1)/NB;
  win.data = (int *) node_allocation (sizeof (int)*compr);
  for (i=0; i<compr; i++) win.data[i] = 0;

  for (i=11; i<14; i++) {
    nshifts = i%NB;
    section = i/NB;
    win.data[section] |= mask[nshifts];
  }
  /* fim da criacao da janela */

  zera_basis_by_ron (&base);

  base.nv = win.size;
  base.nb = (win.size+NB1)/NB;
  base.total = base.size = 1;
  base.A = (int *) node_allocation (sizeof(int)*base.total*base.nb);
  base.B = (int *) node_allocation (sizeof(int)*base.total*base.nb);
  base.ind = (int *) node_allocation (sizeof(int)*base.total);
  copy_win (&win, &base.win);

  ind = 0;
  for (i=0; i<base.total; i++) {
    base.ind[i] = ind;
    for (j=0; j<base.nb; j++) {
      base.A[j+ind] = 0;
      base.B[j+ind] = 0;
    }
    ind += base.nb;
  }

  /* primeiro elemento da base [010,111] */
  ind = base.ind[0];
  nshifts = 0%NB;
  section = 0/NB;
  base.B[ind+section] |= mask[nshifts];

  nshifts = 1%NB;
  section = 1/NB;
  base.A[ind+section] |= mask[nshifts];
  base.B[ind+section] |= mask[nshifts];

  nshifts = 2%NB;
  section = 2/NB;
  base.B[ind+section] |= mask[nshifts];

  base.B[ind+section] = ~base.B[ind+section];

  disp_basis_by_ron (&base);

  write_itrv ("base.txt", &base);

  free_basis_by_ron (&base);
  free_win_by_ron (&win);
  return 0;
}
