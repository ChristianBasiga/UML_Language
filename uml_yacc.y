%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "member.h"
#include "relationships.h"
#include "codeGenerator.h"

//Will convert below to C++ likely instead of C, much easier to work with.
//Will store identifiers in bst or trie. To reuse characters, I will build a trie instead
//for now static 26.

int removeMember(structure* s, member* m);

//Store them in trie, but just making this to store all that stored
//I mean O(n* 26) v.s O(n) not huge diff, but also have extra space.
typedef struct IdentNode{

	char* identifier;
	struct IdentNode* next;

} IdentifierNode;

void append(IdentifierNode* head, char* toAdd){
	IdentifierNode* newIdent = (IdentifierNode*)malloc(sizeof(IdentifierNode));
	newIdent->identifier = toAdd;
	IdentifierNode* current = head;
	while (current->next != NULL){
		
		current = current->next;
	}
	current->next = current;
	current->next = NULL;
}

IdentifierNode* structureIdentifiers;

typedef struct ParamList{



	struct ParamList* next;
	member* data;


} ParamList;

int yydebug = 1;

structure_trie* sTrie;
relationship* relationshipGraph;

int printStructure(char* structureName, char* options){

	structure* s = getStructure(sTrie, structureName);


	if (s == NULL) return 0;
	//Okay, so now how to print it.
	//let's say just basic one first.

	//Instead of enum prob better just to keep as same time, honestly.

	char* structureTypeName = "class";

	if (s->type == INTERFACE){
		structureTypeName = "interface";
	}



	printf("%s %s\n", structureTypeName, structureName);
	
	//Todo: Iterate through graph and display parents and children

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

	//Add to graph as node.
	s->type = stype;


	//Okay, cool seg fault.
	int added = addStructure(sTrie, structureName, s);
	
	if (added){

        if (structureIdentifiers == NULL){
			structureIdentifiers = (IdentifierNode*)malloc(sizeof(IdentifierNode));
		}
		else{
			
			//Create new one and add onto it.
			append(structureIdentifiers, structureName);
		}
		relationship* r = getRelationship(s->name);
		if (relationshipGraph == NULL){
			relationshipGraph = r;
		}
		else{

			addToGraph(relationshipGraph, r);
		}
	}

	return added;

}

 int addMemberToStructure(char* structureName, member* toAdd){
//int addMemberToStructure(char* structureName, char* memberName, char* data_type, char* metaData, char access){



	//Getting structure.
	structure* s = getStructure(sTrie, structureName);
	
	if (s == NULL){
		//Should also have string for error, but that's polish.
		return 0;
	}


	//member* m = getMemberNode(memberName);

	int added = addMember(s, toAdd);

	return added;

}



int deleteAStructure(char* structureName){


	//Are overloads not a thing I guess thats why did it.
	return deleteStructure(sTrie, structureName);
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

int addStructureRelation(char* from, char* to, char* type){


	//Get relationship type, again, so much easier if make const strings
	//but comparison longer.
	relationship_type rType = parseType(type);


	return addRelationship(relationshipGraph, from, to, rType);
}

int removeStructureRelation(char* from, char* to, char* type){

	relationship_type rType = parseType(type);

	return removeRelationship(relationshipGraph, from, to, rType);

}

void yyerror(const char* err){


	fprintf(stderr, "%s\n", err);
}

int yywrap(){

	return 1;

}



int main(){

	sTrie = getNode();
	relationshipGraph = NULL;
	yyparse();


	//Hindsight, yacc should end after the parse, it should fork and 
	//and pass in this info, for testing purposes will just put here.
	//Todo: Generate code based on trie, and relationship graph.
	generateCode(sTrie, relationshipGraph);

    return 0;
}




%}


%union{
	struct ParamList* parameters;
	struct member* memberType;
	char* string;
	char symbol[2];
	char character;
	struct IdentNode* identifiers;
};

%token LEFTP RIGHTP;
%token <string> FUNC;
%token <character> ACCESS;
%token <character> UML;
%token <string> IDENTIFIER;
%token <string> COMMAND;
%token <string> META_TYPE;
%token <string> DATA_TYPE;
%token <string> STRUCTURE;
%token <string> PRINT_SPECIFICATION;
%token <symbol> RELATION; 
%token <character> LPAREN;
%token <character> RPAREN;
%type <string> meta_data;
%type <string> print_format;
%type <string> data_type;
%type <identifiers> identifier_list;
%type <parameters> variables;
%type <memberType> variable;
%type <memberType> function;
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

	/*Virtually the same apart form function literally needing func in it and var not.*/
	COMMAND ACCESS function RELATION IDENTIFIER
	{
		puts("here");
		
		if (strcmp($1, "create") != 0){
			YYABORT;
		}
		else{
			
			$3->accessSpecifier = $2;
			int added = addMemberToStructure($5,$3);
			if (added)
				puts("added member");
			else
				puts("member exists in that structure");	
		}
		
	}
	|
	COMMAND ACCESS variable  RELATION IDENTIFIER 
	{
		puts("here instead");
		//Add update here too
		if (strcmp($1, "create") != 0){
			YYABORT;
		}
		else{

			$3->accessSpecifier = $2;
			int added = addMemberToStructure($5,$3);
			if (added)
				puts("added member");
			else
				puts("member exists in that structure");	
		}
	}
	|
	COMMAND identifier_list PRINT_SPECIFICATION 
	{



		printf("[%p]\n", $2);

		if (strcmp($1,"print") == 0){

			
			IdentifierNode* current = $2;

			while (current != NULL){
				printStructure(current->identifier,$3);
				current = current->next;
			}
			
			//Accept this as valid syntax, then keep going, untl not accept
		}
		else{

			YYABORT;
		}
	}
	|
	COMMAND identifier_list
	{

		if (strcmp($1, "delete") == 0){

			//Delete each one in identifier list.

			IdentifierNode* current = $2;

			while (current != NULL){

				int deleted = deleteAStructure(current->identifier);
				if (deleted){


					printf("Successfully deleted %s\n", current->identifier);
				}
				else{

					printf("Failed to delete %s\n", current->identifier);
					
				}
				current = current->next;
			}
			
		}
		
	}
	|
	
	
	COMMAND UML identifier_list RELATION IDENTIFIER 
	{
		puts("here?");
		//Both identifiers here must refer to structure.
		//Same as below, use symbol table with command bst with nodes of function pointers.
		//Much cleaner than if else ifs
		if (strcmp($1, "create") == 0){

			//UGH, so creating allow, but not deleting for that

			
			IdentifierNode* current = $3;

			while (current != NULL){
				int added = addStructureRelation(current->identifier, $5, $3->identifier);
				current = current->next;
			}
		
			

			
		}
		//but for quick testing else ifs fine.
		else if (strcmp($1, "delete") == 0){
			
			IdentifierNode* current = $3;

			while (current != NULL){
				int deleted = removeStructureRelation(current->identifier, $5, $3->identifier);
				current = current->next;
			}
		}	
			
	}
	|
	/*Deleting relationships and members from structures*/	
	/*this rule should suffice but the distinction would have to made with value of relation token.*/
	COMMAND identifier_list RELATION IDENTIFIER 
	{

		//Then inheritance, granted grammar prob so not doing strcmp stuff like this
		if ($3[0] == '^'){

			//Create relationship
			//Only if both structures.
			int isStructure = structureExists(sTrie, $4);

			if (!isStructure){

				puts("Can only inherit from a structure");
			}
			addStructureRelation($2->identifier, $4, $3);
		}
		
		//When deleting structures need to consider the relationships structure has with other structures
		//composition already has member stuff, inheritance not set up yet.	
		if (strcmp($1, "delete") == 0){

			int deleted = 0;
			//If delete, then process what identifiers are.

			printf("structure identifier is %s\n", $4);
			//Checking if last operand is structure.
			int isStructure = structureExists(sTrie, $4);

			if (isStructure){
	

					

				IdentifierNode* current = $2;

				while (current != NULL){
				//Then here iterate through rest.
					char* other = current->identifier;	
				//But what if it is both a structure and a member.
				//ambigiouity in functionality and syntax here,
				//need to rethink syntax and grammar

				//instead of checking isStructure, should check if member exists within structure
				//for each one
					
					//Otherwise assume is member > structure, and try to delete.
					deleted = deleteMemberFromStructure($4, other);
					if (deleted){

						printf("Deleted member %s from %s\n", other, $4);
					}
					else{
					
						//in future print error for now means didn't exist.
						printf("member %s does not exist in %s\n", other, $4);
					}
					
					current = current->next;
				}
			}
			else{

				printf("Fourth argument must be structure\n");
				YYABORT;
			}
			
		}
	
		
	}
      ;
function:

/*
	FUNC meta_data data_type IDENTIFIER LEFTP RIGHTP{

		puts("here");

		member* m = getMemberNode($4);
		m->type = $3;
		m->mt = FUNCTION;
		m->metaInfo = $2;
		
		$$ = m;
	}
	|*/
	variable LEFTP RIGHTP{


		puts("no parameter method");
		member* m = $1;
		m->mt = FUNCTION;
		m->parameters = NULL;

	}

	|
	variable LEFTP variables RIGHTP
 	{

		puts("with param method");
		//Initialize params.
	
		member* m = $1;
		m->mt = FUNCTION;
		
		ParamList* params = $3;
		
		while (params != NULL){
			
			puts("here???");		
			addParameter(m, params->data);	
			params = params->next;
		}

		if (m->parameters == NULL){

			puts("here? though");

		}
		else{

			puts("here! though");
		}

		$$ = m;
			
	}
	;
	
variables:
	|
	variables variable
    {
		//This should be list of stuf.
		
		if ($1->data == NULL){

				puts("I gotta happen in this instance");	

				$1->data = $2;
				$1->next = NULL;
				$$ = $1;
		}
		else{

				puts("here then?");
			//Otherwise add to list.
			ParamList* current = $1;
			while (current->next != NULL){
				
				current = current->next;	
			}
			ParamList* new = (ParamList*)malloc(sizeof(ParamList));
			new->data = $2;
			new->next = NULL;
			current->next = new;
		}
	}
	;
	/*So this is problem, since this part is in function, it reduces to variable, which is a problem cause then it causes a syntax error*/
variable:
	meta_data data_type IDENTIFIER{
		
		member* m = getMemberNode($3);
		m->type = $2;
		m->mt = VARIABLE;
		m->metaInfo = $1;
		$$ = m;
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
		int isStructure = structureExists(sTrie,$1);

		if (isStructure){
			$$ = $1;
		} 
		else{
			printf("%s is not a defined structure nor a built in type\n", $1);
			YYABORT;
		}
	}
	;

print_format:
	    |
	print_format PRINT_SPECIFICATION 
     	;
identifier_list:
	       {$$ = NULL;}
	      |
	 identifier_list IDENTIFIER
	{

		if ($1 == NULL){

			IdentifierNode* newNode = (IdentifierNode*)malloc(sizeof(IdentifierNode));

			newNode->identifier = $2;
			$$ = newNode;
		}
		else{


			
			IdentifierNode* newNode = (IdentifierNode*)malloc(sizeof(IdentifierNode));
			newNode->identifier = $2;

			$1->next = newNode;

			$$ = $1;
			
		}
		//result should be concat of all identifiers
		
	}
	;
%%



