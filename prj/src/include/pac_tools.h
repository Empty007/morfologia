/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: pac_tools.h
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Header for pac_tools.c
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

typedef struct {
	int e_flag ;                /* emit or not emit report */
	int rep_id ;                /* the current report number */
        char alg_str[6] ;           /* current algorithm */
                                    /* ISI-0, ISI-1, ISi-2, ISI-3 */
        char xpl_fname[100] ;       /* name of example's file */
        int xpl_total ;                /* total of examples */
        int xpl_neg ;                  /* total of negative examples */
        int xpl_pos ;                  /* total of positive examples */
	int overwrite ;                /* overwrite to prior report file ? */
	int max_trv ;               /* maximum number of intervals */
	int maxmem ;                /* maximum of memory occuied */
	int next_stop ;             /* next instant to emit report */
	int step ;                  /* the step of report emission */
	char rep_fname[100] ;           /* the name of report file */
        char bas_fname[100] ;        /* the name of base file */
} rep_struc ;

int  init_report() ;
int  emit_report() ;
int  emit_final_report() ;
void calc_report() ;
int  new_name() ;
int  init_str() ;
