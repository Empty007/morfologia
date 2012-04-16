# include <stdio.h>
# include <string.h>
# include <ctype.h>

char TABLE[5][30] = {
"MM_STRUCT",
"1 3 3 1",
"0 0 0 0 0 0 1 0 0",
0,
};

int main (int argc, char * argv[]) {
  int i, j;
  FILE * arq;
  char str[80], * ptr, nome[80];
  char * file;

  if (argc != 2) {
    fprintf (stderr, "\nUsage: strger <file>\n\n");
    return 0;
  }

  file = argv[1];

  // destrinchar o file
  ptr = file;
  while (!isdigit (*ptr) && *ptr!=0) ptr++;
  if (*ptr==0) {
    fprintf (stderr, "\nUsage: strger <file>\n");
    fprintf (stderr, "<file> must have 9 digits!\n\n");
    return 0;
  }

  for (i=0;  (*ptr=='1' || *ptr=='0') && *ptr!=0; i++,ptr++) str[i]=*ptr;
  str[i]=0;

  if (i!= 9) {
    fprintf (stderr, "\nUsage: strger <file>\n");
    fprintf (stderr, "<file> must have 9 digits '0' or '1'.\n\n");
    return 0;
  }

  arq = fopen (file, "r");
  if (arq!=NULL) {
    // fprintf (stderr, "file %s already exists!\n\n", file);
    fclose (arq);
    return 0;
  }

  for (i=j=0; i<9; i++,j+=2) {
    TABLE[2][j] = str[i];
  }

  arq = fopen (file, "w");
  for (i=0; i<3; i++) {
    fprintf (arq, "%s\n", TABLE[i]);
  }
  fclose (arq);
}
