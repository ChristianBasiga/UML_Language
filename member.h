#ifdef MEMBER_H
#define MEMBER_H

typedef enum {VARIABLE, FUNCTION} memberType;

//Bst for members cause not likely to have alot of overlap
//also not nearly as much as there will be structures
//so not good for the huge space.
typedef struct{
	member* left;
	member* right;
	char accessSpecifier;
	char metaInfo[3];
	char* type;
	char* name;
	memberType mt;
} member;



//Traversing member identifiers per structure.
int memberExists(char* structureName, char* memberName);
int addMember(char* structureName, member* memberToAdd);


#endif
