# include <malloc/malloc.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

struct elem {
  int row;
  int col;
  struct elem * next;
};

struct interval {
  struct elem * A;
  struct elem * Bc;
  struct interval * next;
};

struct coll_interval {
  int sizeBc;
  struct interval * inter;
  struct coll_interval * next;
};

struct output {
  struct elem * A;
  struct elem * Bc;
  struct elem * C;
  int dimC;
  struct output * next;
};

struct filter_output {
  struct elem * C;
  struct interval * base;
  int dimC;
  struct filter_output * next;
};

void * node_allocation ();
int igual ();
struct filter_output * transforma ();
void inverte_list ();
void print_matrix ();
void monta_matriz ();
struct coll_interval * * criate_buckets ();
struct interval * create_interval ();
struct coll_interval * search_subcoll_bucket ();
struct coll_interval * ins_subcoll_bucket ();
void ins_interval ();
struct elem * assembly_matrix ();
win_struc * transf_intervals ();
int diff ();
void ins_output_AB ();
void ins_output_C ();
int verify ();
int modulo ();
int dimension_matrix ();
void sort_matrix ();
int dist_origem ();
int choose_ABc ();
void fatora ();
void make_all ();
void print_filter_output ();
int C2hex ();
int AB2hex ();
int * * monta_tab ();
int fat_out_file ();
int nova_origem ();
int translada ();
int dist_pontos ();
int centro_massa ();
int igual_translacao ();

