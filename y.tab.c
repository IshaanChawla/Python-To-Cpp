#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "table_construct.y"
	#include<stdio.h>
	#include<stdlib.h>
	#include"struct_table.h"
	struct function_args 
	{
		char* iden;
		char* args;
	};
	struct variable_args
	{
		char* iden;
		int is_class;
	};
	extern FILE * yyin;
	struct function function_name[20];
	struct class_type class_name[10];
	int current_function = 0;
	int current_class = 0;
	int get_function(char* func_name)
	{
		int i;
		for(i=0;i<current_function;i++)
			if(strcmp(function_name[i].identifier,func_name) == 0)
				return i;
		return -1;
	}
	int get_class(char* the_class_name)
	{
		int i;
		for(i=0;i<current_class;i++)
			if(strcmp(class_name[i].identifier,the_class_name) == 0)
				return i;
		return -1;
	}
	void add_args(char* arg_var)
	{
		struct variable* new_var = &function_name[current_function].args[function_name[current_function].current_arg];
		new_var->identifier = (char*)malloc(sizeof(arg_var)*sizeof(char)+1);
		strcpy(new_var->identifier,arg_var);
		new_var->type = -5;
		function_name[current_function].current_arg++;
	}
	struct variable* get_class_var(char* var_name)
	{
		int i;
		struct variable* other_var;
		for(i=0;i<class_name[current_class-1].current_class_var;i++)
		{
			other_var = &class_name[current_class-1].class_var[i];
			if(strcmp(other_var->identifier,var_name) == 0)
				return other_var;
		}
		return NULL;
	}
	struct variable* get_local_var(char* var_name)
	{
		int i=0;	
		struct variable* other_var = NULL;		
		for(i=0;i<function_name[current_function-1].current_local_var;i++)
		{
			other_var = &function_name[current_function-1].local_var[i];
			if(strcmp(other_var->identifier,var_name) == 0)
				return other_var;
		}
		return NULL;
	}	
	struct variable* get_args(char* other_var_name)
	{
		int i=0;
		struct variable* other_var = NULL;
		for(i=0;i<function_name[current_function-1].current_arg;i++)
		{
			other_var = &function_name[current_function-1].args[i];
			if(strcmp(other_var->identifier,other_var_name) == 0)
				return other_var;
		}
		return NULL;
	}
	struct variable* get_other_variable(char* other_var_name)
	{
		struct variable* other_var = get_args(other_var_name);
		if(other_var)
			return other_var;
		other_var = get_local_var(other_var_name);		
		return (other_var) ? other_var : NULL;
	}
	void add_or_update_function(char* func_name,char* arg_list)
	{		
		int i,j,k=0,z=0;
		char args[20];
		struct variable *local_variable_in_use;
		for(i=0;i<current_function;i++)
			if(strcmp(function_name[i].identifier,func_name) == 0)
				break;
		if (i == current_function)
		{
			function_name[current_function].current_arg = 0;
			function_name[current_function].current_local_var = 0;
			function_name[current_function].return_type = NULL;
			function_name[current_function].identifier = (char*)malloc(sizeof(func_name)*sizeof(char)+1);
			strcpy(function_name[current_function].identifier,func_name);
			if(arg_list != NULL)
			{
				k = 0;
				if(strstr(arg_list,"self"))
				{
					class_name[current_class-1].class_func[class_name[current_class-1].current_class_func] = &function_name[current_function];
					class_name[current_class-1].current_class_func++;
				}
				for(j=0;j<strlen(arg_list);j++)
				{
					if(arg_list[j] == ',')
					{
						args[k] = '\0';
						add_args(args);
						k=0;
						j++;
					}
					else
					{
						args[k++] = arg_list[j];
					}
				}
				add_args(args);
			}
			current_function++;
		}
		else
		{
			if(arg_list != NULL)
			{
				k = 0;
				z = 0;
				for(j=0;j<function_name[i].current_arg;j++)
				{					
					if(j == 0 && strcmp(function_name[i].args[j].identifier,"self") == 0)
					{
						continue;
					}		
					while(z < strlen(arg_list))
					{
						if(arg_list[z] == ',')
						{
							args[k] = '\0';
							z += 2;
							k = 0;
							break;
						}	
						else
						{
							args[k++] = arg_list[z];
						}					
						z++;
					}
					local_variable_in_use = get_local_var(args);
					function_name[i].args[j].type = local_variable_in_use->type;
				}
			}
		}
	}
	void add_local_or_class_variable_using_const(char* var_name,int var_type,int is_class)
	{
		struct variable* new_var;
		struct variable* check_var;		
		if(!is_class)
		{
			new_var = &function_name[current_function-1].local_var[function_name[current_function-1].current_local_var];
			check_var = get_other_variable(var_name);
		}
		else
		{
			new_var = &class_name[current_class-1].class_var[class_name[current_class-1].current_class_var];
			check_var = get_class_var(var_name);
		}
		if(check_var == NULL)
		{
			new_var->identifier = (char*)malloc(sizeof(var_name)*sizeof(char)+1);
			new_var->pointer = 0;
			strcpy(new_var->identifier,var_name);
			new_var->type = var_type;
			if(!is_class)
				function_name[current_function-1].current_local_var++;
			else
				class_name[current_class-1].current_class_var++;
		}
	}
	void add_local_or_class_variable_using_other_variable(char* var_name,char* other_var_name,int is_class)
	{	
		struct variable* new_var;
		struct variable* other_var = get_other_variable(other_var_name);
		struct variable* check_var;		
		if(!is_class)
		{		
			new_var = &function_name[current_function-1].local_var[function_name[current_function-1].current_local_var];
			check_var = get_other_variable(var_name);
		}
		else
		{
			new_var = &class_name[current_class-1].class_var[class_name[current_class-1].current_class_var];
			check_var = get_class_var(var_name);
		}
		if(check_var == NULL)
		{		
			new_var->identifier = (char*)malloc(sizeof(var_name)*sizeof(char)+1);
			strcpy(new_var->identifier,var_name);
			new_var->pointer = 0;
			new_var->type = other_var->type;
			if(other_var->type == -5)
			{
				new_var->dependency = other_var;
				new_var->class_dependency = NULL;
				new_var->class_function_dependency = NULL;
			}
			else
				new_var->pointer = 1;
			if(!is_class)
				function_name[current_function-1].current_local_var++;
			else
				class_name[current_class-1].current_class_var++;
		}
	}
	char* get_type(int type)
	{
		switch(type)
		{
			case -5:
				return "Unknown";
			case -4:
				return "char";
			case -3:
				return "char*";
			case -2:
				return "int";
			case -1:
				return "float";
			default:
				return class_name[type].identifier;
		}
	}
	void print_table()
	{
		int i=0,j=0;
		for(i=0;i<current_class;i++)
		{
			printf("%s :\n",class_name[i].identifier);
			for(j=0;j<class_name[i].current_class_var;j++)
				printf("%s - %s\n",class_name[i].class_var[j].identifier,get_type(class_name[i].class_var[j].type));
			for(j=0;j<class_name[i].current_class_func;j++)
				printf("%s\n",class_name[i].class_func[j]->identifier);
			printf("\n\n");
		}
		for(i=0;i<current_function;i++)
		{
			printf("%s :\n",function_name[i].identifier);
			if(function_name[i].return_type == NULL)
				printf("Return Type : void\n");
			else
				printf("Return Type : %s\n",get_type(function_name[i].return_type->type));
			for(j=0;j<function_name[i].current_arg;j++)
				printf("%s - %s\n",function_name[i].args[j].identifier,get_type(function_name[i].args[j].type));
			for(j=0;j<function_name[i].current_local_var;j++)
				printf("%s - %s\n",function_name[i].local_var[j].identifier,get_type(function_name[i].local_var[j].type));
			printf("\n\n");
		}	
	}
	void remaining_unknown_types()
	{
		int i=0,j=0,k=0;		
		for(i=0;i<current_function;i++)
		{
			for(j=0;j<function_name[i].current_local_var;j++)
				if(function_name[i].local_var[j].type == -5)
				{
					if(function_name[i].local_var[j].dependency)				
						function_name[i].local_var[j].type = function_name[i].local_var[j].dependency->type;
					if(function_name[i].local_var[j].class_dependency)
					{
						for(k=0;k<function_name[i].local_var[j].class_dependency->current_class_func;k++)
							if(strcmp(function_name[i].local_var[j].class_dependency->class_func[k]->identifier, function_name[i].local_var[j].class_function_dependency) == 0)
							{
								function_name[i].local_var[j].type = function_name[i].local_var[j].class_dependency->class_func[k]->return_type->type;
							}
					}
					if(function_name[i].local_var[j].type != -5)
					{					
						function_name[i].local_var[j].dependency = NULL;
						function_name[i].local_var[j].class_dependency = NULL;
						function_name[i].local_var[j].class_function_dependency = NULL;
					}
				}
		}
		for(i=0;i<current_class;i++)
		{
			for(j=0;j<class_name[i].current_class_var;j++)
				if(class_name[i].class_var[j].type == -5)
				{
					if(class_name[i].class_var[j].dependency)
						class_name[i].class_var[j].type = class_name[i].class_var[j].dependency->type;
					if(class_name[i].class_var[j].class_dependency)
					{
						for(k=0;k<class_name[i].class_var[j].class_dependency->current_class_func;k++)
							if(strcmp(class_name[i].class_var[j].class_dependency->class_func[k]->identifier, class_name[i].class_var[j].class_function_dependency) == 0)
							{
								class_name[i].class_var[j].type = class_name[i].class_var[j].class_dependency->class_func[k]->return_type->type;
							}
					}
					if(function_name[i].local_var[j].type != -5)
					{					
						class_name[i].class_var[j].dependency = NULL;
						class_name[i].class_var[j].class_dependency = NULL;
						class_name[i].class_var[j].class_function_dependency = NULL;
					}
				}
		}
		for(i=0;i<current_function;i++)
		{
			for(j=0;j<function_name[i].current_local_var;j++)
				if(function_name[i].local_var[j].type == -5)
				{
					if(function_name[i].local_var[j].dependency)				
						function_name[i].local_var[j].type = function_name[i].local_var[j].dependency->type;
					if(function_name[i].local_var[j].class_dependency)
					{
						for(k=0;k<function_name[i].local_var[j].class_dependency->current_class_func;k++)
							if(strcmp(function_name[i].local_var[j].class_dependency->class_func[k]->identifier, function_name[i].local_var[j].class_function_dependency) == 0)
							{
								function_name[i].local_var[j].type = function_name[i].local_var[j].class_dependency->class_func[k]->return_type->type;
							}
					}
					if(function_name[i].local_var[j].type != -5)
					{					
						function_name[i].local_var[j].dependency = NULL;
						function_name[i].local_var[j].class_dependency = NULL;
						function_name[i].local_var[j].class_function_dependency = NULL;
					}
				}
		}
	}
	void add_return_value(char* var_name,int is_class)
	{
		if(!is_class)
			function_name[current_function-1].return_type = get_other_variable(var_name);
		else
			function_name[current_function-1].return_type = get_class_var(var_name);
	}
	void add_class(char* the_class_name)
	{
		struct class_type *new_var = &class_name[current_class];		
		new_var->identifier = (char*)malloc(sizeof(the_class_name)*sizeof(char)+1);
		strcpy(new_var->identifier,the_class_name);
		new_var->current_class_var = 0;
		new_var->current_class_func = 0;
		current_class++;
	}
	void add_local_or_class_variable_using_other_class_variable(char* var_name,char* other_var_name,int is_class)
	{
		struct variable* new_var; 
		struct variable* other_var;
		struct variable* check_var;
		if(!is_class)
		{
			new_var = &function_name[current_function-1].local_var[function_name[current_function-1].current_local_var];
			check_var = get_other_variable(var_name);
		}
		else
		{
			new_var = &class_name[current_class-1].class_var[class_name[current_class-1].current_class_var];
			check_var = get_class_var(var_name);
		}
		other_var = get_class_var(other_var_name);
		if(check_var == NULL)
		{
			new_var->identifier = (char*)malloc(sizeof(var_name)*sizeof(char)+1);
			strcpy(new_var->identifier,var_name);
			new_var->pointer = 0;
			new_var->type = other_var->type;
			if(other_var->type == -5)
			{
				new_var->dependency = other_var;
				new_var->class_dependency = NULL;
				new_var->class_function_dependency = NULL;
			}
			else
				new_var->pointer = 1;
			if(!is_class)
				function_name[current_function-1].current_local_var++;
			else
				class_name[current_class-1].current_class_var++;
		}
	}
	void add_local_or_class_variable_using_func_return_type_or_class(char* var_name,char* func_or_class_name,char* arguments,int is_class)
	{		
		int x = get_function(func_or_class_name),y = get_class(func_or_class_name),k = 0,m = -1,z,i,j;
		struct variable* new_var = NULL;
		struct variable* check_var = NULL;
		struct variable* other_var = NULL;
		struct variable* local_variable_in_use = NULL;
		char args[20];
		if(!is_class)
		{
			new_var = &function_name[current_function-1].local_var[function_name[current_function-1].current_local_var];
			check_var = get_other_variable(var_name);
		}
		else
		{
			new_var = &class_name[current_class-1].class_var[class_name[current_class-1].current_class_var];
			check_var = get_class_var(var_name);
		}
		if(x != -1)
		{
			add_or_update_function(func_or_class_name,arguments);
			other_var = function_name[x].return_type;
			m = 0;
		}
		else if (y != -1)
		{
			for(i=0;i<class_name[current_class-1].current_class_func;i++)
			{
				if(strcmp(class_name[current_class-1].class_func[i]->identifier,"__init__") == 0)
				{
					if(arguments != NULL)
					{
						k = 0;
						z = 0;
						for(j=0;j<class_name[current_class-1].class_func[i]->current_arg;j++)
						{					
							if(j == 0 && strcmp(class_name[current_class-1].class_func[i]->args[j].identifier,"self") == 0)
							{
								continue;
							}		
							while(z < strlen(arguments))
							{
								if(arguments[z] == ',')
								{
									args[k] = '\0';
									z += 2;
									k = 0;
									break;
								}	
								else
								{
									args[k++] = arguments[z];
								}					
								z++;
							}
							local_variable_in_use = get_local_var(args);
							class_name[current_class-1].class_func[i]->args[j].type = local_variable_in_use->type;
						}
					}
				}
			}			
			m = 1;
		}
		if(check_var == NULL  && m != -1)
		{
			new_var->identifier = (char*)malloc(sizeof(var_name)*sizeof(char)+1);
			strcpy(new_var->identifier,var_name);
			new_var->pointer = 0;
			if(m == 0)
			{
				new_var->type = other_var->type;
				if(other_var->type == -5)
				{
					new_var->dependency = other_var;
					new_var->class_dependency = NULL;
					new_var->class_function_dependency = NULL;
				}
				else
					new_var->pointer = 1;
			}
			else
			{
				new_var->type = y;
			}
			if(!is_class)
				function_name[current_function-1].current_local_var++;
			else
				class_name[current_class-1].current_class_var++;
					
		}
	}
	void add_local_or_class_variable_using_class_function(char* var_name,char* func_name,char* argument,int is_class)
	{
		int i=0;		
		struct variable* new_var = NULL;
		struct variable* check_var = NULL;
		struct variable* other_var = NULL;		
		if(!is_class)
		{
			new_var = &function_name[current_function-1].local_var[function_name[current_function-1].current_local_var];
			check_var = get_other_variable(var_name);
		}
		else
		{
			new_var = &class_name[current_class-1].class_var[class_name[current_class-1].current_class_var];
			check_var = get_class_var(var_name);
		}
		if(check_var == NULL)
		{
			new_var->identifier = (char*)malloc(sizeof(var_name)*sizeof(char)+1);
			strcpy(new_var->identifier,var_name);
			new_var->pointer = 0;
				
			for(i=0;i<class_name[current_class-1].current_class_func;i++)
			{
				if(strcmp(class_name[current_class-1].class_func[i]->identifier,func_name) == 0)
					break;
			}
			if(i != class_name[current_class-1].current_class_func)
			{
				other_var = class_name[current_class-1].class_func[i]->return_type;
				new_var->type = other_var->type;
				if(other_var->type == -5)
				{
					new_var->dependency = other_var;
					new_var->class_dependency = NULL;
					new_var->class_function_dependency = NULL;
				}
				else
					new_var->pointer = 1;
			}
			else
			{
				new_var->type = -5;
				new_var->dependency = NULL;
				new_var->class_dependency = &class_name[current_class-1];
				new_var->class_function_dependency = (char*)malloc(sizeof(func_name)*sizeof(char)+1);
				strcpy(new_var->class_function_dependency,func_name);
			}
			if(!is_class)
				function_name[current_function-1].current_local_var++;
			else
				class_name[current_class-1].current_class_var++;
		}  
	}
#line 539 "table_construct.y"
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
#line 569 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

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
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    2,    2,    5,    5,    6,    6,    6,
    6,    3,    3,    3,    4,    4,    4,    4,    4,    4,
};
static const short yylen[] = {                            2,
    1,    2,    3,    4,    3,    5,    5,    1,    5,    2,
    2,    2,    1,    1,    3,    4,    3,    3,    3,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
};
static const short yydefred[] = {                         0,
    0,    0,    0,   13,    0,    0,    0,    0,    1,    0,
    8,   12,    0,    0,   14,   10,   11,    0,    2,    0,
    0,   17,    0,    0,    0,   15,   23,   22,   24,    0,
    0,    3,    5,    0,    0,   16,    0,    0,    0,   26,
   25,   27,   28,   29,   30,    4,   19,   18,    9,    0,
    6,    0,    7,
};
static const short yydgoto[] = {                          8,
    9,   23,   32,   46,   11,   24,
};
static const short yysindex[] = {                      -197,
   -4,  -28, -243,    0, -254, -237,  -17, -197,    0,  -31,
    0,    0,  -41,  -30,    0,    0,    0, -225,    0, -242,
  -35,    0,   -5,    1,  -14,    0,    0,    0,    0,    3,
   -2,    0,    0, -253, -253,    0, -217, -207, -206,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -28,
    0,  -28,    0,
};
static const short yyrindex[] = {                         0,
    0,   -9,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   14,    0,   20,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   11,
    0,    0,    0,
};
static const short yygindex[] = {                         0,
   54,    2,    0,    0,  -15,   -8,
};
#define YYTABLESIZE 226
static const short yytable[] = {                         22,
   14,   10,   15,   15,   33,   12,   16,   40,   34,   10,
   18,   13,    7,   21,    2,   27,   28,   29,   14,   17,
   15,   31,   51,   53,   30,   47,   48,   14,   18,   20,
   25,   26,   14,   14,   45,   14,   14,   14,   35,   43,
   41,   36,   42,   39,   44,   49,   37,   15,   38,   50,
   52,   14,   15,   15,   21,   15,   15,   15,    1,    2,
   20,   19,    0,    3,    0,    0,    4,    5,    6,    7,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   15,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   21,
};
static const short yycheck[] = {                         41,
   10,    0,  257,  257,   20,   10,    5,   10,   44,    8,
   46,   40,  267,  267,  257,  258,  259,  260,  262,  257,
   10,   20,   38,   39,  267,   34,   35,   37,   46,   61,
   61,  257,   42,   43,   37,   45,   46,   47,   44,   42,
   43,   41,   45,   46,   47,  263,   61,   37,   46,  257,
  257,   61,   42,   43,   41,   45,   46,   47,  256,  257,
   41,    8,   -1,  261,   -1,   -1,  264,  265,  266,  267,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  257,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  267,
};
#define YYFINAL 8
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 267
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'%'",0,0,"'('","')'","'*'","'+'","','","'-'","'.'","'/'",0,0,0,0,0,
0,0,0,0,0,0,0,0,"'='",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"IDEN","NUM","STRING","REAL_NO","IF","NAME","MAIN","E_O_F","RETURN",
"CLASS","SELF","illegal-symbol",
};
static const char *yyrule[] = {
"$accept : start",
"start : expr",
"start : start expr",
"expr : variable '=' const",
"expr : variable '=' variable any_token",
"expr : variable '=' function",
"expr : variable '=' SELF '.' function",
"expr : variable '=' variable '.' function",
"expr : function",
"expr : IF NAME '=' '=' MAIN",
"expr : RETURN variable",
"expr : CLASS IDEN",
"expr : error '\\n'",
"expr : E_O_F",
"variable : IDEN",
"variable : SELF '.' IDEN",
"function : IDEN '(' arg_list ')'",
"function : IDEN '(' ')'",
"arg_list : variable ',' arg_list",
"arg_list : SELF ',' arg_list",
"arg_list : variable",
"arg_list : SELF",
"const : STRING",
"const : NUM",
"const : REAL_NO",
"any_token : '+'",
"any_token : '\\n'",
"any_token : '-'",
"any_token : '*'",
"any_token : '/'",
"any_token : '%'",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 647 "table_construct.y"

main()
{
	yyin = fopen("./Python Codes/test_code_1.py","r");	
	yyparse();
	remaining_unknown_types();
	print_table();
}
yyerror()
{
}
int yywrap()
{
	return 1;
}
#line 808 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 3:
#line 551 "table_construct.y"
	{
	add_local_or_class_variable_using_const(yystack.l_mark[-2].var->iden,yystack.l_mark[0].var_type,yystack.l_mark[-2].var->is_class);
}
break;
case 4:
#line 554 "table_construct.y"
	{
	if(!yystack.l_mark[-1].var->is_class)
		add_local_or_class_variable_using_other_variable(yystack.l_mark[-3].var->iden,yystack.l_mark[-1].var->iden,yystack.l_mark[-3].var->is_class);
	else
		add_local_or_class_variable_using_other_class_variable(yystack.l_mark[-3].var->iden,yystack.l_mark[-1].var->iden,yystack.l_mark[-3].var->is_class);	
}
break;
case 5:
#line 560 "table_construct.y"
	{
	add_local_or_class_variable_using_func_return_type_or_class(yystack.l_mark[-2].var->iden,yystack.l_mark[0].func->iden,yystack.l_mark[0].func->args,yystack.l_mark[-2].var->is_class);
}
break;
case 6:
#line 563 "table_construct.y"
	{
	add_local_or_class_variable_using_class_function(yystack.l_mark[-4].var->iden,yystack.l_mark[0].func->iden,yystack.l_mark[0].func->args,yystack.l_mark[-4].var->is_class);
}
break;
case 7:
#line 566 "table_construct.y"
	{
	/*add_local_or_class_variable_using_class_function($1.var->iden,$5.func->iden,$5.func->args,$1.var->is_class,0);*/
}
break;
case 8:
#line 569 "table_construct.y"
	{add_or_update_function(yystack.l_mark[0].func->iden,yystack.l_mark[0].func->args);}
break;
case 9:
#line 571 "table_construct.y"
	{add_or_update_function("main",NULL);}
break;
case 10:
#line 573 "table_construct.y"
	{add_return_value(yystack.l_mark[0].var->iden,yystack.l_mark[0].var->is_class);}
break;
case 11:
#line 575 "table_construct.y"
	{add_class(yystack.l_mark[0].text);}
break;
case 12:
#line 577 "table_construct.y"
	{yyerrok;yyclearin;}
break;
case 13:
#line 578 "table_construct.y"
	{return 0;}
break;
case 14:
#line 580 "table_construct.y"
	{
	yyval.var = (struct variable_args*)malloc(sizeof(struct variable_args));
	yyval.var->iden = (char*)malloc(sizeof(yystack.l_mark[0].text)*sizeof(char)+1);
	strcpy(yyval.var->iden,yystack.l_mark[0].text);
	yyval.var->is_class = 0;
}
break;
case 15:
#line 586 "table_construct.y"
	{
	yyval.var = (struct variable_args*)malloc(sizeof(struct variable_args));
	yyval.var->iden = (char*)malloc(sizeof(yystack.l_mark[0].text)*sizeof(char)+1);
	strcpy(yyval.var->iden,yystack.l_mark[0].text);
	yyval.var->is_class = 1;
}
break;
case 16:
#line 593 "table_construct.y"
	{
	struct function_args new_func;
	new_func.iden = (char*)malloc(sizeof(yystack.l_mark[-3].text)*sizeof(char)+1);
	strcpy(new_func.iden,yystack.l_mark[-3].text);
	new_func.args = (char*)malloc(sizeof(yystack.l_mark[-1].text)*sizeof(char)+1);
	strcpy(new_func.args,yystack.l_mark[-1].text);
	yyval.func = &new_func;
}
break;
case 17:
#line 601 "table_construct.y"
	{
	struct function_args new_func;
	new_func.iden = yystack.l_mark[-2].text;
	new_func.args = NULL;
	yyval.func = &new_func;
}
break;
case 18:
#line 608 "table_construct.y"
	{
	yyval.text = (char*)malloc(sizeof(yystack.l_mark[-2].var->iden)*sizeof(char)+1);
	strcpy(yyval.text,yystack.l_mark[-2].var->iden);	
	strcat(yyval.text,", ");
	strcat(yyval.text,yystack.l_mark[0].text);
}
break;
case 19:
#line 614 "table_construct.y"
	{
	yyval.text = (char*)malloc(sizeof("self")*sizeof(char)+1);
	strcpy(yyval.text,"self");	
	strcat(yyval.text,", ");
	strcat(yyval.text,yystack.l_mark[0].text);
}
break;
case 20:
#line 620 "table_construct.y"
	{
	yyval.text = (char*)malloc(sizeof(yystack.l_mark[0].var->iden)*sizeof(char)+1);
	strcpy(yyval.text,yystack.l_mark[0].var->iden);
}
break;
case 21:
#line 624 "table_construct.y"
	{
	yyval.text = (char*)malloc(sizeof("self")*sizeof(char)+1);
	strcpy(yyval.text,"self");
}
break;
case 22:
#line 629 "table_construct.y"
	{
	yyval.var_type = yystack.l_mark[0].var_type;
}
break;
case 23:
#line 632 "table_construct.y"
	{
	yyval.var_type = yystack.l_mark[0].var_type;
}
break;
case 24:
#line 635 "table_construct.y"
	{
	yyval.var_type = yystack.l_mark[0].var_type;
}
break;
case 25:
#line 639 "table_construct.y"
	{}
break;
case 26:
#line 640 "table_construct.y"
	{}
break;
case 27:
#line 641 "table_construct.y"
	{}
break;
case 28:
#line 642 "table_construct.y"
	{}
break;
case 29:
#line 643 "table_construct.y"
	{}
break;
case 30:
#line 644 "table_construct.y"
	{}
break;
#line 1179 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
