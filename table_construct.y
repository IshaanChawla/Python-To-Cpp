%{
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
%}

%union {
	int var_type;
	char* text;
	struct function_args *func;
	struct variable_args *var;
}
%token IDEN NUM STRING REAL_NO IF NAME MAIN E_O_F RETURN CLASS SELF

%%
start : expr
      | start expr
;
expr : variable '=' const {
	add_local_or_class_variable_using_const($1.var->iden,$3.var_type,$1.var->is_class);
}
     | variable '=' variable any_token {
	if(!$3.var->is_class)
		add_local_or_class_variable_using_other_variable($1.var->iden,$3.var->iden,$1.var->is_class);
	else
		add_local_or_class_variable_using_other_class_variable($1.var->iden,$3.var->iden,$1.var->is_class);	
}
     | variable '=' function {
	add_local_or_class_variable_using_func_return_type_or_class($1.var->iden,$3.func->iden,$3.func->args,$1.var->is_class);
}
     | variable '=' SELF '.' function {
	add_local_or_class_variable_using_class_function($1.var->iden,$5.func->iden,$5.func->args,$1.var->is_class);
}
     | variable '=' variable '.' function {
	//add_local_or_class_variable_using_class_function($1.var->iden,$5.func->iden,$5.func->args,$1.var->is_class,0);
}
     | function {add_or_update_function($1.func->iden,$1.func->args);}

     | IF NAME '=' '=' MAIN {add_or_update_function("main",NULL);}
     
     | RETURN variable {add_return_value($2.var->iden,$2.var->is_class);}
     
     | CLASS IDEN {add_class($2.text);}
     
     | error '\n' {yyerrok;yyclearin;}
     | E_O_F {return 0;} 
;
variable : IDEN {
	$$.var = (struct variable_args*)malloc(sizeof(struct variable_args));
	$$.var->iden = (char*)malloc(sizeof($1.text)*sizeof(char)+1);
	strcpy($$.var->iden,$1.text);
	$$.var->is_class = 0;
}
	 | SELF '.' IDEN {
	$$.var = (struct variable_args*)malloc(sizeof(struct variable_args));
	$$.var->iden = (char*)malloc(sizeof($3.text)*sizeof(char)+1);
	strcpy($$.var->iden,$3.text);
	$$.var->is_class = 1;
} 
;
function : IDEN '(' arg_list ')'{
	struct function_args new_func;
	new_func.iden = (char*)malloc(sizeof($1.text)*sizeof(char)+1);
	strcpy(new_func.iden,$1.text);
	new_func.args = (char*)malloc(sizeof($3.text)*sizeof(char)+1);
	strcpy(new_func.args,$3.text);
	$$.func = &new_func;
}
         | IDEN '(' ')'{
	struct function_args new_func;
	new_func.iden = $1.text;
	new_func.args = NULL;
	$$.func = &new_func;
}
;
arg_list : variable ',' arg_list {
	$$.text = (char*)malloc(sizeof($1.var->iden)*sizeof(char)+1);
	strcpy($$.text,$1.var->iden);	
	strcat($$.text,", ");
	strcat($$.text,$3.text);
}
	 | SELF ',' arg_list {
	$$.text = (char*)malloc(sizeof("self")*sizeof(char)+1);
	strcpy($$.text,"self");	
	strcat($$.text,", ");
	strcat($$.text,$3.text);
}
         | variable {
	$$.text = (char*)malloc(sizeof($1.var->iden)*sizeof(char)+1);
	strcpy($$.text,$1.var->iden);
}
	 | SELF {
	$$.text = (char*)malloc(sizeof("self")*sizeof(char)+1);
	strcpy($$.text,"self");
}
;
const : STRING {
	$$.var_type = $1.var_type;
}
      | NUM {
	$$.var_type = $1.var_type;
}
      | REAL_NO {
	$$.var_type = $1.var_type;
}
;
any_token : '+' {}
          | '\n' {}
          | '-' {}
          | '*' {}
          | '/' {}          
	  | '%' {}
;
%%

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
