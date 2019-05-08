#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "member.h"

//Could prob move this stuff around.


//Instead of enum should've just kept as string, SO MUCH easier to deal with.
typedef enum structureType { CLASS, INTERFACE} structureType;
//In hindsight the parents structure type determines relationship type ie: implements or inherits, atleast for code.
//but maybe also have stuff like decorates, aggregation, composition, etc more specific stuff.
//not sure how I would even decorate vs adaptation in terms of just declaring with no implementation, too specific.





typedef struct member member;

//Actually should relationships be in here? Or own separate struct, and structure just has stuff structure needs
//and is then used
typedef struct structure{

	member* members;
	char* name;
	char metaInfo;
	structureType type;
	
} structure;

//Along with identifier, need type of relationship has to structure to generate correct code.


typedef struct structure_trie{

	//Change this to include lower and upper case as well as numbers
	//basically would have to remove array way instead
	//and either have trie of linked lists as children or change this
	//to BST.
	struct structure_trie *children[26];
	structure* data;

} structure_trie;


//Traversiing structure identifiers.


structure_trie* getNode();
int structureExists(structure_trie* root, char* structureName);
int deleteStructure(structure_trie* root, char* structureName);
int addStructure(structure_trie* root, char* structureName, structure* s);
int getAmountOfMembers(structure* s);
structure* getStructure(structure_trie* root, char* structureName);

#endif
