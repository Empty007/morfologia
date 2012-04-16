# include <stdio.h>

int main (void) {
  int size, m, n, points, i, j, k;
  int * X_ker, * Y_ker;

  printf ("m = "); scanf ("%d", &m);
  printf ("n = "); scanf ("%d", &n);
  size = m * n;

  points = 0;
  
  for (i=0; i<size; i++) {
    // printf ("(%d, %d) = (%d, %d)\n", i/n, i%n, X_ker[points-1], Y_ker[points-1]);
    scanf ("%d", &k);
    if (i%n == 0) printf ("\n");
    // printf ("(%2d, %2d) : %2d ", i%n - (n-1)/2, (m-1)/2 - i/m, k);
    printf ("(%2d, %2d) : %2d ", i%n - (n-1)/2, (m-1)/2 - i/n, k);
    if (k) points++;
  }
  printf ("\n");

  /*
  printf ("\n");
  points = 0;
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++) {
      printf ("(%2d, %2d) ", i-1, 1-j);
    }
    printf ("\n");
  }
  */
  
  return 1;
}

