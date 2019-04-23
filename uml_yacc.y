%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Will convert below to C++ likely instead of C, much easier to work with.
//Will store identifiers in bst or trie. To reuse characters, I will build a trie instead
//for now static 26.




int yydebug = 1;

void yyerror(const char* err){


	fprintf(stderr, "%s\n", err);
}

int yywrap(){

	return 1;

}

main(){
	yyparse();
}


/*ToDo:

  -DETERMINE WAY TO HOLD STRUCTURES THAT MAKES CREATING AND ACCESSING THEM EASY.
  -DETERMINE BEST WAY TO OUTPUT THEM.( CONSOLE PRINTS FOR NOW) 



*/

%}


%union{
	char* string;
	char symbol[2];
	char character;

};

%token <string> IDENTIFIER;
%token <string> COMMAND;
%token <string> META_TYPE;
%token <string> DATA_TYPE;
%token <string> STRUCTURE;
%token <string> PRINT_SPECIFICATION;
%token <character> ACCESS;
%token <symbol> RELATION; 

%%

command:
      	COMMAND meta_data STRUCTURE IDENTIFIER
	{

		if (strcmp($1, "create") != 0){
			YYABORT;
		}
		else{
			printf("Creation of structure command %s\n", $1);
			YYACCEPT;
		}
	}
	|
	COMMAND meta_data ACCESS data_type IDENTIFIER RELATION IDENTIFIER
	{
		if (strcmp($1, "create") != 0){
			YYABORT;
		}
		else{

			printf("Creating member %s of structure %s\n", $5, $7);
		}
	}
	|
	COMMAND print_format IDENTIFIER
	{

		if (strcmp($1,"print") == 0){

			printf("Did print command\n");
			//Accept this as valid syntax, then keep going, untl not accept
			YYACCEPT;
		}
		else{

			printf("Incorrect syntax\n");
			YYABORT;
		}


	}
	|	
	IDENTIFIER RELATION IDENTIFIER
	{
		//Both identifiers here must refer to structure.
	}
	;

meta_data:
	 |
	META_TYPE
	;
data_type:
	DATA_TYPE
	|
	IDENTIFIER
	{
		//Makes sure identifier exists and is of a type.
	}
	;

print_format:
	    |
	print_format PRINT_SPECIFICATION 
     	; 
%%



