# include <stdio.h>
# include <stdlib.h>
# include "pac_def.h"
# include "pac_win.h"
# include "pac_bas.h"

int lvdstbas (char * fname1, char * prefix) {
   win_struc     *win;
   trv_struc     base;
   int           i, j, ind;
   int           st, n4bytes;
   FILE          * fd;
   char          fname2[2500];

   if((st = read_itrv (fname1, &base)) == 0) {
      fprintf(stderr, "lvdstbas : read_itrv() base failed\n") ;
      return 0;
   }

   n4bytes = (base.nv+31)/32 ;
   for (i=0; i<base.total; i++) {
     sprintf (fname2, "%s-%03d.txt", prefix, i);
     fd = fopen (fname2, "w");
     fprintf(fd, "BASE %d %d\n", base.nv, 1) ;
     write_win2(fd, &base.win);
     for (j=0; j<n4bytes; j++) fprintf(fd, "%x ", base.A[base.ind[i]+j]); 
     fprintf(fd, "  ") ;
     for (j=0; j<n4bytes; j++) fprintf(fd, "%x ", base.B[base.ind[i]+j]); 
     fprintf(fd, "\n") ;
     fclose (fd);
   }

   if (base.total > 1) {
     fprintf (stderr, "%d basis were generated with prefix %s-\n", 
              base.total, prefix);
   }
   else {
     fprintf (stderr, "%d base was generated with prefix %s-\n", 
              base.total, prefix);
   }
   free_itrv (&base);
   return 1;
}

int main (int argc, char * * argv) {
  if (!lvdstbas(argv[1], argv[2])) {
       fprintf(stderr, "vdstbas : lvdstbas() failed\n") ;
       return 1;
  }
  return 0;
}
