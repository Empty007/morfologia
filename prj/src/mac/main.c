#include "symrec.h"
#include "bison.h"

struct init {
  char * name;
  int  type;
} reserve [] = {
  "dil", DIL,
  "ero", ERO,
  "sup", SUP,
  "inf", INF,
  "neg", NEG,
  "dsp", DSP,
  "fct", FCT,
  "dspfct", DFC,
  0,   0
};

void init_table (void) {
  int i;
  for (i=0; reserve[i].name != 0; i++) {
    putsym (reserve[i].name, reserve[i].type);
  }
}

void main (void) {
  create_sym_table (511);
  init_table ();
  yyparse ();
}
