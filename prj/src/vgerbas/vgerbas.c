# include <stdio.h>
# include "pac_def.h"
# include "pac_win.h"
# include "pac_xpl.h"
# include "pac_bas.h"

/* 
 * typedef struct {
 *         int *A ;        lado esquerdo: (int *) malloc (sizeof(int) * ninterv * n4bytes)
 *         int *B ;        lado direito : (int *) malloc (sizeof(int) * ninterv * n4bytes)
 *         int *ind ;      ind. de cada intervalo:  (int *) malloc (sizeof(int) * ninterv)
 *         int size ;      no. de intervalos
 *         int total ;     no. de intervalos
 *         int nv ;        no. de variaveis
 *         int nb;         = n4bytes = (nvars+31)/32;
 *         win_struc win;  especificacao da janela
 * } trv_struc ;
 */

extern int mask[32] ;

int read_ron_win2 (FILE * * fd,  win_struc * win) {
    char     header[WIN_HEAD_LEN+1] ;
    int      dt ;
    int      *data, n4bytes ;
    int      m, n, mn, size ;
    int      i, j, st;


   /* check file header */
    if(fread(header, 1, WIN_HEAD_LEN, *fd) < WIN_HEAD_LEN) {
       fprintf(stderr, "read_ron_win2() : unrecognized file format\n") ;
       return(0) ;
    }
    header[WIN_HEAD_LEN] = '\0' ;

    if(strcmp(header, WIN_HEAD) == 0) {

       /* read header datas */
       fscanf(*fd, "%d", &m);
       fscanf(*fd, "%d", &n);
       fscanf(*fd, "%d", &size);

       mn = m * n ;
   
       n4bytes = (mn+NB1)/NB ;

       data = (int *)malloc(n4bytes*sizeof(int)) ;
       if(data == NULL) {
          fprintf(stderr, "read_ron_win2() : Memory allocation failed\n") ;
          return(0) ;
       }

      /* data reading loop */
       for(j=0; j<n4bytes; j++) {
          st = fscanf(*fd, "%x", &dt) ;
          if(st == EOF) {
             fprintf(stderr, "read_ron_win2() : file data error\n") ;
             free(data) ;
             return(0) ;
          }
          data[j] = dt ;
       }

       win->data = data ;
       win->hsize = m ;
       win->wsize = n ;
       win->size = size ;
       return(1) ;
    }

    fprintf(stderr, "read_ron_win2() : unrecognized file format\n") ;
    return(0) ;         
}

int nega_str (char *str) {
  while (*str != 0) {
    if (*str == '0') 
      *str = '1';
    else {
      if (*str == '1') *str = '0';
    }
    str++;
  }
  return 1;
}

int str2cfg(int *cfg, int nvars, int * winidx, int m, int n, char * str, int strsize) {
/*  int  *cfg: cfg[w]  where w=number of variables */
/*  int  nvars: no. de variaveis */
/*  int  *winidx:  winidx[w] */
/*  int  m, n: m and n are the height and the width of the 
               rectangle corresponding to window domain */
/*  char *str ;  str[strsize] */

    int i ;
    int block, nshifts ;
    int n4bytes, compr;

    n4bytes = (nvars+31)/32;

    // inicializar cfg
    for (i=0; i<n4bytes; i++) cfg[i] = 0;

    for(i=0; i<nvars; i++) {
       nshifts = i%NB;
       block = i/NB;
       if (str[2*winidx[i]] == '1') {
           cfg[block] |= mask[nshifts];
       }
    }
    return 1;
}


int read_ron_basis (char * fname,  trv_struc * interv) {
/* transforma ron_basis para nina_basis */
/* 29/09/1999 */

    int          nvars, ninterv ;
    FILE         *fd ;
    char         header[8] ;
    int          n4bytes, data, i, j, b, st ;
    char         strA[2500], strB[2500];
    char         back_car, left_car, right_car;
    int          *B, *winidx, ind, nada;
    win_struc    *win;
          
   /* open file ----------------------------------------------------------- */
    fd = fopen (fname, "r");
    if(fd == NULL) {
       fprintf(stderr, "read_ron_basis() : file open error\n") ;
       return 0;
    }

   /* check file header --------------------------------------------------- */
    fscanf (fd, "%s", header) ;
    if(strncmp(header, "BASE", 6) != 0) {
       fprintf(stderr, "read_ron_basis() : unrecognized file format\n") ;
       fclose (fd);
       return 0;
    }

   /* input: number of variables and number of examples ------------------- */
    fscanf(fd, "%d %d\n", &nvars, &ninterv) ;

    n4bytes = (nvars+31)/32 ;

    st = alloc_itrv (interv, nvars, ninterv);
    if (st == 0) {
       fprintf(stderr, "read_ron_basis(): alloc_interv() failed\n") ;
       fclose(fd) ;
       return(0) ;
    }

    st = read_ron_win2 (&fd, &interv->win);

    if(st == 0){
       fprintf(stderr, "read_ron_basis(): read_win2() failed\n") ;
       return(0) ;
    }

   back_car = '-'; left_car = right_car = ' ';
   st = init_str (strA, 2500, &(interv->win), back_car, left_car, right_car);
   if(st == 0) {
       fprintf(stderr, "read_ron_basis(): init_str() failed\n") ;
       return(0) ;
   }
   st = init_str (strB, 2500, &(interv->win), back_car, left_car, right_car);
   if(st == 0) {
       fprintf(stderr, "read_ron_basis(): init_str() failed\n") ;
       return(0) ;
   }

   st = win_index (&(interv->win), &winidx) ;

   if(st == 0) {
      fprintf(stderr, "read_ron_basis(): win_index() failed\n") ;
      return(0) ;
   }

   win = &(interv->win);

   B = (int *) malloc (sizeof(int)*interv->nb);
   if(B==NULL) {
      fprintf(stderr, "read_ron_basis(): memory allocation error\n") ;
      return(0) ;
   }

   interv->total = ninterv;

   /* reading process ---------------------------------------------------- */

   for(i=0,b=0; i<ninterv; i++, b+=n4bytes) {

      ind = interv->ind[i] = b;

      fscanf (fd, " %d", &nada); /* no. do intervalo */
      // printf ("intervalo: %d\n", nada);

      for(j=0; j<win->hsize/2; j++) {
         fscanf (fd, " %[^\t]", &strA[2*j*win->wsize]) ;
         fscanf (fd, " %[^\t]", &strB[2*j*win->wsize]) ;
         // printf ("%s = %s\n", &strA[2*j*win->wsize], &strB[2*j*win->wsize]);
      }

      fscanf (fd, " %[^\t]", &strA[2*(win->hsize/2)*win->wsize]) ;
      fscanf (fd, " %[^\t]", &strB[2*(win->hsize/2)*win->wsize]) ;
      // printf ("%s + %s\n", &strA[2*(win->hsize/2)*win->wsize], &strB[2*(win->hsize/2)*win->wsize]);
      for(j=win->hsize/2+1; j<win->hsize; j++) {
         fscanf (fd, " %[^\t]", &strA[2*j*win->wsize]) ;
         fscanf (fd, " %[^\t]", &strB[2*j*win->wsize]) ;
         // printf ("%s : %s\n", &strA[2*j*win->wsize], &strB[2*j*win->wsize]);
      }
  
      str2cfg (&(interv->A)[ind], nvars, winidx, win->hsize, win->wsize, strA, 2500);
      nega_str (strB);
      str2cfg (B, nvars, winidx, win->hsize, win->wsize, strB, 2500);
      // for(j=0; j<nvars; j++) interv->B[ind+j]=~B[j];

   }

   fclose(fd) ;  
   free (winidx);
   free(B) ;
   return 1;         
}




int main (int argc, char * * argv) {
  trv_struc interv;
  if( !read_ron_basis (argv[1], &interv)) {
    fprintf(stderr, "vbasger : read_ron_basis() failed\n") ;
    return 1;
  } 
  write_itrv (argv[2], &interv);
  return 0;
}
