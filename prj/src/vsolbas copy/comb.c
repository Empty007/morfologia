# include <stdio.h>

int numero_de_combinacoes (int m, int n) {
  if (n==0) return 1;
  if (m==n) return 1;
  return numero_de_combinacoes (m-1,n-1) + numero_de_combinacoes (m-1,n);
}

int combinacoes (int m, int n, int * * v) {
  /* v deve ser alocado antes da chamada desta funcao */
  int i, j, tam1, tam2;
  int *a, *b;
  if (n==1) {
    *v = (int *) node_allocation (sizeof(int)*m);
    for (j=0; j<m; j++) (*v)[j] = j;
    return m;
  }

  if (m==n) {
    *v = (int *) node_allocation (sizeof(int)*m);
    for (j=0; j<n; j++) (*v)[j] = j;
    return 1;
  }

  a = b = NULL;
  tam1 = combinacoes (m-1,   n, &a); // 22
  tam2 = combinacoes (m-1, n-1, &b); // 21

  *v = (int *) node_allocation (sizeof(int)*(tam1+tam2)*n);
  for (i=0; i<tam1; i++)
    for (j=0; j<n; j++) (*v)[i*n+j]=a[i*n+j];
  for (i=0; i<tam2; i++) {
    for (j=0; j<n-1; j++) (*v)[(tam1+i)*n+j]=b[i*(n-1)+j];
    (*v)[(tam1+i)*n+(n-1)]=m-1;
  }

  if (a) free (a);
  if (b) free (b);

  return tam1+tam2;
}

int _combinacoes_ (int m, int n, int * * v) {
  static cont = 0;
  int k, i, j;

  if (*v==NULL) {
     *v = (int *) node_allocation (sizeof(int)*n);
     for (i=0; i<n; i++) (*v)[i] = i;
     return ++cont;
  }

  if ((*v)[n-1] < m-1) {
    (*v)[n-1]++;
    return ++cont;
  }

  /* neste caso v[n-1] == m-1 */

  /* verifica se acabou */
  if ((*v)[0] == m-n) {
    free (*v);
    *v = NULL;
    return 0;
  }

  /* neste caso v[n-1] == m-1 e ainda da para continuar */

  for (i=n-2; i>=0; i--) {
    if ((*v)[i]<m-1 && (*v)[i]!=m-n+i) break;
  }
  (*v)[i]++; k=(*v)[i]+1;
  for (i++; i<n; i++) (*v)[i]=k++;
  return ++cont;
}


int main (void) {
  int m, n, *v, c1, c2, i, j, retorno;
  printf ("m = ");
  scanf ("%d", &m);
  printf ("n = ");
  scanf ("%d", &n);
  c1 = numero_de_combinacoes (m,n);
  printf ("comb = %d\n", c1);
  // v = NULL;
  //c2 = combinacoes (m, n, &v);
  //printf ("comb = %d\n", c2);
  // for (i=0; i<c2; i++) {
  //  for (j=0; j<n; j++)
  //    printf ("%2d ", v[i*n+j]);
  //  printf ("\n");
  //}
  //if (v) free (v);

  printf ("Testando ... \n");
  v = NULL; retorno=1;
  for (i=0; retorno; i++) {
    retorno = _combinacoes_ (m, n, &v);
    if (retorno) {
      printf ("%02d) ", retorno);
      for (j=0; j<n; j++) printf ("%2d ", v[j]);
      printf ("\n");
    }
  }
    
  return 0;
}
