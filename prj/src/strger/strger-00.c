# include <stdio.h>
# include <string.h>

char TABLE[5][30] = {
"MM_STRUCT",
"1 3 3 1",
"0 0 0 0 0 0 1 0 0",
0,
};

int main (int argc, char * argv[]) {
  int i, j;
  FILE * arq;
  char nome[80];

  if (argc != 3) {
    fprintf (stderr, "\nUsage: strger <string> <directory>\n\n");
    return 0;
  }

  if (strlen (argv[1]) != 9) {
    fprintf (stderr, "\nUsage: strger <string> <directory>\n");
    fprintf (stderr, "<string> must have 9 caracteres!\n\n");
    return 0;
  }

  strcpy (nome, argv[2]);
  if ( nome[strlen(nome)-1] != '/') strcat (nome, "/");
  strcat (nome, argv[1]);
  strcat (nome, ".str");
  arq = fopen (nome, "r");
  if (arq!=NULL) {
    // fprintf (stderr, "file %s already exists!\n\n", nome);
    fclose (arq);
    return 0;
  }

  for (i=j=0; i<9; i++,j+=2) {
    TABLE[2][j] = (argv[1][i] != '0') + '0';
  }

  arq = fopen (nome, "w");
  for (i=0; i<3; i++) {
    fprintf (arq, "%s\n", TABLE[i]);
  }
  fclose (arq);
}
