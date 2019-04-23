#ifdef STRUCTURE_H
#define STRUCTURE_H

#include "member.h"

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

int structureExists(structure_trie* root, char* structureName);
int addStructure(structure_trie* root, char* structureName, structureType type);
structure* getStructure(structure_trie* root, char* structureName);

#endif
