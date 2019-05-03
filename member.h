#ifndef MEMBER_H
#define MEMBER_H
#include "structure.h"
typedef enum memberType {VARIABLE, FUNCTION} memberType;

typedef struct structure structure;
//Bst for members cause not likely to have alot of overlap
//also not nearly as much as there will be structures
//so not good for the huge space.
typedef struct member{
	struct member* left;
	struct member* right;
	char accessSpecifier;
	
	//Why'd I make this size 3? Oh right asteric instead of spelling out.
	char* metaInfo;
	char* type;
	char* name;
	memberType mt;
} member;

//LL really just a stack, if I keep reference to tail
typedef struct memberLL{

	struct memberLL* next;
	struct memberLL* prev;
	member* data;

} memberLL;



int memberExists(structure* s, char* memberName);
int addMember(structure* s, member* memberToAdd);
int deleteAllMembers(structure* s);
member* getMemberNode(char* name);

memberLL* getMembers(structure* s);

#endif
