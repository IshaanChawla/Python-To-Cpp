%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include"struct_table.h"
	int i=0;
	int j=0;
	int inClass = 0;
	char current_class_name[40];
	char current_block_spacing[100];
	char raw_input_string[100];	
	char iden[50];	
	struct node
	{
		char spacing[100];
	};
	struct stack
	{
		struct node arr[100];
		int top;
	}spacing_stack;
	void push(struct stack *the_stack, char* spacing)
	{
		the_stack->top++;
		strcpy(the_stack->arr[the_stack->top].spacing,spacing);		
	}
	void pop(struct stack *the_stack)
	{
		if(the_stack->top != -1)		
			the_stack->top--;
	}
	void quotes_check(char* xxtext)
	{
		int i=0,j=0;
		for(i=0;i<xxleng;i++)
		{
			if(xxtext[i] == '\'')
			{
				j=i+1;
				while(xxtext[j] != '\'')
					j++;
				if(j != i+2)
				{
					if(j != i+3 || (j == i+3 && xxtext[i+1] != '\\'))
					{
						xxtext[i] = '\"';
						xxtext[j] = '\"';
					}
				}
				i = j+1;
			}
			else if(xxtext[i] == '\"')
			{
				j=i+1;
				while(xxtext[j] != '\"')
					j++;
				if(j == i+2)
				{
					xxtext[i] = '\'';
					xxtext[j] = '\'';
				}
				i = j+1;
			}
		}
	}
	void write_arguments(char* xxtext,int start,int end_bracket)
	{
		int i;
		quotes_check(xxtext);
		for(i=start;i<xxleng;i++)
		{
			if (xxtext[i] == ':')
			{
				if(end_bracket)
					fprintf(xxout,")");
				fprintf(xxout,"%s{",current_block_spacing);
				break;
			}
			fprintf(xxout,"%c",xxtext[i]);
		}
	}
	char* replace_substring(char* str, char* original_sub, char* replace_sub)
	{
	    	char *buffer = (char*)malloc(4096*sizeof(char));
	    	char *p;
	     
	    	if(!(p = strstr(str, original_sub)))
	    	return str;
	     
	    	strncpy(buffer, str, p-str);
	    	buffer[p-str] = '\0';
	     
	    	sprintf(buffer+(p-str), "%s%s", replace_sub, p+strlen(original_sub));
	     	
		if (strstr(buffer, original_sub))
			buffer = replace_substring(buffer,original_sub,replace_sub);
		
	    	return buffer;
	}
	void remove_substring(char* str,char* substring)
	{
		size_t length = strlen(substring);
    		while((str = strstr(str, substring)))
		{
			memmove(str, str + length, 1 + strlen(str + length));
		}
	}
	void remove_self(char* xxtext)
	{
		remove_substring(xxtext,"self,");
		remove_substring(xxtext,"self");
	}
%}
iden [a-zA-Z_][a-zA-Z0-9_]*
%%
("\n")+[ \t]+ {
	if(strcmp(xxtext,current_block_spacing) != 0)
	{
		if(strlen(current_block_spacing) < strlen(xxtext))
		{
			push(&spacing_stack,xxtext);
			strcpy(current_block_spacing,xxtext);
		}
		else
		{			
			pop(&spacing_stack);
			strcpy(current_block_spacing,spacing_stack.arr[spacing_stack.top].spacing);
			fprintf(xxout,"%s}",current_block_spacing);
		}
	}
}
"\n"+. {
	pop(&spacing_stack);
	while(spacing_stack.top != -1)
	{
		strcpy(current_block_spacing,spacing_stack.arr[spacing_stack.top].spacing);
		fprintf(xxout,"%s}",current_block_spacing);
		pop(&spacing_stack);
	}
	fprintf(xxout,"\n");
	inClass = 0;
	strcpy(current_block_spacing,"");
	strcpy(current_class_name,"");
	yyless(1);
}
"if "(.*)":" {
	xxtext = replace_substring(xxtext,"self.","this->");
	if (strstr(xxtext,"__name__") && strstr(xxtext,"__main__"))
	{
		fprintf(xxout,"int main()\n{");
	}
	else
	{
		fprintf(xxout,"%sif(",current_block_spacing);
		write_arguments(xxtext,3,1);
	}
}
"elif "(.*)":" {
	xxtext = replace_substring(xxtext,"self.","this->");
	fprintf(xxout,"%selse if(",current_block_spacing);
	write_arguments(xxtext,5,1);
}
"else"[ ]*":" {
	xxtext = replace_substring(xxtext,"self.","this->");
	fprintf(xxout,"%selse",current_block_spacing);
	write_arguments(xxtext,5,0);
}
"while "(.*)":" {
	xxtext = replace_substring(xxtext,"self.","this->");
	fprintf(xxout,"%swhile(",current_block_spacing);
	write_arguments(xxtext,6,1);
}
"print "(.*) {
	xxtext = replace_substring(xxtext,"self.","this->");
	iden[0] = '\0';	
	fprintf(xxout,"%scout << ",current_block_spacing);
	i = 0;
	j = 0;
	for(i=6;i<strlen(xxtext);i++)
	{
		if(xxtext[i] == '+' || xxtext[i] == ',')
		{
			iden[j] = '\0';
			fprintf(xxout,"%s << ",iden);
			iden[0] = '\0';
			j = 0;
		}
		else if(xxtext[i] != ' ')
		{
			iden[j] = xxtext[i];
			j++;
		}
	}
	iden[j] = '\0';
	fprintf(xxout,"%s;",iden);
	iden[0] = '\0';
}
"return"(.*) {
	xxtext = replace_substring(xxtext,"self.","this->");
	fprintf(xxout,"%s%s;",current_block_spacing,xxtext);
}
{iden}[ ]*"="[ ]*"raw_input"(.*) {
	xxtext = replace_substring(xxtext,"self.","this->");
	i = 0;
	j = 0;	
	iden[0] = '\0';
	raw_input_string[0] = '\0';
	while(xxtext[i] != ' ')
	{
		iden[i] = xxtext[i];
		i++;
	}
	iden[i] = '\0';
	while(xxtext[i] != '(')
		i++;
	i++;
	while(xxtext[i] == ' ')
		i++;
	if(xxtext[i] == '\"' || xxtext[i] == '\'')
	{
		raw_input_string[j] = '\"';
		i++;
		j++; 
		while(xxtext[i] != '\"' && xxtext[i] != '\'')
		{
			raw_input_string[j] = xxtext[i];
			j++;
			i++;
		}
		raw_input_string[j] = '\"';
		raw_input_string[j+1] = '\0';
		fprintf(xxout,"%scout << %s;",current_block_spacing,raw_input_string);	
	}
	fprintf(xxout,"%scin >> %s;",current_block_spacing,iden);
}
"class "(.*)":" {
	xxtext = replace_substring(xxtext,"self.","this->");
	fprintf(xxout,"class ");
	i = 6;
	j = 0;
	inClass = 1;
	while(xxtext[i] != '(')
	{
		fprintf(xxout,"%c",xxtext[i]);
		current_class_name[i-6] = xxtext[i];		
		i++;
	}
	current_class_name[i-6] = '\0';
	i++;
	while(xxtext[i] == ' ')
		i++;
	while(xxtext[i] != ')')
	{
		if(j == 0)
		{	
			fprintf(xxout," : public %c",xxtext[i]);
			j = 1;
		}		
		else
		{
			if(xxtext[i] == ',')
				fprintf(xxout,", public ");
			else
				fprintf(xxout,"%c",xxtext[i]);
		}
		i++;
	}
	fprintf(xxout,"\n{");
	push(&spacing_stack,"\n");
}
"def "(.*)":" {
	remove_self(xxtext);
	if(inClass == 1)
	{
		if(strstr(xxtext,"__init__"))
		{
			xxtext = replace_substring(xxtext,"__init__",current_class_name);
			fprintf(xxout,"%s",current_block_spacing);
		}
		else
		{
			fprintf(xxout,"%s_____ ",current_block_spacing);
		}
	}
	else
	{
		fprintf(xxout,"%s_____ ",current_block_spacing);
	}	
	if(inClass != 1)
	{
		strcat(current_block_spacing,"\n");		
		push(&spacing_stack,current_block_spacing);
	}
	write_arguments(xxtext,4,0);
}
(.*) {
	xxtext = replace_substring(xxtext,"self.","this->");
	quotes_check(xxtext);
	fprintf(xxout,"%s%s;",current_block_spacing,xxtext);
}
"\n"+ {}
%%
main()
{
	spacing_stack.top = -1;
	xxin = fopen("./Python Codes/test_code_1.py","r");
	xxout = fopen("./Cpp Codes/cpp_code_1.cpp","w");
	fprintf(xxout,"#include<iostream>\n#include<stdio.h>\nusing namespace std;\n\n");
	xxlex();
	fprintf(xxout,"\n\treturn 0;\n}");
}
int xxwrap()
{
	return 1;
}
