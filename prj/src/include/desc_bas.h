/* desc_bas.h : include file for programs related to basis specification */


/* interval structure */

struct interval;

struct interval{  
        int  *A ;
        int  *B_c ;
        struct interval *next;
};


typedef struct interval itr_struc;

int transfer_basis_according_window();
int copy_basis();
void free_vect_basis();
void reset_basis();
void function1();
void maximal_list();
int list_interv_to_basis();

void free_list_interval();
void free_interval();
int alloc_interval();
