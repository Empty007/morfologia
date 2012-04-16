#include <stdio.h>
#include <malloc/malloc.h>
#include "pac_win.h"
#include "pac_bas.h"
#include "desc_win.h"
#include "desc_bas.h"
#include "intersection_basis.h"

int lvintbas(fname1, fname2, fname3)
 char      *fname1 ;
 char      *fname2 ;
 char      *fname3 ;
{
    trv_struc basis1, basis2 ,basis3 ;
    int       st ;



    reset_basis(&basis1);
    st = read_itrv(fname1, &basis1) ;
    if(st == 0) {
       fprintf(stderr, "lvintbas : read_itrv() failed\n") ;
       free_itrv(&basis1) ;
       return(0) ;
    }


    reset_basis(&basis2);
    st = read_itrv(fname2, &basis2) ;
    if(st == 0) {
       fprintf(stderr, "lvintbas : read_itrv() failed\n") ;
       free_itrv(&basis1) ;
       free_itrv(&basis2) ;
       return(0) ;
    }

    reset_basis(&basis3);

/* ------------------------------------------------------------------------- */
 
    st = supremum_win(&(basis1.win), &(basis2.win),&(basis3.win)) ;
    if(st == 0) {
       fprintf(stderr, "lvintbas : sup_win() failed\n") ;
       free_itrv(&basis1) ;
       free_itrv(&basis2) ;
       return(0) ;
    }

    if(basis1.win.size != basis3.win.size){
            st = transfer_basis_according_window(&basis1,&(basis3.win));
            if(st == 0) {
                  fprintf(stderr, "lvintbas : transf_itrv() failed\n") ;
                  free_itrv(&basis1) ;
                  free_itrv(&basis2) ;
                  free_itrv(&basis3) ;                 
                  return(0) ;
            }
    }

    if(basis2.win.size != basis3.win.size){
            st = transfer_basis_according_window(&basis2,&(basis3.win));
            if(st == 0) {
                  fprintf(stderr, "lvintbas : transf_itrv_itrv() failed\n") ;
                  free_itrv(&basis1) ;
                  free_itrv(&basis2) ;
                  free_itrv(&basis3);
                  return(0) ;
            }
    }

    st = intersection_basis(&basis1,&basis2,&basis3);

    if(st == 0){
           fprintf(stderr, "lvintbas : intersection_basis() failed\n") ;
           free_itrv(&basis1) ;
           free_itrv(&basis2) ;
           free_itrv(&basis3) ;
           return(0) ;
            

    }
     
    st = write_itrv(fname3, &basis3);

    free_itrv(&basis1) ;
    free_itrv(&basis2) ;
    free_itrv(&basis3) ;
    if(st == 0) {
       fprintf(stderr, "lvintbas: write_itrv() failed\n") ;
       return(0) ;
    }

    return(1) ;

}

int main (int argc, char * *  argv) {
    if(!lvintbas(argv[1], argv[2], argv[3])) {
       fprintf(stderr, "vlinter : lvintbas() failed\n") ;
       return 1 ;
    }
    return 0;
}
