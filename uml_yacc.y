%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "member.h"
//Will convert below to C++ likely instead of C, much easier to work with.
//Will store identifiers in bst or trie. To reuse characters, I will build a trie instead
//for now static 26.



//Store them in trie, but just making this to store all that stored
//I mean O(n* 26) v.s O(n) not huge diff, but also have extra space.
typedef struct IdentNode{

	char* identifier;
	struct IdentNode* next;

} IdentifierNode;


int yydebug = 1;

structure_trie* sTrie;

//So from the atoms I get back, this is create, member stuff and all that.
int createStructure(char* structureName, char* type){

	structure* s = (structure*)malloc(sizeof(structure));
	s->members = NULL;
	s->name = structureName;
	printf("creating structure with name %s\n", s->name);
	structureType stype = CLASS;

	if (strcmp(type, "interface") == 0){

		stype = INTERFACE;
	
	}

	s->type = stype;

	puts("Creating structure");

	//Okay, cool seg fault.
	int added = addStructure(sTrie, structureName, s);
	
	return added;

}

int addMemberToStructure(char* structureName, char* memberName, char* data_type, char* metaData, char access){



	//Getting structure.
	structure* s = getStructure(sTrie, structureName);
	
	if (s == NULL){
		//Should also have string for error, but that's polish.
		return 0;
	}

	printf("s name is %s\n", structureName);
	printf("Structure got %s\n", s->name);

	member* m = getMemberNode(memberName);

	puts("Get to here");
	int added = addMember(s, m);

	return added;

}


void yyerror(const char* err){


	fprintf(stderr, "%s\n", err);
}

int yywrap(){

	return 1;

}

main(){

	sTrie = getNode();
	yyparse();
}




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

%type <string> data_type;
%type <string> meta_data;
%%


/*First non terminal is what should be left in stack, so to allow multiple command
same as allowing multiple meta_data specifications, was overthinking this*/
program:
       |
       program command
       ;

command:
      	COMMAND meta_data STRUCTURE IDENTIFIER
	{

		if (strcmp($1, "create") != 0){
			YYABORT;
		}
		else{

			int created = createStructure($4, $3);
			
			if (created){
				printf("created\n");
			}
			else{
				printf("already exists\n");
			//	YYABORT;
			}
		}
	}
	|
	COMMAND meta_data ACCESS data_type IDENTIFIER RELATION IDENTIFIER
	{
		if (strcmp($1, "create") != 0){
			YYABORT;
		}
		else{

			int added = addMemberToStructure($7, $5, $4, $2,$3);
			if (added)
				puts("added member");
			else
				puts("member exists in that structure");	
		}
	}
	|
	COMMAND print_format IDENTIFIER
	{

		puts("Going here?");

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



