#include "member.h"
#include <string.h>
#include <stdlib.h>


member* getMemberNode(char* name){

	member* newNode = (member*)malloc(sizeof(member));
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->name = (char*)malloc(strlen(name));
	newNode->parameters = NULL;
	strcpy(newNode->name, name);


	return newNode;
}
int memberExistsUtil(member* current, char* memberName){


	if (current == NULL){

		return 0;
	}

	int comparison = strcmp(memberName, current->name);

	if (comparison == 0){

		return 1;
	}
	else if (comparison < 0){

		return memberExistsUtil(current->left, memberName);
	}
	else{
		return memberExistsUtil(current->right, memberName);
	}


}	

int addMemberUtil(member* current, member* memberToAdd){



	if (current == NULL){


		return 0;
	}

	char* memberName = memberToAdd->name;
	
	int comparison = strcmp(memberName, current->name);

	if (comparison == 0){

		//Already existed
		return 0;
	}
	else if (comparison < 0){

		if (current->left == NULL){

			current->left = memberToAdd;
			return 1;
		}
		else{
			return addMemberUtil(current->left, memberToAdd);
		}

	}
	else{

		if (current->right == NULL){

			current->right = memberToAdd;
			return 1;
		}
		else{
			return addMemberUtil(current->right, memberToAdd);
		}
	}


}	

int addParameter(member* function, member* param)
{


	if (function->parameters == NULL){
		function->parameters = param;
		return 1;
	}
    
	return addMemberUtil(function->parameters, param);
}



memberLL*  getToTail(memberLL* ll){

	memberLL* current = ll;
	while (current->next != NULL){

		current = current->next;
	
	}
	return current;

}

void freeMember(member* m){

	if (m == NULL) return;

	free(m->metaInfo);
	free(m->type);
	free(m->name);
	
	//then recursively free parameters	
	freeMember(m->parameters->left);
	freeMember(m->parameters->right);

	free(m);
}

void dfsMembers(member* root, memberLL* ll){


	if (root == NULL){
		
		return;
	}

	memberLL* llNode = (memberLL*)malloc(sizeof(memberLL));
	llNode->next = NULL;
	llNode->data = root;

	//Better if I just make my own stack instead of doign recursively
	//cause of extra need to go all the way to tail each time.
	
	//But then how do I set it for rest.
	ll->next = llNode;	


	//Getting tail is extra iterations not needed
	///will make stack later to replace

	memberLL* tail = getToTail(ll);
	dfsMembers(root->left, tail);

	tail = getToTail(ll);
	dfsMembers(root->right, tail);


}
memberLL* getMembers(structure* s){
	


	//Head for this has no actual data.
	memberLL* ll = (memberLL*)malloc(sizeof(memberLL));
	ll->next = NULL;
		
		
	member* root = s->members;
	dfsMembers(root, ll);

		

	return ll->next;
}



int memberExists(structure* s, char* memberName){


	return memberExistsUtil(s->members, memberName);
}







int addMember(structure* s, member* memberToAdd){

	if (s->members == NULL){
		s->members = memberToAdd;
		return 1;
	}
	else
		return addMemberUtil(s->members, memberToAdd);
}

//Need to also remove, good for demo purposes.
//remove instead of create. but same everything else.

void deleteAllUtil(member* root){

	if (root == NULL) return;

	deleteAllUtil(root->left);
	deleteAllUtil(root->right);
	free(root);
}


int deleteAllMembers(structure* s){


	deleteAllUtil(s->members);
	s->members = NULL;	
}



int removeMemberUtil(member* current, member* memberToRemove){

	if (current == NULL){


		return 0;
	}

	char* memberName = memberToRemove->name;

	int comparison = strcmp(memberName, current->name);
	int found;

	//Will do this differently.

	if (comparison == 0){

		//found it.
		return 1;
	}
	else if (comparison < 0 ){



		found = removeMemberUtil(current->left, memberToRemove);

		if (found == 1){

			free(current->left);
			current->left = NULL;
			return 2;
		}

	}
	else{
		found = removeMemberUtil(current->right, memberToRemove);
		
		if (found == 1){

			free(current->right);
			current->right = NULL;
			return 1;
		}

	}
}

int removeMember(structure* s, member* memberToRemove){


	if (s->members == NULL){

		return 0;
	}
	

	member* root = s->members;

	//If match root.
	if (strcmp(memberToRemove->name, root->name) == 0){


		//Check left and right to set new root.

		member* temp = NULL;
		if (root->left != NULL){

			temp = root->left;

		}

		if (root->right != NULL){

			
			//if left also active make it new root and attack righ to it.
			if (temp != NULL){

				temp->right = root->right;
			}
			else{
				temp = root->right;
			}



		}

		free(root);

		//Then if had no children it is now NULL
		s->members = temp;
		return 1;

	}

	return removeMemberUtil(s->members, memberToRemove);
	

}



