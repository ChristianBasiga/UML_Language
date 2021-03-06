%{

#include "y.tab.h"
#include <string.h>



/*To do, assign from yytext to yylval, need to create the union*/

void assignToString(){

	
	yylval.string = (char*)malloc(strlen(yytext)+1);
	strcpy(yylval.string,yytext);

}

void assignToChar(){

	yylval.character = yytext[0];
}

void assignToSymbol(){


	strcpy(yylval.symbol, yytext);

}

%}

%%
func		{return FUNC;}
uml			{return UML;}
class|interface		{assignToString(); return STRUCTURE;}
char|void|int|float|string	{assignToString(); return DATA_TYPE;}
abstract|static		{assignToString(); return META_TYPE;}
create|delete|print		{assignToString(); return COMMAND;}
all|parents|children	{assignToString(); return PRINT_SPECIFICATION;}
[a-zA-Z]+[a-zA-Z0-9]*	{assignToString(); return IDENTIFIER;}
[\+\-\*]			{assignToChar(); return ACCESS;}
\^|>			{assignToChar(); return RELATION;}
[ \t]	;
\(			{return LEFTP;}
\)			{return RIGHTP;}
%%

