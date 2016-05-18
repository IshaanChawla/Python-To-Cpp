%{
	#include<stdio.h>
	#include<stdlib.h>
	#include"y.tab.h"
	char* makeString(char* yytext)
	{
		char *p = (char*)malloc((sizeof(yytext)+1)*sizeof(char));
		strcpy(p,yytext);
		return p;
	}
%}

IDEN [a-zA-Z_][a-zA-Z0-9_]*
STRING (\".*\")|(\'.*\')
NUM [0-9]*
REAL_NO [0-9]+.[0-9]+

%%
<<EOF>> {
	return E_O_F;
}
"self" {
	yylval.text = makeString(yytext);
	return SELF;
}
"def" {}
"if" {
	yylval.text = makeString(yytext);
	return IF;
}

"__name__" {
	yylval.text = makeString(yytext);
	return NAME;
}
"\"__main__\"" | 
"'__main__'" {
	yylval.text = makeString(yytext);
	return MAIN;
}
"else" {}
"elif" {}
"while" {}
"print" {}
"raw_input" {}
"for" {}
"class" {
	yylval.text = makeString(yytext);
	return CLASS;
}
"return" {
	yylval.text = makeString(yytext);
	return RETURN;
}
{IDEN} {
	yylval.text = makeString(yytext);
	return IDEN;
}
{STRING} {
	if(yyleng == 3)
		yylval.var_type = -4;
	else
		yylval.var_type = -3;	
	return STRING;
}
{NUM} {
	yylval.var_type = -2;
	return NUM;
}
{REAL_NO} {
	yylval.var_type = -1;
	return REAL_NO;
}
[=] {return '=';}
[,] {return ',';}
[(] {return '(';}
[)] {return ')';}
[\n] {return '\n';}
[.] {return '.';}
[+] {return '+';}
[-] {return '-';}
[*] {return '*';}
[/] {return '/';}
[%] {return '%';}
. {}
%% 
