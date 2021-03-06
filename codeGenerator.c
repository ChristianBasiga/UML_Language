#include "codeGenerator.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void writeToFile(ReadyToWrite* s);

void generateCode(structure_trie* root, relationship* relationshipGraph){

	//Parses code to ReadyToWrite struct
	

	ReadyToWriteLL* ll = NULL;
	relationship* current = relationshipGraph;

	while (current != NULL){


		ll  = (ReadyToWriteLL*)malloc(sizeof(ReadyToWriteLL));

		ll->next = NULL;

		ReadyToWrite* data = (ReadyToWrite*)malloc(sizeof(ReadyToWrite));
		
		structure* s = getStructure(root, current->identifier);

		//They can point to same string in memory.
		data->className = s->name;
		
		connection* connections = current->connections;
		int includeCount = 0;

		data->includes = connections;
		while (connections != NULL){
		
			includeCount += 1;

			connections = connections->next;
		}


		//For buffering purposes in writing files.
		data->includeCount = includeCount;
		data->members = getMembers(s);
		data->memberCount = getAmountOfMembers(s);
		ll->data = data;

		writeToFile(data);
		ll = ll->next;

		//Start thread to write to file, for now just in one go. In hindsight don't need linkedlist
		current = current->next;
		free(data);
	}


}

void splitGeneration(ReadyToWriteLL* toGenerate){

	//Splits the processing into two threads.
}

char* parseAccessSpecifier(char accessChar){

	char* specifier = NULL;


	if (accessChar == '+'){
			specifier = (char*)malloc(strlen("public:"));
			strcpy(specifier,"public:");
	}
	else if (accessChar == '-'){
			specifier = (char*)malloc(strlen("private:"));
			strcpy(specifier,"private:");
	}
	else{
			specifier = (char*)malloc(strlen("protected:"));
			strcpy(specifier, "protected:");

	}

	return specifier;

}




void addToQueue(MemberQueue* root, member* toAdd){


	if (toAdd == NULL){
		return; 
	}

	//Could just excahnge it with head,
	MemberQueue* current = root;

	while (current->next != NULL){

		
		current = current->next;
	}

	MemberQueue* newMember = (MemberQueue*)malloc(sizeof(MemberQueue));
	newMember->data = toAdd;
	current->next = newMember;

}

void writeToFile(ReadyToWrite *s){


	
	char* fileName = (char*)malloc(strlen(s->className) + 3);
	strcpy(fileName, s->className);
	strcat(fileName,".h");
	printf("file name writing is [%s]\n", fileName);
	FILE *fp = fopen(fileName, "w");

	//First write new lines equal 
	//Everytime one of them writes, increment both, but if thread both, then makes it complicated.
	//but otherwise could add a buffer between them but that would waste space.

	int lineCount  =  s->memberCount;
	lineCount += s->includeCount;
	
	//Then + 3 for name and curly braces and 2 more incase of amount of implements / inherits
	
	lineCount += 5;


	//Also needs lines for all of the includes,
	//need another structure that puts all that into one place for easy writing.


//Need public pointer and private pointer to, unless prefix everything with : lmao
//which would be stupid, but would work. But then don't actually need includes if do in java
//well needed for imports, from other packages.

	//Starting at one off from each other, buffer inbetween being number of members total.
	

	//offset in bytes not new lines, starting at 3 for class name and curly braces
	int offSet = 3;

	//Restart at beginning. 
	fseek(fp, 0,0);

	//write includes.
	
	connection* currentInclude = s->includes;

	while (currentInclude != NULL){

		int includeBuffer = strlen("#include \"\"") + strlen(currentInclude->identifier);

		char* includeString = (char*)malloc(includeBuffer);
	
		strcat(includeString,"include \"\"");
		strcat(includeString, currentInclude->identifier);

		//Catting the literal should should give it the null terminator.
		strcat(includeString, "\n");
		currentInclude = currentInclude->next;
		fputs(includeString, fp);
		


		offSet += strlen(includeString);

		free(includeString);
	}

	char* className = (char*)malloc(strlen("class ") + strlen(s->className) +3);
	strcpy(className, "class ");
	className = strcat(className,s->className);
	className = strcat(className, "\n");
	offSet += strlen(className);	

	fputs(className, fp);

	free(className);
		
	fputs("{\n",fp);


	//TODO: start using registers instead of main memory.

	FILE *functionWriter = fp;
	FILE *variableWriter = fp;


/*
	for(int i = 0; i < lineCount + offSet; ++i){


		fputc('\n', fp);
	}
*/



	//fseek(functionWriter, offSet + 1, 0);	
	//I wonder if fseek goes over new lines
//	fseek(variableWriter, offSet, 0);

	memberLL* members = s->members;

	
	//If i do get members, I'm essentially traversing it twice though, could just do dfs, then do this at each stuff, makes
	//writing into file harder though.
	while (members != NULL){



		int memberType = members->data->mt;



		//Since used so often during these, I should cache this to avoid constant releases of something so small.
		char* accessSpecifier = parseAccessSpecifier(members->data->accessSpecifier);

		//Depending on type, write using respecting fp.
		
		//+ 5 for semicolon and new line, and spaces
		int buffer = strlen(members->data->type) + /*strlen(members->data->metaInfo) +*/ strlen(members->data->name) + 
		strlen(accessSpecifier) + 5;

		
		if (memberType == FUNCTION){


			//Then extra buffer for paranthesis and  parameters.
			//prob cause of this buffer space.
			//buffer += 15;

			
			buffer += 5;

			//Could have dest for header then dest for params.
			char* dest = (char*)malloc(buffer);
			strcpy(dest,"\t");
			strcat(dest, accessSpecifier);
			strcat(dest, " ");
//			strcat(dest, members->data->metaInfo);

			strcat(dest, members->data->type);
			strcat(dest, " ");

			strcat(dest, members->data->name);
			strcat(dest,"(");

			fputs(dest,fp);
		
			//Gotta refactor alot cause is tedious like this.

			
			member* parameters = members->data->parameters;

			if (parameters != NULL){
				
				//If buffer is issue would need to run through params twice to get exact cunt
			
				MemberQueue* root = (MemberQueue*)malloc(sizeof(MemberQueue));
				root->data = parameters;

				root->next = NULL;

				//Then gotta write parameters.
				//Bfs of parameters.
				while (root != NULL){
	
		

					//Actually can put back to dst stuff had before, as long as strcpy nothing
					//to dest first it won't have previous entry stuff. It works as is so some extra I/o calls
					//not big deal.	
					char* parameterString = (char*)malloc(strlen(root->data->type) + strlen(root->data->name) + 2);	
					//Write root's information.
					strcpy(parameterString,"");
					strcat(parameterString, root->data->type);
					strcat(parameterString, " ");
					strcat(parameterString, root->data->name);
					
					printf("Current parameter string [%s]\n", parameterString);

					//Then pop off root
					addToQueue(root, root->data->left);
					addToQueue(root, root->data->right);
				
					if (root->next != NULL)
						strcat(parameterString,",");

					MemberQueue* prevRoot = root;
					root = root->next;
		
					//More I/O write this way instead of huge batches, but if it fixes issue.
					//it fixed issue
					fputs(parameterString,fp);
					free(parameterString);
					free(prevRoot);

				}

			}
			fputs(")\n", fp);

			
		//	fputs(cutDown, fp);
			free(dest);
							

		}
		else if (memberType == VARIABLE){


			char* dest = (char*)malloc(buffer + 1);

			strcat(dest,"\t");
			strcat(dest, accessSpecifier);
			strcat(dest, " ");
			strcat(dest, members->data->type);
			strcat(dest, " ");
			strcat(dest, members->data->name);
			strcat(dest,";\n");
		
			//Then write to file.
			fputs(dest,fp);
		
			//Then move functionPointer down a line.
			//fseek(functionWriter,strlen(dest), 2);
			//fseek(fp, strlen(dest), 2);
			free(dest);
		}

		

		members = members->next;
		///Could be optimized alot.
		free(accessSpecifier);
	}	


	//from 1 off end of file.
	fputs("};\n", fp);

	fclose(fp);
}
