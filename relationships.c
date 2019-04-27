#include "relationships.h"
#include <string.h>

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
			connection* newConnection = (connections*)malloc(sizeof(connections));
			newConnection->type = type;
			newConnection->identifier = (char*)malloc(strlen(to));
			strcpy(newConnection->identifier, to);

			connection* currentConnection = current->connections;

			if (currentConnection == NULL){

				current->connections = newConnection;
				newConnection->next = NULL;
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
