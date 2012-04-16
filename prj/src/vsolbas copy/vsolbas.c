# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "fat.h"
# include "pac_def.h"

# define MAX_INTERVALOS 200

extern int mask[NB];

struct win_ron {
  struct elem * win;
  int h,w,nv;
};

typedef struct win_ron WIN_RON;

struct inter_ron {
  struct elem *A;
  struct elem *Bc;
  struct inter_ron * next;
};

typedef struct inter_ron L_INTER_RON;

struct base_ron {
  L_INTER_RON * base;
  int ni;
  WIN_RON * win;
};

typedef struct base_ron BASE_RON;

int print_str (char *str, int h, int w) {
  int j;
  for(j=0; j<h/2; j++)
    printf("%5s   %s\n", "", &str[2*j*w]) ;
  printf("%5d)  %s\n", 1, &str[2*(h/2)*w]) ;
  for(j=h/2+1; j<h; j++) 
    printf("%5s   %s\n", "", &str[2*j*w]) ;
  printf("\n") ;
  return 1;
}

int conta_struct_elem (struct elem *apt_elem) {
  int cont=0;
  while (apt_elem) {
    cont++; apt_elem = apt_elem->next;
  }
  return cont;
}

int print_struct_elem (struct elem *apt_elem) {
  while (apt_elem) {
    printf ("(%d, %d)\n", apt_elem->row, apt_elem->col); apt_elem = apt_elem->next;
  }
  return 1;
}

int free_struct_elem (struct elem *apt_elem) {
  /* mata uma "matrix" : usado somente por free_base_ron */
  struct elem * aux_elem;
  while (apt_elem) {
    aux_elem = apt_elem; apt_elem = apt_elem->next;
    free (aux_elem);
  }
  return 1;
}

int free_base_ron (BASE_RON * base) {
  struct elem * aux_elem;
  L_INTER_RON * apt_inter, * aux_inter;
  /* matando a janela */
  free_struct_elem (base->win->win);
  /* matando os intervalos */
  for (apt_inter = base->base; apt_inter;) {
    free_struct_elem (apt_inter->A);
    free_struct_elem (apt_inter->Bc);
    aux_inter = apt_inter; apt_inter = apt_inter->next;
    free (aux_inter);
  }
  return 1;
}

int nega_str_tudo (char *str, int h, int w) {
  int i;
  for (i=0; i<w*h*2 && i<2500; i++) {
    if (*str == '0') 
      *str = '1';
    else {
      if (*str == '1') *str = '0';
    }
    str++;
  }
  return 1;
}

int win_nina_ron (WIN_RON * win_ron, win_struc * win) {
  /* transforma a janela da nina para a minha janela */
  char back_car, left_car, right_car;
  static char strA[2500];
  int sizeA, * winidx, i, j;

  win_ron->h   = win->hsize;
  win_ron->w   = win->wsize;
  win_ron->nv  = win->size;

  set_cars ("-", "[", "]", &back_car, &left_car, &right_car);
  init_str (strA, 2500, win, back_car, left_car, right_car);
  win_index (win, &winidx);
  for (i=0; i<win->size; i++) {
    if ( (win->data[winidx[i]/NB] & mask[winidx[i]%NB]) != 0) 
      strA[2*winidx[i]] = '1';
    else strA[2*winidx[i]] = '0';
  }
  win_ron->win = assembly_matrix (strA, win->hsize, win->wsize, &sizeA);
  // print_matrix (win_ron->win, win->hsize);
  // print_struct_elem (win_ron->win);
  free (winidx);
  return 1;
}

int bas_nina_ron (BASE_RON * basis_out, trv_struc * basis_in) {
  char back_car, left_car, right_car;
  static char strA[2500], strB[2500];
  int ind, * winidx, i;

  struct elem *A, *Bc;
  int sizeA, sizeBc;
  L_INTER_RON * aux;

  // printf ("%u %u %u\n", basis_in->A[0], basis_in->A[1], basis_in->A[2]);

  set_cars ("-", "[", "]", &back_car, &left_car, &right_car);
  basis_out->ni = basis_in->total;

  basis_out->win = (WIN_RON *) node_allocation (sizeof (WIN_RON));
  win_nina_ron (basis_out->win, &(basis_in->win));

  init_str (strA, 2500, &(basis_in->win), back_car, left_car, right_car);
  init_str (strB, 2500, &(basis_in->win), back_car, left_car, right_car);
  win_index (&(basis_in->win), &winidx);

  // printf ("Win: ");
  // for (i=0; i<basis_in->nv; i++) printf ("%d ", winidx[i]);
  // printf ("\n");
  // for (i=0;i<3;i++) printf ("%u ", basis_in->win.data[i]);
  // printf ("\n");

  basis_out->base = NULL;
  for (i=0; i<basis_out->ni; i++) {
    ind = basis_in->ind[i];
    /* assembly matrix A */
    cfg2str (&basis_in->A[ind], basis_in->nv, winidx, basis_in->win.hsize,
	    basis_in->win.wsize, strA, 2500); 
    A = assembly_matrix (strA, basis_in->win.hsize, basis_in->win.wsize, 
                         &sizeA);
    // print_matrix (A, basis_in->win.wsize);
    // print_str (strA, basis_in->win.hsize, basis_in->win.wsize);

    /* assembly matrix Bc */
    cfg2str (&basis_in->B[ind], basis_in->nv, winidx, basis_in->win.hsize,
	    basis_in->win.wsize, strB, 2500);
    Bc = assembly_matrix (strB, basis_in->win.hsize, basis_in->win.wsize, 
                          &sizeBc); 
    aux       = (L_INTER_RON *) node_allocation (sizeof (L_INTER_RON));
    aux->A    = A;
    aux->Bc   = Bc;
    aux->next = NULL;
    if (basis_out->base) {
      aux->next = basis_out->base;
      basis_out->base = aux;
    }
    else basis_out->base = aux;
  }

  free (winidx);
  return 1;
}

int dados_win_ron_nina (int * dados, int h, int w, struct elem * apt) {
  int l_origem, c_origem;
  int lin, col, ind, block, nshifts, i;

  /* Calculo da origem */
  l_origem = (h-1)/2;
  c_origem = (w-1)/2;

  // print_struct_elem (apt);

  /* montando a janela */
  for (; apt; apt=apt->next) {
    lin = apt->row+l_origem;
    col = apt->col+c_origem;
    ind = lin * w + col;
    nshifts = ind % NB;
    block   = ind / NB;
    // printf ("h=%d w=%d lin=%d col=%d pos=%d block=%d nshifts=%d mask=%u\n", 
    // h,w,lin, col, ind, block, nshifts, mask[nshifts]);
    dados[block] |= mask[nshifts];
  }
  // for (i=0;i<3;i++) printf ("%u ", dados[i]);
  // printf ("\n");  
  return 1;
}

int dados_ron_nina (int * dados, int nv, int * winidx, int h, int w, struct elem * apt) {
  int l_origem, c_origem;
  int lin, col, ind, block, nshifts;
  int i;

  /* Calculo da origem */
  l_origem = (h-1)/2;
  c_origem = (w-1)/2;

  // print_struct_elem (apt);

  /* montando a janela */
  for (; apt; apt=apt->next) {
    lin = apt->row+l_origem;
    col = apt->col+c_origem;
    ind = lin * w + col;

    // for (i=0; i<nv; i++) printf ("%d ", winidx[i]);
    // printf ("\n");

    // printf ("lin=%d col=%d ind=%d h=%d w=%d nv=%d\n", lin,col,ind,h,w,nv);
    for (i=0; i<nv; i++) 
      if (winidx[i] == ind) break;
    if (i==nv) {
      fprintf (stderr, "vsolbas : dados_ron_nina () : pau\n");
      return 0;
    }

    nshifts = i % NB;
    block   = i / NB;
    // printf ("h=%d w=%d lin=%d col=%d block=%d nshifts=%d mask=%u\n", h,w,lin, col,
    // block, nshifts, mask[nshifts]);
    dados[block] |= mask[nshifts];
  }
  
  return 1;
}

int bas_ron_nina (trv_struc * basis_out, BASE_RON * basis_in) {
  int i,j, b, l_origem, c_origem;
  L_INTER_RON * apt;
  int lin, col, ind, block, nshifts;
  int * winidx;

  basis_out->total = basis_in->ni;
  basis_out->size  = basis_in->ni;
  basis_out->nv    = basis_in->win->nv;
  basis_out->nb    = (basis_in->win->nv+31)/32;
  basis_out->ind   = (int *) malloc (sizeof(int) * basis_in->ni);
  basis_out->A     = (int *) malloc (sizeof(int) * basis_in->ni 
                     * basis_out->nb);
  basis_out->B     = (int *) malloc (sizeof(int) * basis_in->ni 
                     * basis_out->nb);

  basis_out->win.hsize  = basis_in->win->h;
  basis_out->win.wsize  = basis_in->win->w;
  basis_out->win.size   = basis_in->win->nv;
  basis_out->win.data   = (int *) malloc (sizeof(int) * 
                          (basis_in->win->w * basis_in->win->w + NB1) / NB);

  /* Inicializa os vetores */
  for (i=0; i<(basis_in->win->w * basis_in->win->w + NB1) / NB; i++)
    basis_out->win.data[i] = 0;

  // print_struct_elem (basis_in->win->win);

  for (i=0, b=0; i<basis_in->ni;i++,b+=basis_out->nb) {
    basis_out->ind[i] = b;
    for (j=0; j<basis_out->nb; j++) {
      basis_out->A[b+j] = basis_out->B[b+j] = 0;
    }
  }

  dados_win_ron_nina (basis_out->win.data, basis_in->win->h, basis_in->win->w, 
                      basis_in->win->win);

  win_index (&(basis_out->win), &winidx);
  
  apt = basis_in->base;
  for (i=0; i<basis_in->ni;i++) {
    b = basis_out->ind[i];
    // printf ("%d = %d\n", i, b);
    dados_ron_nina (& (basis_out->A[b]), basis_in->win->nv, winidx, basis_in->win->h, 
                    basis_in->win->w, apt->A);
    // print_matrix (apt->A, basis_in->win->w);
    // printf ("= %u\n", basis_out->A[b]);
    dados_ron_nina (& (basis_out->B[b]), basis_in->win->nv, winidx, basis_in->win->h, 
                    basis_in->win->w, apt->Bc);
    apt=apt->next;
  }
  free (winidx);
  return 1;
}

int lvsolbas (char * fname1, char * fname2) {
  trv_struc basis1, basis2, basis3;
  int       st;
  BASE_RON  base_ron;

  reset_basis (&basis1);
  st = read_itrv (fname1, &basis1);
  if (st == 0) {
    fprintf (stderr, "lvsolbas : read_itrv() failed\n");
    free_itrv (&basis1, &base_ron);
    return 0;
  }

  bas_nina_ron (&base_ron, &basis1);
  free_itrv (&basis1);
  bas_ron_nina (&basis1, &base_ron);
  write_itrv (fname2, &basis1);

  free_itrv (&basis1);
  free_base_ron (&base_ron);
  return 1;
}


int main (int argc, char * * argv) {

    if (!lvsolbas(argv[1], argv[2])) {
       fprintf(stderr, "vlsol : lvsolbas() failed\n") ;
       return 1;
    }

    return 0;
}
