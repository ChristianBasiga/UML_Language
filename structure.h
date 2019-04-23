#ifdef STRUCTURE_H
#define STRUCTURE_H

#include "members.h"

//Could prob move this stuff around.

typedef enum { CLASS, INTERFACE} structureType;

typedef struct{

	member* members;
	char metaInfo;
	structureType type;
	
} structure;


typedef struct{

	structure_trie children[26];
	structure* data;

} structure_trie;


//Traversiing structure identifiers.

int structureExists(char* structureName);
int addStructure(char* structureName, structureType type);
structure* getStructure(char* structureName);

#endif
