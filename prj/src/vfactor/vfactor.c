#include "pac_win.h"
#include "pac_bas.h"
#include "fat.h"

int lvfactor(pt_coll, out, size_coll, n) 
  struct coll_interval * * pt_coll;
  struct filter_output * * out;
  int size_coll, n;
{
  struct output * out_put = NULL;

  make_all (pt_coll, size_coll, & out_put, n);
  * out = transforma (out_put);
  if (* out == NULL) {
    fprintf (stderr, "vfactor () : failed\n");
    return 0;
  }
  return 1;
}


int main (int argc, char * * argv) {
  struct coll_interval * * pt_coll;
  struct filter_output * out;
  win_struc * win;
  int size_coll, n;

  win = transf_intervals (&pt_coll, argv[1], & size_coll, &n);
  if (win == NULL) {
    fprintf (stderr, "vfactor : vfactor () failed\n");
    return 1;
  }
  
  if(!lvfactor(pt_coll, & out, size_coll, n)) {
       fprintf(stderr, "vfactor : lvfactor() failed\n") ;
       return 1 ;
    }
    
  if (!fat_out_file (out, n, win, argv[2])) {
    fprintf (stderr, "vfactor() : failed\n");
    return 1;
  }
  return 0;
}






