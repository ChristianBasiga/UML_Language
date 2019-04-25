#include "structure.h"
#include <stdlib.h>
#include <string.h>


int deleteStructure(structure_trie* root, char* structureName){


	//Get structure.
	
	//Will delete all members
	//I could delete each one individually, but will cause alot of repition.



	//Start negative 1 to take into account case of 1 length and 2 length strings
	return deleteUtil(root, structureName, -1);	

	//Need to remove from trie.

}

int deleteUtil(structure_trie* root, char* structureName, int index){


	//Keep going until next child doesn't exist or exist at last character.
	

	//stop at character before last.
	if (index == strlen(structureName) - 2){

		//Gets next child
		int next = structureName[index + 1];

		//only structures lower case right now
		//rework trie later
		structure_trie* nextChild = root->children[next - 97];

		//didn't exist.
		if (nextChild == NULL){

			return 0;
		}
		else{

			//First must do post order to delete members.
			//
			//otherwise delete it.
			deleteAllMembers(nextChild);
			//Since making NULL full pointer this step isn't needed.
			free(nextChild);
			root->children[next-97] = NULL;
			return 1;
		}
	}
	else{
		
		deleteUtil(root, structureName, index + 1);

	}



}


structure_trie* getNode(){


	structure_trie* newNode = (structure_trie*)malloc(sizeof(structure_trie));
	for (int i = 0; i < 26; ++i){

		newNode->children[i] = NULL;
	}

	newNode->data = NULL;
	return newNode;

}



int structureExists(structure_trie* root, char* structureName){
	return existUtil(root, structureName, 0, NULL);
}


structure* getStructure(structure_trie* root, char* structureName){


	
	structure* retrieved = NULL;
	existUtil(root, structureName, 0, &retrieved);


	return retrieved;


}

int addStructure(structure_trie* root, char* structureName, structure* s){

	return addStructureUtil(root, structureName, 0, s);
}

int addStructureUtil(structure_trie* currentNode, char* structureName, int index, structure* s){


	

	//Otherwise if reached end of input, then check content of current Node.
	if (index >= strlen(structureName)){

		if (currentNode->data == NULL){

			puts("I for sure get here");
			currentNode->data = s;
			return 1;
		}
		else
			return 0;
	}


	char next = structureName[index];
	
	//May turn this to use bst as well actually
	//in hindsight.	
	structure_trie** nextChild = &currentNode->children[next - 97];
	
	
	//If letter not used before, initialize that node.
	if (*nextChild == NULL){

		currentNode->children[next-97] = getNode();

	}

	printf("Looking at character %c, next child is %p\n", next, nextChild);
	return addStructureUtil(*nextChild, structureName, index + 1, s);

}

int existUtil(structure_trie* currentNode, char* name, int index, structure** foundNode){


	printf("%p\n", name);

	//base case out of input.
	if (index >= strlen(name)){

		puts("I happen");		
		//Check if current node  has content, then exists.
		if (currentNode != NULL){


			puts("But i do not");

			printf("Found node %s\n", currentNode->data->name);
			if (foundNode != NULL){
				*foundNode = currentNode->data;
			}
			return 1;
		}

		return 0;
	}
	if (currentNode == NULL){

		
		return 0;
	}


	char next = name[index];	
	structure_trie* nextChild = currentNode->children[next-97];
	
	
	printf("Looking at character %c, next child is %p\n", next, nextChild);
	return existUtil(nextChild, name, index + 1, foundNode);	
}
