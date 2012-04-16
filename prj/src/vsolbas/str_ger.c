# include <stdio.h>

int main (int argc, char *argv[]) {
  FILE * ent, * sai;     // arquivo de entrada e saida
  int n_col, n_lin, i, j;
  int * matrix;

  if (argc != 3) {
    return 0;
  }

  /* abertura do arquivo */
  if (strcmp(argv[1], "@")) ent = fopen (argv[1], "r");
  else ent = stdin;

  /* leitura da entrada */
  fscanf (ent, "%d", &n_lin);
  fscanf (ent, "%d", &n_col);

  matrix = (int *) node_allocation (sizeof (int) * n_lin * n_col);

  for (i=0; i<n_lin; i++)
    for (j=0; j<n_col; j++)
      fscanf (ent, "%d", &matrix[i*n_col + j]);

  fclose (ent);

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
    fprintf (sai, "0 %d %d 1\n", n_lin, n_col);
    for (i=0; i<n_lin*n_col; i++) fprintf (sai, "%d ", (matrix[i]?1:0));
  }

  fprintf (sai, "\n");

  fclose (sai);
  return 0;
}
