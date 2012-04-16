# include <stdio.h>
# include <malloc/malloc.h>
# include "pac_win.h"
# include "pac_bas.h"
# include "pac_def.h"
# include "mat_str.h"

int main (int argc, char * argv[]) {
  trv_struc base, dual;
  char * fname1, *fname2;

  fname1 = argv[1];
  fname2 = argv[2];

  zera_basis_by_ron (&base);
  read_itrv (fname1, &base);
  zera_basis_by_ron (&dual);

  basis_dual_operator (&base, &dual);

  // disp_basis_by_ron (&dual);
  write_itrv (fname2, &dual);

  free_basis_by_ron (&dual);
  free_basis_by_ron (&base);
}
