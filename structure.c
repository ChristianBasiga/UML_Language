#include "structure.h"

int structureExists(structure_trie* root, char* structureName){
	return existUtil(root, structureName[0], 0);
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





	existUtil(root, structureName, 0, &retrieved);


	return retrieved;


}

int addStructure(structure_trie* root, char* structureName, int index, structureType type){





	if (next == '\0'){

			
		//Check if current node  has content, then exists.
		if (currentNode->data != NULL){
			//Failed to add.
			return 0;
		}
		else{

			currentNode->data = (structure*)malloc(sizeof(structure));

			currentNode->data->type = type;


		}
	}
}

int existUtil(structure_trie* currentNode, char* name, int index, structure** foundNode = NULL){


	char next = name[index];

	//base case out of input.
	if (next == '\0'){

			
		//Check if current node  has content, then exists.
		if (currentNode->data != NULL){

			if (foundNode != NULL){

				*foundNode = currentNode->data;
			}
			return 1;
		}

		return 0;
	}
	
	//ooo, wait but also allows numbers, so maybe not trie?
	//need to rethink structure for this
	//I mean for structures unlikely that includes numbers though honestly.
	return existUtil(currentNode->children[next - 97], name, index + 1, foundNode);	
}
