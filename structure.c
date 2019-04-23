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
	return existUtil(root, structureName[0], 0, NULL);
}


structure* getStructure(structure_trie* root, char* structureName){


	
	//In future prob copy it instead, cause deleting will be issue.
	//Now main question, how do I iterate through all of the trie?
	//Like yeah DFS, but what am I using for input?
	//Less and less is using trie an actual good idea.
	//Yeah, I would have to fucking store the keys themselves.
	//I mean guess fine, for most purposes other than getting everything is still O(26)
	//but when get everything will be O(n*26); So along with this a linked list.
	//then only add to link list of successful insert.
	structure* retrieved = NULL;



	printf("Structure name looking for %s\n", structureName);
	printf("root passing in %p\n", root);
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
		
	structure_trie** nextChild = &currentNode->children[next - 97];
	
	
	//If letter not used before, initialize that node.
	if (*nextChild == NULL){

		currentNode->children[next-97] = getNode();

	}

	printf("Looking at character %c, next child is %p\n", next, nextChild);
	return addStructureUtil(*nextChild, structureName, index + 1, s);

}

int existUtil(structure_trie* currentNode, char* name, int index, structure** foundNode){




	//base case out of input.
	if (index >= strlen(name)){

			
		//Check if current node  has content, then exists.
		if (currentNode != NULL){


			printf("Found node %s\n", currentNode->data->name);
			*foundNode = currentNode->data;
			
			return 1;
		}

		return 0;
	}
	if (currentNode == NULL){

		
		puts("here?");
		return 0;
	}
	//Then make sure children aren't NULL.

	char next = name[index];	
	structure_trie* nextChild = currentNode->children[next-97];

	printf("Looking at character %c, next child is %p\n", next, nextChild);
	//ooo, wait but also allows numbers, so maybe not trie?
	//need to rethink structure for this
	//I mean for structures unlikely that includes numbers though honestly.
	return existUtil(nextChild, name, index + 1, foundNode);	
}
