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


	//Could just excahnge it with head,
	MemberQueue* current = root;

	while (current->next != NULL){

		
		current = current->next;
	}

	MemberQueue* newMember = (MemberQueue*)malloc(sizeof(MemberQueue));

	current->next = newMember;

}

void writeToFile(ReadyToWrite *s){

	char* fileName = (char*)malloc(strlen(s->className) + 3);
	strcat(fileName, s->className);
	strcat(fileName,".h");
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

	for(int i = 0; i < lineCount; ++i){


		fputc('\n', fp);
	}

//Need public pointer and private pointer to, unless prefix everything with : lmao
//which would be stupid, but would work. But then don't actually need includes if do in java
//well needed for imports, from other packages.

	//Starting at one off from each other, buffer inbetween being number of members total.
	
	//Reset 
	FILE *functionWriter = fopen(fileName,"w");

	FILE *variableWriter = fopen(fileName,"w");

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

	char* className = (char*)malloc(strlen("class ") + strlen(s->className)) + 3;
	char* originalPtr = className;
	strcat(className, "class ");
	strcat(className,s->className);
	strcat(className, "\n");



	fputs(className, fp);

//	free(originalPtr);
		
	fputs("{\n",fp);


	//TODO: start using registers instead of main memory.



	//I wonder if fseek goes over new lines
	fseek(variableWriter, 0, 0);
	fseek(functionWriter, offSet + 1, 0);

	memberLL* members = s->members;


	//If i do get members, I'm essentially traversing it twice though, could just do dfs, then do this at each stuff, makes
	//writing into file harder though.
	while (members != NULL){


		int memberType = members->data->mt;




		//Since used so often during these, I should cache this to avoid constant releases of something so small.
		char* accessSpecifier = parseAccessSpecifier(members->data->accessSpecifier);

		//Depending on type, write using respecting fp.
		
		//+ 5 for semicolon and new line, and spaces
		int buffer = strlen(members->data->type) + strlen(members->data->metaInfo) + strlen(members->data->name) + 
		strlen(accessSpecifier) + 5;

		
		if (memberType == FUNCTION){


			//Then extra buffer for paranthesis and  parameters.
			buffer += 2 + 256;


			char* dest = (char*)malloc(buffer);

			strcat(dest, accessSpecifier);
			strcat(dest, " ");
			strcat(dest, members->data->metaInfo);

			strcat(dest, members->data->type);
			strcat(dest, " ");

			strcat(dest, members->data->name);
			strcat(dest,"(");

			//Gotta refactor alot cause is tedious like this.
			member* parameters = members->data->parameters;

			member* current = parameters;
			member* nextInQueue;
			
			MemberQueue* root = (MemberQueue*)malloc(sizeof(MemberQueue));
			root->data = parameters;

			MemberQueue* next = root;


			//Then gotta write parameters.
			//Bfs of parameters.
			while (root != NULL){

				
				//Write root's information.
				strcat(dest, root->data->type);
				strcat(dest, root->data->name);

				if (root->next != NULL)
					strcat(dest,",");

				//Then pop off root
				addToQueue(root, root->data->left);
				addToQueue(root, root->data->right);

				MemberQueue* prevRoot = root;
				root = root->next;
				//Should create a free function for members
				freeMember(prevRoot->data);
				free(prevRoot);

			}

			strcat(dest,");\n");
			
			fputs(dest, functionWriter);
			free(dest);
							

		}
		else if (memberType == VARIABLE){


			char* dest = (char*)malloc(buffer);

			strcat(dest, accessSpecifier);
			strcat(dest, " ");
			strcat(dest, members->data->type);
			strcat(dest, " ");
			strcat(dest, members->data->name);
			strcat(dest,";\n");
		
			//Then write to file.
			fputs(dest,fp);
		
			//Then move functionPointer down a line.
			fseek(functionWriter,1, 2);
			fseek(fp, strlen(dest), 2);
			free(dest);
		}

		

		members = members->next;
		///Could be optimized alot.
		free(accessSpecifier);
	}	

	fclose(functionWriter);
	fclose(variableWriter);

	//from 1 off end of file.
	fputs("}\n", fp);

	fclose(fp);
}
