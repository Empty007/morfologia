#include <stdio.h>
#include <malloc/malloc.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "mat_str.h"
#include "vec_str.h"

int lverobas(fname1, fname2, fname3)
 char      *fname1 ;
 char      *fname2 ;
 char      *fname3 ;
{
    trv_struc basis1, basis2 ;
    int       st ,i;
    mat_str     str;

    reset_basis(&basis1);
    st = read_itrv(fname1, &basis1) ;
    if(st == 0) {
       fprintf(stderr, "lverobas : read_itrv() failed\n") ;
       free_itrv(&basis1) ;
       return(0) ;
    }
 
    reset_basis(&basis2);

/* ------------------------------------------------------------------------- */

    st = read_str(fname2, &str);
    if(st == 0){
           fprintf(stderr, "lverobas :read_str() failed\n") ;
           free_itrv(&basis1) ;
           return(0) ;
    }
    st = erosion_basis(&basis1,&str, &basis2);
    free_itrv(&basis1);
    free_str(&str);
    if(st == 0){
           fprintf(stderr, "lverobas :read_str() failed\n") ;
           return(0) ;
    }

    st = write_itrv(fname3, &basis2);
    free_itrv(&basis2) ;
 
    if(st == 0) {
       fprintf(stderr, "lverobas: write_itrv() failed\n") ;
       return(0) ;
    }

    return(1) ;

}


int main (int argc, char * * argv) 
{
    if(!lverobas(argv[1], argv[3], argv[2])) {
       fprintf(stderr, "lverobas : lverobas() failed\n") ;
       return 1 ;
    }
    return 0;
}


