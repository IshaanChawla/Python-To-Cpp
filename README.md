To execute the program :

Requirements : flex and bison

Phase 1 and 2 are still not integrated

Phase 1: Getting Type of Variables

lex table_construct.l
yacc -d table_construct.y
gcc -w lex.yy.c y.tab.c -ll
./a.out


Phase 2 : Converting python syntax to Cpp

lex -P x pythontocpp.l
gcc lex.xx.c -o pythontocpp
./pythontocpp

