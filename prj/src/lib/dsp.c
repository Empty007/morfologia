# include <malloc/malloc.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>

# include "dsp.h"

struct elem * hex2C (vetor, n)
     int * vetor;
     int n; /* dimensao da matriz C */
{
  int tam = (n*n+31)/32;
  int meio = (n-1)/2;
  struct elem * elem = NULL, * aux;
  int i, j, bit, lin = 0, col = 0;
  
  for (i=0; i<tam; i++) {
    for (j=0,bit=1; j<32; j++) {
      if (vetor[i] & bit) {
	/* tem um bit aceso */
	aux = (struct elem *) node_allocation (sizeof (struct elem));
	aux->row = lin - meio;
	aux->col = col - meio;
	aux->next = elem;
	elem->next = aux;
      }
      bit = bit << 1;
      col++;
      if (col >= n) {
	lin++;
	col = 0;
      }
    }
  }
  return elem;
}

int imp_vet_mat (fd, vetor, dimC)
     FILE * * fd;
     int * vetor, dimC;
{
  int tam = (dimC*dimC+31)/32;
  int i, j, bit, lin = 0, col = 0;
  char * buffer = (char *) node_allocation (sizeof (char)*2*(dimC+1));
  int meio = dimC/2;

  for (i=0; i<tam; i++) {
    for (j=0,bit=1; j<32; j++) {
      if (vetor[i] & bit) {
	/* tem um bit aceso */
	buffer[2*col] = '1';
      }
      else buffer[2*col] = '0';
      buffer[2*col+1] = ' ';
      if (lin == meio && col == meio) {
        buffer[2*col+1] = ']';
        buffer[2*col-1] = '[';
      }
      bit = bit << 1;
      col++;
      if (col >= dimC) {
        buffer[2*dimC] = col = 0;
        if (lin < dimC) fprintf (*fd, "%5s %s\n", "", buffer);
        lin++;
      }
    }
  }
  free (buffer);
  return 1;  
}


int str2vetor (str, n, vetor)
     char * str;
     int n; /* dimensao da matriz */
     int * vetor;
{
  char * ap, * ini = str;
  int tam = (n*n + 31)/32;
  int i = 0;
  ap=str;
  while(*ap) {
    if (!isxdigit(*ap)) {
      if (*ap != 0) {
	*ap = 0; ap++;
      }
      vetor[i++] = (int) strtoul(ini, (char  **)NULL, 16);
      while (!isxdigit(*ap) && *ap) ap++; ini = ap; 
    }
    else ap++;
  }
  /* *ap vale 0; e ainda falta o ultimo inteiro */
  if (ini != ap && isxdigit(*ini)) {
    vetor[i++] = (int) strtoul(ini, (char  **)NULL, 16);
  }
  return 1;
}

int imp_base_mat (fname, out, num)
    char * fname;
    FILE ** out;
    int num;
{
   win_struc     *win;
   trv_struc     base ;
   int           nvars ;
   char          strA[2500] ;
   char          strB[2500] ;
   int           i, j, ind, counter ;
   int           end ;
   int           st;
   int           *B, *winidx ;
   char          back_car, left_car, right_car ;  

   st = set_cars("-", "[", "]", &back_car, &left_car, &right_car) ;
   if(st == 0) {
      return(0) ;
   }  

   st = read_itrv(fname, &base);
   
   if(st == 0) {
      fprintf(stderr, "vdspfact : read_itrv() failed\n") ;
      return(0) ;
   }
   
   st = init_str(strA, 2500, &base.win, back_car, left_car, right_car) ;
   if(st == 0) {
      return(0) ;
   }
   st = init_str(strB, 2500, &base.win, back_car, left_car, right_car) ;
   if(st == 0) {
      return(0) ;
   }
   st = win_index(&base.win, &winidx) ;
   if(st == 0) {
      return(0) ;
   }
   
   B = (int *) node_allocation (sizeof(int)*base.nb) ;
   
   fprintf(* out, "\n\nVISUALIZATION OF BASE\n") ;
   fprintf(* out, "Total of base elements = %d\n", base.total) ;
   fprintf(* out, "\n\n") ;

   end=base.total ;

   counter = 1 ;
   win = &base.win ;

   for(i=0; i<end; i++) {
     ind = base.ind[i] ;
     
     for(j=0; j<base.nb; j++) {
         B[j] = ~base.B[ind+j] ;
      }

     cfg2str(&base.A[ind], base.nv, winidx, win->hsize, win->wsize, strA, 2500) ;
     cfg2str(B           , base.nv, winidx, win->hsize, win->wsize, strB, 2500) ;

     for(j=0; j<win->hsize/2; j++) {
         fprintf(* out, "%5s   %s", "", &strA[2*j*win->wsize]) ;
         fprintf(* out, "%5s   %s\n", "", &strB[2*j*win->wsize]) ;
     }

     fprintf(* out,"%5d)  %s", i+1, &strA[2*(win->hsize/2)*win->wsize]) ;
     fprintf(* out,"%5s   %s\n", "", &strB[2*(win->hsize/2)*win->wsize]) ;

     for(j=win->hsize/2+1; j<win->hsize; j++) {
         fprintf(* out, "%5s   %s", "", &strA[2*j*win->wsize]) ;
         fprintf(* out, "%5s   %s\n", "", &strB[2*j*win->wsize]) ;
     }
     fprintf(* out, "\n") ;
   }

   free_itrv(&base) ;
   free(winidx) ;
   free(B) ;
   return(1);
}

