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
	int *nrovars; // quantidade de 1's
	int *nroTranslations;
	int size;
} equivalence_struct;

int equivalence_class();