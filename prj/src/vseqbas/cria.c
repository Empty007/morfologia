# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

extern int mask[NB];

int main (void) {
  win_struc win;
  int compr, i, j, x, k;
  int nshifts, section;
  trv_struc base;
  int ind;
  int * winidx;
  int cont;
  int M[1000][1000];

  /* criando a janela 111 */
  // largura da janela
  scanf ("%d", &win.hsize);
  win.wsize =  win.hsize;

  for (i=0; i<win.hsize; i++) {
    for (j=0; j<win.hsize; j++) {
      M[i][j] = 0;
    }
  }

  compr = (win.hsize*win.wsize+NB1)/NB;
  win.data = (int *) node_allocation (sizeof (int)*compr);
  for (i=0; i<compr; i++) {
    win.data[i] = 0;
  }

  cont = 0;
  for (i=0; i<win.hsize; i++) {
    for (j=0; j<win.hsize; j++) {
      scanf ("%d", &x);
      if (x) {
        M[i][j] = cont++;
        x = i*win.hsize + j;
        nshifts = x%NB;
        section = x/NB;
        win.data[section] |= mask[nshifts];
      }
    }
  }

  // numero de variaveis
  // scanf ("%d", &win.size);
  win.size = cont;

  // disp_win_by_ron (&win);

  // Gera a base
  zera_basis_by_ron (&base);

  base.nv = win.size;
  base.nb = (win.size+NB1)/NB;

  // Le o numero de intervalos
  scanf ("%d", &base.total);
  base.size = base.total;

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

  for (k=0; k<base.total; k++) {
    ind = base.ind[k];
    for (i=0; i<win.hsize; i++) {
      // Le uma linha do lado esquerdo 
      for (j=0; j<win.hsize; j++) {
        scanf ("%d", &x);
        if (x) {
          nshifts = M[i][j]%NB;
          section = M[i][j]/NB;
          base.A[ind+section] |= mask[nshifts];
        }
      }
      // Le uma linha do lado direito 
      for (j=0; j<win.hsize; j++) {
        scanf ("%d", &x);
        if (x) {
          nshifts = M[i][j]%NB;
          section = M[i][j]/NB;
          base.B[ind+section] |= mask[nshifts];
        }
      }
    }
  }

  ind = 0;
  for (i=0; i<base.total; i++) {
    ind = base.ind[i];
    for (j=0; j<base.nb; j++) {
      base.B[j+ind] = ~base.B[j+ind];
    }
  }

  disp_basis_by_ron (&base);

  write_itrv ("base.txt", &base);

  free_basis_by_ron (&base);
  free_win_by_ron (&win);
  return 0;
}
