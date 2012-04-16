/*   symrec.h                                          */
/*   Time-stamp: <98/09/24 10:37:07 ronaldo>           */

/* Data type for links in the chain of symbols.        */
struct symrec {
  char *name;    /* name of symbol                     */
  int type;      /* type of symbol: either VAR or FNCT */
  char * value;  /* value of symbol                    */
  struct symrec *next;      /* link field              */
};

typedef struct symrec symrec;

/* The symbol table: a chain of `struct symrec'.     */
symrec *putsym (char *sym, int type);
symrec *getsym (char *sym);
void   create_sym_table (unsigned size);
void   print_sym_table(void);
