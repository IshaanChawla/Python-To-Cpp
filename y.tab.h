#define IDEN 257
#define NUM 258
#define STRING 259
#define REAL_NO 260
#define IF 261
#define NAME 262
#define MAIN 263
#define E_O_F 264
#define RETURN 265
#define CLASS 266
#define SELF 267
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	int var_type;
	char* text;
	struct function_args *func;
	struct variable_args *var;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
