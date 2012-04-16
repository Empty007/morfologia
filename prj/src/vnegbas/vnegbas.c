#include <stdio.h>
#include <malloc/malloc.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "negation_basis.h"

int lvnegbas(fname1, fname2)
 char      *fname1 ;
 char      *fname2 ;
{
    trv_struc basis1, basis2 ;
    int       st ;
 
    reset_basis(&basis1);
    st = read_itrv(fname1, &basis1) ;
    if(st == 0) {
       fprintf(stderr, "lvneg : read_itrv() failed\n") ;
       free_itrv(&basis1) ;
       return(0) ;
    }

    reset_basis(&basis2);

/* ------------------------------------------------------------------------- */
    st = copy_win(&(basis1.win), &(basis2.win)) ;
    if(st == 0) {
       fprintf(stderr, "lneg : copy_win() failed\n") ;
       free_itrv(&basis1);  
       return(0) ;
   }

    st = negation_basis(&basis1,&basis2);

    if(st == 0){
           fprintf(stderr, "lvneg : negation() failed\n") ;
           free_itrv(&basis1) ;
           free_itrv(&basis2) ;
           return(0) ;
    }
    st = write_itrv(fname2, &basis2);
    free_itrv(&basis1) ;
    free_itrv(&basis2) ;
 
    if(st == 0) {
       fprintf(stderr, "lvneg: write_itrv() failed\n") ;
       return(0) ;
    }

    return(1) ;

}

int main (int argc, char * * argv) {
    if(!lvnegbas(argv[1], argv[2])) {
       fprintf(stderr, "vlneg3 : lvnegbas() failed\n") ;
       return 1;
    }
    return 0;
}




