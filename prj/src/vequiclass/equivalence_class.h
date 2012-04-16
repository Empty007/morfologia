/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 >>>>
 >>>>         File Name: equivalence_class.h
 >>>>
 >>>> Date Last Updated: June 22 1995 
 >>>>
 >>>>          Header for equiv_class.c 
 >>>>
 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

typedef struct {
	int **translate;
} trans_struct;

typedef struct {
	trans_struct *trans ;
	int nroTranslations;
	int tamanho ; // quantidade de 1's
} equivalence_struct ;

int equivalence_class();