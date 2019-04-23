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
	char metaInfo[3];
	char* type;
	char* name;
	memberType mt;
} member;



//Traversing member identifiers per structure.
//Structure instead of structure name to avoid circular dependancy.
//I mean having it still makes it require lmao.
int memberExists(structure* s, char* memberName);
int addMember(structure* s, member* memberToAdd);
member* getMemberNode(char* name);

#endif
