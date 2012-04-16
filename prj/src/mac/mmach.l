%{
# include <string.h>
# include "symrec.h"
# include "bison.h"
%}

%%

[0-1]{9} {
  strcpy (yylval.vetor, yytext);
  strcat (yylval.vetor, ".str");
  return NUM;
}

[a-zA-Z][a-zA-Z0-9]* {
  symrec * s;
  s = getsym (yytext);
  if (s == 0) {
    s = putsym (yytext, STR);
  }
  yylval.ptr = s;
  return s->type;
}

[ \t]* {
  ;
}

#.*\n {
 ;
}

.|\n {
  return *yytext;
}

%%