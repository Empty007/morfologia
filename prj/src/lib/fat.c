# include <malloc/malloc.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# include "pac_win.h"
# include "pac_bas.h"
# include "fat.h"

void * node_allocation (size)
     int size;
{
  void * aux = malloc (size);
  if (aux == NULL) {
    fprintf (stderr, "My problem is alocation memory\n");
    exit (1);
  }
  return aux;
}

int igual (A, B) 
struct elem *A, *B;
{
  int sim = (A && B) || (A==NULL && B==NULL);
  while (A && B && sim) {
    if (A->row != B->row || A->col != B->col) 
      sim = 0;
    A = A->next;
    B = B->next;
  }
  return (sim && A == NULL && B == NULL);
}

int igual_translacao (A, B, row, col) 
struct elem *A, *B;
int *row, *col;
{
  int conta, contb;
  struct elem *ap;
  for (ap=A, conta=0; ap; ap=ap->next) {
    conta++;
  }
  for (ap=B, contb=0; ap; ap=ap->next) {
    contb++;
  }
  if (conta != contb) return 0;
  if (conta == 0) return 1;
  return (diff (A, B, row, col));
}


int dist_origem (A)
     struct elem * A;
{
  int minimo = A->row * A->row + A->col * A->col, distancia;
  A = A->next;
  while (A) {
    distancia = A->row * A->row + A->col * A->col;
    if (minimo > distancia) {
      minimo = distancia;
    }
    A = A->next;
  }
  return minimo;
}

int translada (ap, x, y)
     struct elem * ap;
     int x, y;
{
  for (; ap; ap = ap->next) {
    ap->row = ap->row-x;
    ap->col = ap->col-y;
  }
  return 1;
}


int nova_origem (out, x, y)
     struct output * out;
     int x, y;
{
  translada (out->A, x, y);
  translada (out->Bc, x, y);
  translada (out->C, x, y);
  return 1;
}

int centro_massa (A, x, y)
     struct elem * A;
     int *x, *y;
{
  int cont = 0, lin = 0, col = 0;
  while (A) {
    lin += A->row;
    col += A->col;
    cont++;
    A = A->next;
  }
  *x = lin/cont; *y = col/cont;
  
  return 1;
}

struct filter_output * transforma (out)
  struct output * out;
{
  struct filter_output * filter;
  struct filter_output * aux;
  struct interval * aux_inter;
  struct output *ant, * atual, * rem;
  int size = sizeof (struct filter_output);
  struct elem  * elm, *aux_elm;
  int x, y;
  
  filter = NULL;
  
  while (out) {

    /* insiro matriz C, A e Bc em filter */
    aux = (struct filter_output *) node_allocation (size);
    aux->C = out->C;         /* matriz C          */
    aux->dimC = out->dimC;   /* dimensao matriz C */
    aux->base = (struct interval *) node_allocation (sizeof (struct interval));
    (aux->base)->A = out->A; /* matriz A          */
    (aux->base)->Bc = out->Bc; /* matriz Bc          */
    (aux->base)->next = NULL;
    aux->next = filter;
    filter = aux;
    
    /* procurar em out todos os C's que sao iguais ao C colocado em
    filter */
    ant = out; atual = out->next;
    while (atual) {
      x = y = 0;
      if (igual_translacao (aux->C, atual->C, &x, &y)) {
	/* aux = atual + (x, y) */
	int cont = 0, cont1 = 0;
	
        aux_inter = (struct interval *)
	             node_allocation (sizeof(struct interval));
/* ==================================================================== */
/*	{  
	  struct elem * angelica = aux->C;
	  while (angelica) {
	    cont++;
	    angelica = angelica->next;
	  }

	  angelica = atual->C;
	  while (angelica) {
	    cont1++;
	    angelica = angelica->next;
	  }
	  if (cont == 4 || cont1 == 4) {
	    printf ("atual removido = %d -- %d\n", cont1, cont);
	  }
	} */
/* ==================================================================== */
	/* devemos transladar atual de (x, y) */
        nova_origem (atual, x, y);	
        aux_inter->A = atual->A;
        aux_inter->Bc = atual->Bc;
        aux_inter->next = aux->base;
        aux->base = aux_inter; 
        rem = atual;
        atual = atual->next;
        ant->next = atual;

        for (elm = rem->C; elm; elm=aux_elm) {
	  aux_elm = elm->next;
	  free (elm);
        }
        free (rem);
      }
      else {
        ant = atual;
        atual = atual->next;
      }
    }
    
    rem = out;
    out = out->next;
    free (rem);
  }
  
  return filter;
} 

void inverte_list (list)
     struct elem * * list;
{
  struct elem * last, *on;

 if (*list != NULL) {
    last = NULL;
    on = *list;
    while ((*list)->next!= NULL) {
      *list = (*list)->next;
      on->next = last;
      last = on;
      on = *list;
    }
    on->next = last;
  }
}

void print_matrix (A, n)
     struct elem * A; int n;
{
  int i=0, j, i_new, k;
  char * line;
  int meio = (n-1)/2;
  struct elem * B;

  line = (char *) node_allocation (sizeof(char)*n);
  printf ("\n");
  while (A) {
    /* assembly a line */
    for (j=0; j<n; j++) line[j] = 0;

    for (k = A->row; A && A->row==k; A=A->next) {
      if (A->col+meio >= n || A->col+meio < 0) {
        printf ("test: Alerta!! passou %d\n", A->col+meio);
	exit (1);
      }
      line[A->col+meio]=1;
    }
    /* print lines between i and k */
    for (; i<k+meio; i++) {
      for (j=0; j<n; j++)
        printf ("%2d", 0);
      printf ("\n");
    }
    /* print a line k */
    for (j=0; j<n; j++)
      printf ("%2d", line[j]);
    printf ("\n");
    /* next line */
    i++;
  }
  /* print lines between i and n */
    for (; i<n; i++) {
      for (j=0; j<n; j++)
        printf ("%2d", 0);
      printf ("\n");
    }
  /* free line */
  free (line);
}

void monta_matriz (A, S, n)
     struct elem * A; char * * S; int n;
{
  int i=0, j, i_new, k;
  char * line;
  int meio = (n-1)/2;
  struct elem * B;
  
  line = (char *) node_allocation (sizeof(char)*n);
 
  while (A) {
    /* assembly a line */
    for (j=0; j<n; j++) line[j] = 0;

    B = A;
    for (k = A->row; A && A->row==k; A=A->next) {
      if (A->col+meio >= n || A->col+meio < 0) {
        printf ("test: Alerta!! passou %d\n", A->col+meio);
	exit (1);
      }
      line[A->col+meio]=1;
    }

    /* assembly lines between 0 and k-1 */
    for (; i<k+meio; i++) {
      for (j=0; j<n; j++) {
        *(S[i]+j) = 0;
      }
    }

    /* assembly the line k */
    for (j=0; j<n; j++) {
      *(S[i]+j) = line[j];
    }

    /* next line */
    i++;
  }
  /* assembly lines between i and n */
    for (; i<n; i++) {
      for (j=0; j<n; j++) {
        *(S[i]+j) = 0;
      }
    }
  /* free line */
  free (line);
}


struct coll_interval * * criate_buckets (maximo)
int maximo;
{
  int i, size;
  struct coll_interval * * C;

  size = (maximo + 1) * sizeof (struct coll_interval *);
  C = (struct coll_interval * *) node_allocation (size);

  for (i=0; i<=maximo; i++)
    C[i] = NULL;

  return C;
}

struct interval * create_interval (A, Bc)
     struct elem *A, *Bc; 
{
  int size;
  struct interval * inter;
  
  size = sizeof (struct interval);
  inter = (struct interval *) node_allocation (size);
  inter->A = A; inter->Bc = Bc; inter->next = NULL;

  return inter;
}

struct coll_interval * search_subcoll_bucket (sizeA, sizeBc, pt_bucket)
     int sizeA, sizeBc;
     struct coll_interval * * pt_bucket;
{

  struct coll_interval * pt_subcoll;
  
  for (pt_subcoll = pt_bucket[sizeA]; pt_subcoll; pt_subcoll =
       pt_subcoll->next) { 
    if (pt_subcoll->sizeBc == sizeBc)
      return pt_subcoll;
  }
  return NULL;
}  

struct coll_interval * ins_subcoll_bucket (pt_bucket, sizeA, sizeBc)
     struct coll_interval * * pt_bucket;
     int sizeA, sizeBc;
{

  struct coll_interval * aux;
  int size = sizeof (struct coll_interval);

  aux = (struct coll_interval *) node_allocation (size);
  aux->sizeBc = sizeBc;
  aux->inter = NULL;

  aux->next = pt_bucket[sizeA];
  pt_bucket[sizeA] = aux;
  
  return aux;
}

void ins_interval (pt_subcoll, inter)
     struct coll_interval * pt_subcoll;
     struct interval * inter;
{
  inter->next = pt_subcoll->inter;
  pt_subcoll->inter = inter;
}

struct elem * assembly_matrix (str, lins, cols, size_matrix)
     char * str; int lins, cols, *size_matrix;
{
  struct elem * matrix, *aux;
  int i, j, size = sizeof (struct elem);
  int origem_lin = (lins - 1) / 2, origem_col = (cols - 1) / 2;
  
    matrix = NULL; *size_matrix = 0;
    for (i=0; i<lins; i++) {
      for (j=0; j<cols; j++) {
        if (str[2*i*cols+2*j] == '1') {
	  (*size_matrix)++;
	  aux = (struct elem *) node_allocation (size);
	  aux->row = i-origem_lin; aux->col = j-origem_col;
	  aux->next = matrix; matrix = aux;
        }
      }
    }
  inverte_list (& matrix); 
  return matrix;
}

int print_bucket (pt_bucket, n)
     struct coll_interval * * pt_bucket;
     int n;
{
  struct interval * inter;
  struct coll_interval * pt_balde;
  int z = 0, i, j, cont = 0;
  char * * A, * * B;
  
  A = (char * *) node_allocation (sizeof (char *) * (n));
  for (j=0; j<n; j++) A[j] = (char *) node_allocation (sizeof (char) * (n));
  B = (char * *) node_allocation (sizeof (char *) * (n));
  for (j=0; j<n; j++) B[j] = (char *) node_allocation (sizeof (char) * (n));

  while (z <= n * n) {
    pt_balde = pt_bucket[z];
    while (pt_balde) {
      inter = pt_balde->inter;
      while (inter) {
	printf ("\n\n\n(%d) [%d][%d]\n\n", ++cont, z, pt_balde->sizeBc);
	monta_matriz (inter->A, A, n);
	monta_matriz (inter->Bc, B, n);
       
	for (i=0; i<n; i++) {
	  for (j=0; j<n; j++) {
	    printf ("%2d", *(A[i]+j));
	  }
	  printf ("%5s", "");
	  for (j=0; j<n; j++) {
	    printf ("%2d", *(B[i]+j));
	  }
	  printf ("\n");
	}
	inter = inter->next;
      }
      pt_balde = pt_balde->next;
    }
    z++;
  }
  
  for (j=0; j<n; j++) free (A[j]);
  free (A);

  for (j=0; j<n; j++) free (B[j]);
  free (B);

  return 1;
}


win_struc * transf_intervals (pt_buckets, fname, n_buckets, n)
struct coll_interval * * * pt_buckets;
char * fname; int * n_buckets, *n;

{
  struct interval * inter;
  struct coll_interval * pt_subcoll;
  int i, j, n_intervals, element;
  int sizeA, sizeBc;
  struct elem * A, *Bc;
  int           st;
  trv_struc     base;
  win_struc     *win, * win_out;
  static char   strA[2500];
  static char   strB[2500];
  int           ind, size;
  char          back_car, left_car, right_car;
  int           *winidx ;

   st = set_cars(" ", " " , " ", &back_car, &left_car, &right_car) ;
   if(st == 0) {
      return(NULL);
   }  
  
  st = read_itrv(fname, &base);
  if(st == 0) {
    fprintf(stderr, "transf_intervals : read_itrv() failed\n") ;
    return(NULL);
  }

  win_out = (win_struc *) node_allocation (sizeof (win_struc));
  win = &base.win;
  win_out->hsize = win->hsize;
  win_out->wsize = win->wsize;
  win_out->size  = win->size;
  size = (win->hsize * win->wsize + 31) / 32;
  win_out->data = (int *) node_allocation (sizeof (int) * size);
  for (i=0; i<size; i++) {
    win_out->data[i] = win->data[i];
  }

  n_intervals = base.total;
  * n = win->wsize;
  if (win->hsize > win->wsize)
    * n = win->hsize;
  
  if (*n % 2 == 0) (*n)++;
  * n_buckets = *n * *n;
  * pt_buckets = criate_buckets (*n_buckets);

  st = init_str(strA, 2500, &base.win, back_car, left_car, right_car);
  if(st == 0) {
      return(0) ;
  }
  st = init_str(strB, 2500, &base.win, back_car, left_car, right_car) ;
  if(st == 0) {
     return(0) ;
  }
  
  st = win_index(&base.win, &winidx) ;
  if(st == 0) {
     return(NULL) ;
  }
  
  for (i=0; i<n_intervals; i++) {
    ind = base.ind[i] ;
 
    /* assembly matrix A */
    cfg2str(&base.A[ind], base.nv, winidx, win->hsize, win->wsize, strA, 2500);
    A = assembly_matrix (strA, win->hsize, win->wsize, & sizeA);
    
    /* assembly matrix Bc */
    cfg2str(&base.B[ind], base.nv, winidx, win->hsize, win->wsize, strB, 2500) ;
    Bc = assembly_matrix (strB, win->hsize, win->wsize, & sizeBc);
    
    /* create interval [A, Bc] */
    inter = create_interval (A, Bc);

    /* verify if (sizeA, sizeBc) exists in bucket */
    pt_subcoll = search_subcoll_bucket (sizeA, sizeBc, * pt_buckets); 

    /* if it doesn't exist, then create a new node */
    if (!pt_subcoll) {
      pt_subcoll = ins_subcoll_bucket (*pt_buckets, sizeA, sizeBc);
    }

    /* insere the interval [A, Bc] into bucket (sizeA, sizeBc) */
    ins_interval (pt_subcoll, inter);
  }
  free_itrv(&base) ;
  free (winidx);
  /* print_bucket (* pt_buckets, *n); */
  return win_out;
}

int diff (base, inter, row, col)
     struct elem * base, * inter;
     int *row, *col;
{
  *row = inter->row - base->row;
  *col = inter->col - base->col;

  inter = inter->next; base = base->next;

  while (inter != NULL && inter->row - base->row == *row &&
                          inter->col - base->col == *col) {
    inter = inter->next; base = base->next;
  }
  return (inter == NULL);
}

void ins_output_AB (A, Bc, pt_out)
     struct elem * A,  * Bc;
     struct output * * pt_out;
{
  struct output * pt_aux_out;
  int size = sizeof (struct output);
  struct elem * aux;
  
  pt_aux_out = (struct output *) node_allocation (size);
  pt_aux_out->A = A; pt_aux_out->Bc = Bc;
  size = sizeof (struct elem);
  aux = (struct elem *) node_allocation (size);
  /* insere origin into matrix C */
  aux->col = aux->row = 0; aux->next = NULL; pt_aux_out->C = aux;
  pt_aux_out->next = * pt_out;
  *pt_out = pt_aux_out;
}

void ins_output_C (pt_out, x, y)
     struct output * * pt_out;
     int x, y;
{
  int size = sizeof (struct elem);
  struct elem * C = (struct elem *) node_allocation (size);
  C->row = x; C->col = y; C->next = (*pt_out)->C;
  (*pt_out)->C = C;
}

int verify (base, inter, x, y)
struct interval * base, * inter;
int *x, *y;
{
  int row1, col1, row2, col2;

  if (base->A) {
    if (!diff (base->A, inter->A, &row1, &col1)) {
      return 0;
    }
  }
  if (base->Bc) {
    if (!diff (base->Bc, inter->Bc, &row2, &col2)) {
      return 0;
    }
  }
  if (base->A && base->Bc) {
  *x=row1; *y=col1; return (row1==row2 && col1==col2);
  }
  if (!base->A && base->Bc) {
  *x=row2; *y=col2; return 1;
  }
  if (base->A && !base->Bc) {
  *x=row1; *y=col1; return 1;
  }
  if (!base->A && !base->Bc) {
  *x = *y = 0; return 1;
  }
  return 1;
}

int modulo (x)
     int x;
{
  if (x<0) return -x;
  else return x;
}

int dimension_matrix (elem, n)
     struct elem * elem;
     int n;
{
  int major_row = 0, major_col = 0, major;
  
  for (; elem; elem=elem->next) {
    if (major_row < modulo(elem->row))
      major_row = modulo(elem->row);
    if (major_col < modulo(elem->col))
      major_col = modulo(elem->col);
  }
  major = major_col;
  if (major_row > major_col) major = major_row;
  major = 2*major+1;
  if (major < n) major = n;
  return major;
}

void sort_matrix (output)
     struct output * output;
{
  char * * matrix;
  int i, j, half = (output->dimC-1)/2, tam = output->dimC;
  struct elem * elem;
  matrix =  (char * *) node_allocation (sizeof (char *) * tam);
  for (i=0; i<tam; i++) 
    matrix[i] = (char *) node_allocation (sizeof (char) * tam);
  for (i=0; i<tam; i++)
    for (j=0; j<tam; j++)
      *(matrix[i]+j) = 0;
  for (elem = output->C; elem; elem = elem->next) {
    int l = elem->row+half, c = elem->col+half;
    *(matrix[l]+c) = 1;
  }
  /* sorting ... */
  elem = output->C; 
  for (i=0; i<tam; i++)
    for (j=0; j<tam; j++)
      if (*(matrix[i]+j)) {
        elem->row = i - half; elem->col = j - half;
	elem = elem->next;
      }
  inverte_list (& elem); 
  for (i=0; i<tam; i++) 
    free (matrix[i]);
  free (matrix);
}

int choose_ABc (pt_inter)
     struct interval * * pt_inter;
{
  struct interval * ap = * pt_inter, * menor = * pt_inter;
  int menor_distancia, distancia;
  struct elem *A, *Bc;

  if (ap->A == NULL) return 0;
  menor_distancia = dist_origem (ap->A);
  ap = ap->next;
  while (ap) {
    distancia = dist_origem (ap->A);
    if (menor_distancia > distancia) {
      menor_distancia = distancia;
      menor = ap;
    }
    ap = ap->next;
  }
  if (menor != * pt_inter) {
    A = menor->A; Bc = menor->Bc;
    menor->A = (*pt_inter)->A; menor->Bc = (*pt_inter)->Bc;
    (*pt_inter)->A = A; (*pt_inter)->Bc = Bc;
  }
  
  return 1;
}


void fatora (pt_out, pt_inter, n)
     struct output * * pt_out;
     struct interval * * pt_inter;
     int n;
{
  struct interval * pt_aux_inter, * pt_rem_inter, * pt_ant_inter;
  int x, y;
  while (*pt_inter) {
    /* Let [A, Bc] be an interval called base */

    /* choose [A, Bc] */
    choose_ABc (pt_inter);
    
    /* [A, Bc] stored in *pt_inter */
    ins_output_AB ((*pt_inter)->A, (*pt_inter)->Bc, pt_out);

    pt_ant_inter = * pt_inter; /* remove elements from subcoll */
    pt_aux_inter = (*pt_inter)->next;

    while (pt_aux_inter) {
      if (verify (* pt_inter, pt_aux_inter, &x, &y)) {
        /* insere displacement into matrix C transposto*/
        ins_output_C (pt_out, -x, -y);
        /* remove element from .... */
        pt_rem_inter = pt_aux_inter;
        pt_ant_inter->next = pt_aux_inter->next;
        pt_aux_inter = pt_aux_inter->next;
        free (pt_rem_inter);
      }
      else {
	pt_ant_inter = pt_aux_inter;
        pt_aux_inter = pt_aux_inter->next;
      }
    }
    /* remove [A, Bc] (base) from bucket */
    pt_rem_inter = * pt_inter;
    *pt_inter = (*pt_inter)->next;
    free (pt_rem_inter);
    /* calcule dimension of matrix C */
    (*pt_out)->dimC = dimension_matrix ((*pt_out)->C, n);
    /* sort matrix C */
    sort_matrix (*pt_out);
  }
}

void make_all (pt_coll, size_coll, pt_out, n)
     struct coll_interval * * pt_coll;
     int size_coll;
     struct output * * pt_out;
     int n;
{
  struct coll_interval * pt_subcoll, * pt_rem_subcoll;
  int i;

  for (i=0; i<=size_coll; i++){
    pt_subcoll = pt_coll[i];
    while (pt_subcoll) {
      fatora (pt_out, &(pt_subcoll->inter), n);
      /* remove bucket (i, pt_subcoll->sizeBc) from pt_coll */
      pt_rem_subcoll = pt_subcoll;
      pt_subcoll = pt_subcoll->next;
      free (pt_rem_subcoll);
    }
    pt_coll[i] = NULL;
  }
}
void print_filter_output (pt_out, n)
     struct filter_output * pt_out;
     int n;
{
  struct interval * aux;
  char * * A, * * B;
  int i, j, cont;
  
  A = (char * *) node_allocation (sizeof (char *) * (n));
  for (j=0; j<n; j++) A[j] = (char *) node_allocation (sizeof (char) * (n));
  B = (char * *) node_allocation (sizeof (char *) * (n));
  for (j=0; j<n; j++) B[j] = (char *) node_allocation (sizeof (char) * (n));

  printf ("\n\n\nOutPut:\n");
  cont = 0;
  while (pt_out) {
    print_matrix (pt_out->C, pt_out->dimC);
    aux = pt_out->base;
    while (aux) {
      printf ("\n(%d)\n", ++cont);

      monta_matriz (aux->A, A, n);
      monta_matriz (aux->Bc, B, n);
      
      for (i=0; i<n; i++) {
	for (j=-4; j<=pt_out->dimC+3; j++) printf (" ");
        for (j=0; j<n; j++) {
	  printf ("%2d", *(A[i]+j));
 	}
        printf ("%5s", "");
        for (j=0; j<n; j++) {
	  printf ("%2d", *(B[i]+j));
        }
      printf ("\n");
      }
      aux = aux->next;
    }
    pt_out = pt_out->next;
  }
  
  for (j=0; j<n; j++) free (A[j]);
  free (A);

  for (j=0; j<n; j++) free (B[j]);
  free (B);

}

int C2hex (vetor, elem, n)
     int * vetor;
     struct elem * elem;
     int n; /* dimensao da matriz C */
{
  int meio = (n-1)/2, lin, col;
  int tam = (n*n + 31)/32;
  int i; /* indice do vetor em hexadecimal */
  int bit, aux;

  for (i=0; i<tam; i++) vetor[i] = 0;
  for (; elem; elem = elem->next) {
    lin = elem->row + meio;
    col = elem->col + meio;
    i = (lin * n + col) / 32;  /* indice do vetor */
    bit = (lin * n + col) % 32; /* descobre qual o bit a ser aceso */
    aux = 1 << bit; /* faz bit shift's para a esquerda do numero 1 */
    vetor[i] = vetor[i] | aux; /* faz ou log. para acender o bit-esimo elem */
  }
  return 1;
}

int AB2hex (vetor, elem, win, tab)
     int * vetor;
     struct elem * elem;
     win_struc * win;
     int * * tab;
{
  int hmeio = (win->hsize-1)/2, wmeio = (win->wsize-1)/2, lin, col;
  int tam = (win->size + 31)/32;
  int i; /* indice do vetor em hexadecimal */
  int bit, aux;

 
  for (i=0; i<tam; i++) vetor[i] = 0;
  for (; elem; elem = elem->next) {
    lin = elem->row + wmeio;
    col = elem->col + hmeio;
    i = *(tab[lin]+col)/32;  /* indice do vetor */
    if (tab[lin] == NULL || i == -1 || i>=tam)
      printf ("errado 3: %d, %d, %d\n", i, tab[lin], tam);
    bit = *(tab[lin]+col) % 32; /* descobre qual o bit a ser aceso */
    aux = 1 << bit; /* faz bit shift's para a esquerda do numero 1 */
    vetor[i] = vetor[i] | aux; /* faz ou log. para acender o bit-esimo elem */
  }
  return 1;
}

int * * monta_tab (win)
     win_struc * win;
{
  int * * tab;
  int i, j, cont, tam = (win->hsize*win->wsize+31)/32;
  int bit, * lin_aloc;
  int lin, col, mask;

  tab = (int * *) node_allocation (sizeof (int *) * win->hsize);
  lin_aloc = (int *) node_allocation (sizeof(int)*win->hsize);

  for (i=0; i<win->hsize; i++) {
    lin_aloc[i] = 0;
    tab[i] = NULL;
  }
  
  cont = 0; lin = 0; col = 0;
  /* montando a tabela */
  for (i=0; i<tam; i++) {
    for (bit=0,mask=1; bit<32; bit++) {
      if (win->data[i] & mask) {
	/* tenho o indice e o bit aceso */
	if (! lin_aloc [lin]) {
	  tab[lin] = (int *) node_allocation (sizeof (int) * win->wsize);
	  lin_aloc [lin]++;
	  for (j=0; j<win->wsize; j++) *(tab[lin]+j) = -1;
	}
	*(tab[lin]+col) = cont++;
      }
      mask = mask << 1;
      col++;
      if (col >= win->wsize) {
	col = 0; lin++;
      }
    }
  }
  free (lin_aloc);
  return tab;
}

int fat_out_file (out, n, win, fname)
     struct filter_output * out;
     int n; /* dimensao considerada da matriz na base */
     win_struc * win;
     char * fname;
{
  FILE * fd = fopen (fname, "w");
  int size, num, maior;
  int * vetor, i;
  struct filter_output * aux;
  struct interval * inter;
  int * * tab;

  if (!fd) {
    fprintf (stderr, "fat_out_file() : fopen failed\n");
    return 0;
  }

  /* aloca espaco para o vetor de hexadecimais */
  maior = 0;
  for (aux = out; aux; aux = aux->next) {
    if (aux->dimC > maior)
      maior = aux->dimC;
  }
  maior = (maior * maior + 31) / 32;
  vetor = (int *) node_allocation (sizeof (int) * maior);

  /* Monta a saida */

  tab = monta_tab (win);

  while (out) {
    int cont = 0;
    /* imprime matriz C */
    size = (out->dimC * out->dimC+31)/32;
    C2hex (vetor, out->C, out->dimC);
    fprintf (fd, "MATRIZ C: %d\n", out->dimC);
    for (i=0; i<size; i++) fprintf (fd, "%x ", vetor[i]);
    fprintf (fd, "\n");
    /* imprime a parte da base correspondente */
    for (inter=out->base,num=0; inter; inter=inter->next) num++;
    fprintf (fd, "BASE %d %d\n", win->size, num);
    fprintf (fd, "WINSPEC %d %d %d\n", win->hsize, win->wsize, win->size);
    size = (win->hsize * win->wsize + 31) / 32;
    for (i=0; i<size; i++) fprintf (fd, "%x ", win->data[i]); 
    fprintf (fd, "\n");
    size = (win->size+31)/32;
    for (inter = out->base; inter;inter =inter->next) {
      AB2hex (vetor, inter->A, win, tab);
      for (i=0; i<size; i++) fprintf (fd, "%x ", vetor[i]);
      fprintf (fd, "  ");
      AB2hex (vetor, inter->Bc, win, tab);
      for (i=0; i<size; i++) fprintf (fd, "%x ", vetor[i]);
      fprintf (fd, "\n");
    }
    out = out->next;
  }
  fclose (fd);
  free (vetor);
  return 1;
}
