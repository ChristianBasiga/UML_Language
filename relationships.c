#include "relationships.h"
#include <string.h>



relationship_type parseType(char* type){

	relationship_type rType = INHERITS;

	if (strcmp(type,"aggregation") == 0){
		rType = AGGREGATION;
	}
	else if (strcmp(type,"composition") == 0){

		rType = COMPOSITION;
	}

	return rType;

}

char* typeToString(relationship_type type){

	char* parsed = NULL;
	if (type == INHERITS){
	
		char inherits[] = "inherits";
		parsed = (char*)malloc(strlen(inherits));
		strcpy(parsed,inherits);
	}

	return parsed;

}

connection* getConnections(relationship* graph, char *identifier){


	relationship* current = graph;

	int matched = 0;
	while (current!= NULL){

		int matched = strcmp(identifier, current->identifier) == 0;
		if (matched) {
			
			return current->connections;
		}
		current = current->next;

	}

	return NULL;


}

relationship*  getRelationship(char* identifier){

	relationship* newRelationship = (relationship*)malloc(sizeof(relationship));

	//Could copy it, but they really all pointing to same identifier
	//if any turns to garbage, all should be gone.
	newRelationship->identifier = identifier;
	newRelationship->connections = NULL;

	return newRelationship;

}

int addToGraph(relationship* root, relationship* toAdd){


	relationship* current = root;

	while (current->next != NULL){
		current = current->next;
	}	

	current->next = toAdd;
	return 1;

}

connection* makeConnection(char* to, relationship_type type){

	connection* newConnection = (connection*)malloc(sizeof(connection));
	newConnection->type = type;		
	newConnection->identifier = (char*)malloc(strlen(to));
	newConnection->next = NULL;
	strcpy(newConnection->identifier, to);

	return newConnection;
}

int addRelationship(relationship* root, char* from, char* to, relationship_type type){


	//So from root traverse next till find the from,
	//then from that relationship traverse connections till find the to.


	relationship* current = root;
	int madeConnection = 0;
	while (current != NULL && !madeConnection){
		
		
		int comparison = strcmp(current->identifier, from);

		//To speed up relationships creation and deletion.
		//could be bsts of bsts, but how many relationships will there be really.
		//Just note that is optimization could make.
		//If do that have to have 2 functions essentially doing the same
		//for finding relationships and connections, so recursively find relationship
		//then from there add or recursively find connection.	
		
		if (comparison == 0){

			//If found from, now add the to.

			connection* newConnection = makeConnection(to, type);

			connection* currentConnection = current->connections;

			if (currentConnection == NULL){

				current->connections = newConnection;
				madeConnection = 1;
			}

			else{

				while (currentConnection->next != NULL){

					currentConnection = currentConnection->next;
				}

				currentConnection->next = newConnection;
				madeConnection = 1;
			}



		}		

		current = current->next;

	}

	return madeConnection;

}

int removeRelationship(relationship* root, char* from, char* to, relationship_type type){


	relationship* current = root;
	int removed = 0;
	
	while (current != NULL && !removed){


		int comparison = strcmp(current->identifier, from);

		if (comparison == 0){

			
			connection* currentConnection = root->connections;
			//What's worst though, mallocing a fake connection, or a simple check.
			connection* fake = (connection*)malloc(sizeof(connection));
			fake->next = currentConnection;
			connection* prev = fake;
			while (currentConnection != NULL){

		
				if (strcmp(currentConnection->identifier, to) == 0 && currentConnection->type == type){

					//Instead of having check will make fake head.
					//cause if we have 1->2 and delete 1, there is no prev set to restablish connection. But with fake head:
					//fake->1->2, deleting 1, will have fake->2, then make connections equal to fake->next

					/*
					if (prev == NULL){

						root->connections = currentConnection->next;
					}
					else*/ 
					prev->next = currentConnection->next;

					//Hmm I also shouldn't be restoring the same strings everywhere.
					//should all point to same one, but that will make deleting harder cause gotta implement reference counting.
					free(currentConnection->identifier);
					free(currentConnection);
					removed = 1;
					break;
				}

				prev = currentConnection;
				currentConnection = currentConnection->next;
				

			}


			root->connections = fake->next;
			free(fake);
			
				

		}
		current = current->next;


		
	}

	return removed;


}
