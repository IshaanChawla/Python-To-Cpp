struct variable
{
	char* identifier;
	int type; //-5 - Not known,-4 - char, -3 - char*, -2 - int, -1 - float, 0+ - Class Var
	int pointer; //0 - No, 1 - Yes
	struct variable* dependency;
	struct class_type* class_dependency; 
	char* class_function_dependency; 
};
struct function
{
	char* identifier;
	struct variable* return_type;
	struct variable args[10];
	int current_arg;
	struct variable local_var[20];
	int current_local_var;
};
struct class_type
{
	char* identifier;
	struct variable class_var[10];
	int current_class_var;
	struct function *class_func[10];
	int current_class_func;
};
