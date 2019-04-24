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

int printStructure(char* structureName, char* options){

	structure* s = getStructure(sTrie, structureName);

	//Okay, so now how to print it.
	//let's say just basic one first.

	//Instead of enum prob better just to keep as same time, honestly.

	char* structureTypeName = "class";

	if (s->type == INTERFACE){
		structureTypeName = "interface";
	}



	printf("%s %s\n", structureTypeName, structureName);
	memberLL* members = getMembers(s);

	memberLL* current = members;

	while (current != NULL){

		
		printf("looking at member %s %s\n", current->data->type,  current->data->name);
		current = current->next;

	}

	
}

//So from the atoms I get back, this is create, member stuff and all that.
int createStructure(char* structureName, char* type){

	structure* s = (structure*)malloc(sizeof(structure));
	s->members = NULL;
	s->name = structureName;
	structureType stype = CLASS;

	if (strcmp(type, "interface") == 0){

		stype = INTERFACE;
	
	}

	s->type = stype;


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


	member* m = getMemberNode(memberName);

	int added = addMember(s, m);

	return added;

}




int deleteMemberFromStructure(char* structureName, char* memberName){


	structure* s = getStructure(sTrie, structureName);

	if (s == NULL){
		return 0;
	}

	//Maybe remove based on name instead of node, to avoid traversingtwice
	//but it's just odubling so not huge deal, but hindsight consideration.

	member* m = getMemberNode(memberName);

	return removeMember(s,m);
	

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
%type <string> print_format;
%%


/*First non terminal is what should be left in stack, so to allow multiple command
same as allowing multiple meta_data specifications, was overthinking this*/
program:
       |
       program command 
       ;

command:
      	COMMAND meta_data STRUCTURE IDENTIFIER '\n'
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
	COMMAND meta_data ACCESS data_type IDENTIFIER RELATION IDENTIFIER '\n'
	{
		//Add update here too
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
	/*thought having the statement terminator would fix it, but I guess not */
	COMMAND print_format IDENTIFIER '\n'
	{
		//Need to rethink grammar.

		printf("Here?\n");
		if (strcmp($1,"print") == 0){

			printStructure($3,$2);
			//Accept this as valid syntax, then keep going, untl not accept
		}
		else{

			YYABORT;
		}
	}
	|	
	/*Issue is it chooses to shift this to try to match it, that's why need the null terminator*/
	COMMAND IDENTIFIER RELATION IDENTIFIER '\n' 
	{
	
	/*TODO: figure out order of operations of stuff as needed, I mean right now just basic values but
	//if I were to make this more interesting with precedance, like removing should be done before adding or something like that
	//maybe sub commands? like instead of print like a get all classes or get class, <identifier>,...<identifier_i> then add a member to each one.
	//that would be good. For now get all basic functionality done, then work on sub commands. core my home, work that core.
		//Then depending on relation and command do stuff accordingly.
*/
		if (strcmp($1, "delete") == 0){

			int deleted = 0;
			//If delete, then process what identifiers are.

			//Checking if last operand is structure.
			int isStructure = structureExists(sTrie, $4);


			if (isStructure){

				
				isStructure = structureExists(sTrie, $2);

				if (isStructure){

					//Then pass in relation to structure symbol table of function pointers
					//need to look into function pointers in C again.
					
				}
				else{

					//Otherwise assume is member > structure, and try to delete.
					deleted = deleteMemberFromStructure($4, $2);
					if (deleted){

						printf("Deleted member %s from %s\n", $2, $4);
					}
					else{
					
						//in future print error for now means didn't exist.
						printf("member %s does not exist in %s\n", $2, $4);
					}
				}
			}
			

			//ambigiouity may occur.


			//Could be any relation and any identifier
			//ie: class to class and member to class, delete member from class. remove relationship of class to class.
		}
		

	}
	|
	IDENTIFIER RELATION IDENTIFIER '\n'
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



