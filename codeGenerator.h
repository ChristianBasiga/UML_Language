#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "structure.h"
#include "relationships.h"

typedef struct structure structure;
typedef struct relationship relationship;


/*To have
 * Appropriate Register Allocation
 * Threadable methods for generating code, if there are dependancies between classes, can't be made concourrently, but classes with no connections can be made concurrently
 * Atleast three registers, one for doing classes that can be done independantly, then two for holding a class and its dependancies
 *
 * Code Generation:
 * 	Can have two queues, one for isolated classes, then one for classes with relationships, so for latter one without parent have priority.
 * 	Doesn't actually matter cause test dependancy in syntax phase.
 *
 * 	Since using C++ compiler afterward don't need to generate assembly code,
 *
 * 	methods:
 * 		Get passed in full list of all structures and relationship graph.
 * 		Split between those without relationships and those with into separate threads.
 * 		Queue for those without just one go.
 * 		Those with, recursively construct classes.
 *
 * */



typedef struct ReadyToWrite{


	char* className;
	//Would basically be connections.
	connection* includes;
	int includeCount;
	//Alot of repeated work, like pretty fucking bad.
	memberLL* members;
	int memberCount;


} ReadyToWrite;

typedef struct MemberQueue{


	struct MemberQueue* next;
	member* data;

} MemberQueue;



//Maybe struct for the parsed structure, like separate into methods and variables
//or maybe just create two file pointers for each, then go forward on both each time.


//Interface calls.
void generateCode(structure_trie* root, relationship* relationshipGraph);



#endif
