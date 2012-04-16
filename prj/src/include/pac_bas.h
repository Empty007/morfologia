/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: pac_bas.h
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Header for pac_bas.c 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define BLKSIZE 500

typedef struct {
        int *A ;
        int *B ;
        int *ind ;
        int size ;
        int total ;
        int nv ;
        int nb;
        win_struc win;
} trv_struc ;

int  read_itrv() ;
int  pertence() ;
int  contido() ;
int  contido2() ;
int  alloc_itrv() ;
int  add_itrv() ;
int  realloc_itrv() ;
void reset_itrv() ;
void free_itrv() ;
int  sel_itrv() ;
int  uni_itrv() ;
int  dimension() ;
int  write_itrv() ;
int  disp_interv() ;
int  pertence2() ;
