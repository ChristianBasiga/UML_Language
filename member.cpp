#include "member.h"
#include <string.h>


int memberExists(structure* s, char* memberName){


	return memberExistsUtil(s->members, memberName);
}



int memberExistsUtil(member* current, char* memberName){


	if (current == NULL){

		return 0;
	}

	int comparison = strcmp(current->name, memberName);

	else if (comparison == 0){

		return 1;
	}
	else if (comparison < 0){

		return memberExistsUtil(current->left, memberName);
	}
	else{
		return memberExistsUtil(current->right, memberName);
	}


}	



int addMember(structure* s, member* memberToAdd){
	return addMemberUtil(s->members, memberToAdd);
}

int addMemberUtil(member* current, member* memberToAdd){


	if (current == NULL){

		return 0;
	}

	int comparison = strcmp(current->name, memberName);

	else if (comparison == 0){

		return 1;
	}
	else if (comparison < 0){

		if (current->left == NULL){

			current->left = memberToAdd;
			return 1;
		}
		else{
			return memberExistsUtil(current->left, memberName);
		}

	}
	else{

		if (current->right == NULL){

			current->right = memberToAdd;
			return 1;
		}
		else{
			return memberExistsUtil(current->right, memberName);
		}
	}


}	
