#include <stdio.h>
#include <malloc/malloc.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "mat_str.h"
#include "equivalence_class.h"

int lvequiclass(fname1, fname2, fname3) 
 char      *fname1 ;
 char      *fname2 ;
 char      *fname3 ;
{
    trv_struc basis1, basis2;
    int       st ,i;
    mat_str     str;    
    equivalence_struct equivalence;
 
    reset_basis(&basis1);
    st = read_itrv(fname1, &basis1) ;
    if(st == 0) {
       fprintf(stderr, "lvequiclass : read_itrv() failed\n") ;
       free_itrv(&basis1) ;
       return(0) ;
    }

/* ------------------------------------------------------------------------- */    
    st = equivalence_class(&basis1, &basis2, &equivalence);
    free_itrv(&basis1);
    if(st == 0){
           fprintf(stderr, "lvequiclass :read_str() failed\n") ;
           return(0) ;
    }

    st = write_itrv(fname2, &basis2);
    free_itrv(&basis2);
 
    if(st == 0) {
       fprintf(stderr, "lvequiclass: write_itrv() failed\n") ;
       return(0) ;
    }

    st = write_equivalence(fname3, &equivalence);
    /*free_equivalence(&equivalence);*/

    if(st == 0) {
       fprintf(stderr, "lvequiclass: write_equivalence() failed\n") ;
       return(0) ;
    }

    return(1) ;

}

int main (int argc, char * * argv) {
    if(!lvequiclass(argv[1], argv[2], argv[3])) {
       fprintf(stderr, "lvequiclass : lvequiclass() failed\n") ;
       return 1 ;
    }
    return 0;
}

