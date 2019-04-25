#include "structure.h"
#include <stdlib.h>




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

			
		//Check if current node  has content, then exists.
		if (currentNode != NULL){



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
