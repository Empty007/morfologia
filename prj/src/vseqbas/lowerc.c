# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

extern int mask[NB];
extern int beatriz;

typedef struct base_list {
  trv_struc base;
  struct base_list * prox;
} trv_struc_list;


int main (int argc, char * argv[]) {
  trv_struc base;
  char * fname1, *fname2;
  int lado;
  mat_str   str, str_rot;
  win_struc win_ero;
  trv_struc_list * lower, * apt_list;

  fname1 = argv[1];
  fname2 = argv[2];

  zera_basis_by_ron (&base);
  read_itrv (fname1, &base);
  read_str (fname2, &str);

  beatriz = 1;
  gera_lower_bounds_crescente (&lower, &base, &str);

  lado = 0;
  while (lower) { 
    apt_list = lower;
    lower = lower->prox;
    printf ("Lower Basis %03d:\n", ++lado);
    disp_basis_by_ron (&apt_list->base);
    free_basis_by_ron (&apt_list->base);
    free (apt_list);
  }


  free_basis_by_ron (&base);
  if (str.tag == MM_IMG && str.dat) free (str.dat);
}
