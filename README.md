To execute the program :

Requirements : flex and bison

Phase 1 and 2 are still not integrated

Phase 1: Getting Type of Variables

1. lex table_construct.l
2. yacc -d table_construct.y
3. gcc -w lex.yy.c y.tab.c -ll
4. ./a.out

Phase 2 : Converting python syntax to Cpp

1. lex -P x pythontocpp.l
2. gcc lex.xx.c -o pythontocpp
3. ./pythontocpp

