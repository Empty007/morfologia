/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: pac_cfg.h
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Header for pac_cfg.c
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define  EQUAL   0
#define  LESS    -1
#define  GREAT   1

typedef struct Cfg_node {
        int *data ;
        int fq0 ;
        int fq1 ;
        int bal ;
        struct Cfg_node *left ;
        struct Cfg_node *right ;
} cfg_node ;

typedef struct {
        int   total ;
        int   sumtotal ;
        int   nv ;
        int   cfgsize ;
        cfg_node  *root ;
        win_struc win;
} cfg_struc ;


void     leftrotate() ;
void     rightrotate() ;
cfg_node *alloc_cfg_node() ;
void     set_cfg_node() ;
int      compare_cfg_node() ;
int      insert_cfg() ;
int      read_cfg() ;

void     free_cfg_node() ;
void     free_cfg_tree() ;
void     free_cfg() ;
void     reset_cfg() ;

void     print_cfg_tree() ;
int      write_cfg() ;

int      merge_cfg() ;

int      count_conflicts() ;
void     disp_cfg_node() ;
void     dsp_cfg_tree1() ;
void     dsp_cfg_tree2() ;

int      count_pat() ;
