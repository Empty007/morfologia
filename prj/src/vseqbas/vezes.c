int tres_vezes () {
  int i,j,k,n,cont;
  cont=i=j=k=0;n=5;
  while (i<n) {
    cont++;
    printf ("%d %d %d\n", i,j,k);
    k++;
    if (k==n) {k=0;j++;}
    if (j==n) {j=0;i++;}
  }
  printf ("cont = %d\n", cont);
}

int duas_vezes () {
  int i,j,n,cont;
  cont=i=j=0;n=10;
  while (i<n) {
    cont++;
    printf ("%d %d\n", i,j++);
    if (j==n) {j=0;i++;}
  }
  printf ("cont = %d\n", cont);
}

int vezes (int num[], int m) {
  /* num nao tem valor zero */ 
  int * i,j,cont;

  i = (int *) malloc (sizeof(int) * m);

  for (j=0; j<m; j++) i[j]=0;
  cont=0;
  while (i[0]<num[0]) {
    cont++;
    for (j=0; j<m; j++) printf ("%d ", i[j]);
    i[m-1]++;
    printf ("\n");
    for (j=m-1; j>0; j--) {
      if (i[j]==num[j]) {i[j]=0;i[j-1]++;}
    }
  }
  printf ("cont = %d\n", cont);
  free (i);
}

int main () {
  int num[10], i;
  for (i=0;i<9;i++) num[i] = 3;
  vezes (num, 5);
}
