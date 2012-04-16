# include <stdio.h>

int main (int argc, char *argv[]) {
  FILE * ent, * sai;     // arquivo de entrada e saida
  int n_col, n_lin, i, j, k_1, k_2;
  int * matrix, * engloba, maior;
  char sig[200];

  if (argc != 3) {
    return 0;
  }

  /* abertura do arquivo */
  if (strcmp(argv[1], "@")) ent = fopen (argv[1], "r");
  else ent = stdin;

  fscanf(ent, "%s\n", sig);
  fclose (ent);
  if (strcmp(sig, "MM_STRUCT") == 0) {
    return 0;
   }

  /* abertura do arquivo */
  if (strcmp(argv[1], "@")) ent = fopen (argv[1], "r");
  else ent = stdin;

  /* leitura da entrada */
  fscanf (ent, "%d", &n_lin);
  fscanf (ent, "%d", &n_col);

  maior = n_lin;
  if (maior<n_col) maior = n_col;
  if (maior%2==0) maior++;

  matrix  = (int *) (node_allocation (sizeof (int) * n_lin * n_col));
  engloba = (int *) (node_allocation (sizeof (int) * maior * maior));

  for (i=0; i<n_lin; i++)
    for (j=0; j<n_col; j++) {
      // fscanf (ent, "%d", &matrix[j*n_lin + i]); // Lendo como transposta
      fscanf (ent, "%d", &matrix[i*n_col + j]); 
    }

  fclose (ent);

  for (i=0; i<maior; i++) 
    for (j=0; j<maior; j++) engloba[i*maior+j] = 0;

  k_1 = (maior-1)/2 - (n_lin-1)/2;
  k_2 = (maior-1)/2 - (n_col-1)/2;
  for (i=0; i<n_lin; i++)
    for (j=0; j<n_col; j++) {
      engloba[(i+k_1)*maior+(j+k_2)] = matrix[i*n_col + j]; 
    }

  /* abertura do arquivo */
  if (strcmp(argv[2], "@")) sai = fopen (argv[2], "w");
  else sai = stdout;

  fprintf (sai, "MM_STRUCT\n"); /* indica elemento struturante */

  if (n_lin == 3 && n_col == 3) {
    fprintf (sai, "1 3 3 1\n");
    for (i=0; i<3; i++)
      for (j=0; j<3; j++)
        fprintf (sai, "%d ", (matrix[i*3+j] ? 1 : 0));
  }
  else {
    fprintf (sai, "0 %d %d 1\n", maior, maior);
    for (i=0; i<maior*maior; i++) fprintf (sai, "%d ", (engloba[i]?1:0));
  }

  free (matrix);
  free (engloba);
  fclose (sai);
  return 0;
}

/*  teste.c
int main (void) {
  int size, m, n, points, i, j, k;
  int * X_ker, * Y_ker;

  printf ("m = "); scanf ("%d", &m);
  printf ("n = "); scanf ("%d", &n);
  size = m * n;

  points = 0;
  
  for (i=0; i<size; i++) {
    scanf ("%d", &k);
    if (i%n == 0) printf ("\n");
    printf ("(%2d, %2d) : %2d ", i%n - (n-1)/2, (m-1)/2 - i/n, k);
    if (k) points++;
  }
  printf ("\n");
  return 1;
}
*/
