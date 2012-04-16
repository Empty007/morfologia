
%{
  # include <stdio.h>
  # include <string.h>
  # include "symrec.h"
  void baseid (char * file);
  void troca (char * isto, char * por_isto, char *nisto);
  int num_linha = 1;
  int num = 0;
  int tam;
  char * sai[1000];
  extern char * yytext;
  char * dil = "~/prj/bin/vdilbas";
  char * ero = "~/prj/bin/verobas";
  char * neg = "~/prj/bin/vnegbas";
  char * sup = "~/prj/bin/vunibas";
  char * inf = "~/prj/bin/vintbas";
  char * dsp = "~/prj/bin/vdspbas";
  char * fct = "~/prj/bin/vfactor";
  char * dfc = "~/prj/bin/vdspfact";
  char * nulo = "";
%}

%union {
  char vetor[80];
  symrec * ptr;
  struct sai {
    char *op;
    char sai[50];
    char ent[50];
    char * str;
  } out;
}

%token <vetor> NUM
%token <ptr>   STR
%token DIL ERO
%right '='
%left SUP INF 
%right '.'
%token NEG DSP FCT DFC

%type  <out>   oper

%%

start : /* empty */ 
      | start line  { num_linha++; }
;

line  : '\n'
      | oper '\n'   { printf ("%s\n", $1.op); 
                      printf ("rm ~/prj/tmp/tmp*\n");
                    }
      | assign '\n'
/*    | error '\n' */
;

assign : STR '=' NUM { char *linha = "~/prj/str/";
                       $1->value = (char *) malloc (strlen(linha)+strlen($3)+1);
                       sprintf ($1->value, "%s%s", linha, $3);
                     }
;

oper  : DIL '_' STR {
                      $$.str = $3->value;
                      sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                      sprintf ($$.ent, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                      baseid ($$.ent);
                      tam = strlen (dil) + strlen ($$.ent) + strlen ($$.sai)
                            + strlen ($$.str) + 5;
                      $$.op = (char *) malloc (tam);
                      sprintf ($$.op, "%s %s %s %s\n", 
                               dil, $$.ent, $$.sai, $$.str); 
                    }
      | ERO '_' STR {
                      $$.str = $3->value;
                      sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                      sprintf ($$.ent, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                      baseid ($$.ent);
                      tam = strlen (ero) + strlen ($$.ent) + strlen ($$.sai)
                            + strlen ($$.str) + 5;
                      $$.op = (char *) malloc (tam);
                      sprintf ($$.op, "%s %s %s %s\n", 
                               ero, $$.ent, $$.sai, $$.str); 
                    }
      | NEG         {
                      $$.str = nulo;
                      sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                      sprintf ($$.ent, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                      baseid ($$.ent);
                      tam = strlen (neg) + strlen ($$.ent) + strlen ($$.sai)
                            + 4;
                      $$.op = (char *) malloc (tam);
                      sprintf ($$.op, "%s %s %s\n", 
                               neg, $$.ent, $$.sai); 
                    }
      | oper SUP oper {
                        $$.str = nulo;
                        sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                        sprintf ($$.ent, "%s", $1.ent);
                        sprintf ($3.ent, "%s", $1.ent);
                        /* trocar $3.ent por $1.ent em $3.op */
                        troca ($3.ent, $1.ent, $3.op);
                        tam = strlen ($1.op) + strlen ($3.op) + strlen (sup)
                              + strlen ($1.sai) + strlen ($3.sai) +
                              strlen ($$.sai) + 7;
                        $$.op = (char *) malloc (tam);
                        sprintf ($$.op, "%s\n%s\n%s %s %s %s\n", 
                                 $1.op, $3.op, sup, $1.sai, 
                                 $3.sai, $$.sai); 
                      }
      | oper INF oper {
                        $$.str = nulo;
                        sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                        sprintf ($$.ent, "%s", $1.ent);
                        sprintf ($3.ent, "%s", $1.ent);
                        /* trocar $3.ent por $1.ent em $3.op */
                        troca ($3.ent, $1.ent, $3.op);
                        tam = strlen ($1.op) + strlen ($3.op) +
                        strlen (inf) + strlen ($1.sai) + strlen ($3.sai) +
                        strlen ($$.sai) + 7;
                        $$.op = (char *) malloc (tam);
                        sprintf ($$.op, "%s\n%s\n%s %s %s %s\n", 
                                 $1.op, $3.op, inf, $1.sai, 
                                 $3.sai, $$.sai); 
                      }
      | oper '.' oper {
                        $$.str = nulo;
                        sprintf ($$.sai, "%s", $1.sai);
                        sprintf ($$.ent, "%s", $3.ent);
                        /* trocar $1.ent por $3.sai em $1.op */
                        troca ($1.ent, $3.sai, $1.op);
                        tam = strlen ($3.op) + strlen ($1.op) + 3;
                        $$.op = (char *) malloc (tam);
                        sprintf ($$.op, "%s\n%s\n", $3.op, $1.op); 
                      }
      | '(' oper ')'  {
                        $$.str = $2.str;
                        sprintf ($$.sai, "%s", $2.sai);
                        sprintf ($$.ent, "%s", $2.ent);
                        tam = strlen ($2.op) + 1;
                        $$.op = (char *) malloc (tam);
                        sprintf ($$.op,  "%s", $2.op);
                      }
     | DSP '.' oper   { 
                        char *linha;
                        sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                        tam = strlen (dsp) + strlen ($3.sai) + strlen ($$.sai) + 4;
                        tam += strlen ($$.sai) + 18;
                        linha = (char *) malloc (tam);
                        sprintf (linha, "%s %s %s 5000\n\ncat %s | less\n", 
                                 dsp, $3.sai, $$.sai, $$.sai);

                        $$.str = $3.str;
                        sprintf ($$.sai, "%s", $3.sai);
                        sprintf ($$.ent, "%s", $3.ent);
                        tam = tam + strlen ($3.op) + 2;
                        $$.op = (char *) malloc (tam);
                        sprintf ($$.op,  "%s\n%s", $3.op, linha);
                      }
     | FCT '.' oper   { 
                        char *linha;
                        sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                        tam = strlen (fct) + strlen ($3.sai) + strlen ($$.sai) + 4;
                        linha = (char *) malloc (tam);
                        sprintf (linha, "%s %s %s\n", fct, $3.sai, $$.sai);

                        $$.str = $3.str;
                        sprintf ($$.sai, "%s", $3.sai);
                        sprintf ($$.ent, "%s", $3.ent);
                        tam = tam + strlen ($3.op) + 2;
                        $$.op = (char *) malloc (tam);
                        sprintf ($$.op,  "%s\n%s", $3.op, linha);
                      }
     | DFC '.' FCT '.' oper   { 
                        char *linha1, *linha2, *buffer;
                        sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                        tam = strlen (fct) + strlen ($5.sai) + strlen ($$.sai) + 4;
                        linha1 = (char *) malloc (tam);
                        sprintf (linha1, "%s %s %s\n", fct, $5.sai, $$.sai);
                           
                        buffer = (char *) malloc (strlen ($$.sai) + 1);
                        sprintf (buffer, "%s", $$.sai);
                        sprintf ($$.sai, "/home/ronaldo/prj/tmp/tmp%03d", ++num);
                        tam = strlen (dfc) + strlen (buffer) + strlen ($$.sai);
                        tam += strlen ($$.sai) + 18;
                        linha2 = (char *) malloc (tam);
                        sprintf (linha2, "%s %s %s \n\ncat %s | less\n", 
                                 dfc, buffer, $$.sai, $$.sai);

                        $$.str = $5.str;
                        sprintf ($$.sai, "%s", $5.sai);
                        sprintf ($$.ent, "%s", $5.ent);
                        tam = strlen(linha1) + strlen (linha2) + strlen ($5.op) + 3;
                        $$.op = (char *) malloc (tam);
                        sprintf ($$.op,  "%s\n%s\n%s", $5.op, linha1, linha2);
                      }
;

%%

int yyerror (char *s) {
  printf ("%s, line: %d lex item: %s\n", s, num_linha, yytext);
}

void baseid (char * file) {
  FILE * arq = fopen (file, "w");
  int i;
  char * linhas [] = {
  "BASE 1 1\n",
  "WINSPEC  5  5  1\n",
  "1000\n",
  "1   0\n",
  "\n",
  0
  };
  if (arq == NULL) {printf ("baseid failed\n"); exit (0);}
  for (i=0; linhas[i] != 0; i++) fprintf (arq, "%s", linhas[i]);
  fclose (arq);
}

void troca (char * isto, char * por_isto, char * nisto) {
  /* supoe-se que isto e por_isto tenham o mesmo comprimento */
  char * ptr = nisto;
  int i;
  while (ptr) {
    ptr = strstr (ptr, isto);
    for (i=0; ptr && por_isto[i] != 0; i++, ptr++) {
      *ptr = por_isto[i];
    }
  }
}