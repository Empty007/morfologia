/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: pac_xpl.h
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Header for pac_xpl.c
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define  BLOCKSIZE  1000
#define  NEGATIVE   0
#define  POSITIVE   1


typedef struct {
        int   size ;
        int   total ;
        int   nv ;
        int   nb ;
        int   nneg ;
        int   npos ;
        int   *cfg ;
	int   *ind ;
	int   *vl ;
	int   *fx ;
        win_struc win;   
} xpl_struc ;


int  read_xpl() ;
void free_xpl() ;
int  alloc_xpl() ;
int  sep_xpls() ;
int  disp_xpl() ;
int  write_xpl() ;
int  verif_cond() ;
int  reduce2xpl() ;

