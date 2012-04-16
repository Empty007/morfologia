/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: pac_win.h
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Header to file pac_win.c 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


#define SQUARESIZE    5 
#define MAXWINSIZE    16

typedef struct {      /* window specification data structure */
        int  *data ;
        int  hsize ;
        int  wsize ;
        int  size ;
} win_struc ;


int   read_win() ;
int   read_win2();
void  free_win() ;
void  disp_win() ;
int   write_win() ;
void  write_win2();
int   copy_win();
int   cfg2str() ;
int   win_index() ;
