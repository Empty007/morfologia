# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

extern int mask[NB];

int main (int argc, char * argv[]) {
  trv_struc base, upper;
  char * fname1, *fname2;
  int lado;
  mat_str   str, str_rot;
  win_struc win_ero;

  fname1 = argv[1];
  fname2 = argv[2];

  zera_basis_by_ron (&base);
  read_itrv (fname1, &base);
  read_str (fname2, &str);

  zera_basis_by_ron (&upper);
  upper_bound_by_ron (&upper, &base, &str);
  lado = menor_lado_possivel_da_janela (&(upper.win));
  transform_to_small_window (&(upper.win), lado);
  disp_basis_by_ron (&upper);

  /* transpondo o elemento struturante */
  str_rot.dat = NULL;
  lvrotstr (&str, 4, 0, &str_rot);

  win_ero.size = win_ero.hsize = win_ero.wsize = 0;

  /* construindo a janela $W=W'\ominus C^t$ */
  erode_win (&(base.win), &win_ero, &str_rot);

  diminui_desmonta (&upper, &win_ero);
  disp_basis_by_ron (&upper);

  free_basis_by_ron (&upper);
  free_basis_by_ron (&base);
  if (str.tag == MM_IMG && str.dat) free (str.dat);
  if (str_rot.tag == MM_IMG && str_rot.dat) free (str_rot.dat);
}
