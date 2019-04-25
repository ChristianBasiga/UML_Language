#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "member.h"

//Could prob move this stuff around.


//Instead of enum should've just kept as string, SO MUCH easier to deal with.
typedef enum structureType { CLASS, INTERFACE} structureType;

typedef struct member member;
typedef struct structure{

	member* members;
	char* name;
	char metaInfo;
	structureType type;
	
} structure;


typedef struct structure_trie{

	struct structure_trie *children[26];
	structure* data;

} structure_trie;


//Traversiing structure identifiers.


structure_trie* getNode();
int structureExists(structure_trie* root, char* structureName);
int deleteStructure(structure_trie* root, char* structureName);
int addStructure(structure_trie* root, char* structureName, structure* s);
structure* getStructure(structure_trie* root, char* structureName);

#endif
