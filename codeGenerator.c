#include "codeGenerator.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void writeHeaderFile(ReadyToWrite* s);

void generateCode(structure_trie* root, relationship* relationshipGraph){

	//Parses code to ReadyToWrite struct

	//Could make a main to actually test compilation.
	//then make it have includes of each one. 	

	relationship* current = relationshipGraph;

	//Reroute error stream to logs
	system("mkdir output 2> logs");	
	//Make a main.
	FILE *fp = fopen("output/main.cpp","w");

	//First thing to do is write in all of the includes, then generate corresponding .h files.
	char* include = "#include \"";
	while (current != NULL){


		printf("Looking at relationships of %s\n", current->identifier);

		ReadyToWrite* data = (ReadyToWrite*)malloc(sizeof(ReadyToWrite));
		
		structure* s = getStructure(root, current->identifier);

		//They can point to same string in memory.
		data->className = s->name;
		
		char* toInclude = (char*)malloc(strlen(include) + strlen(s->name) + 2);
		strcpy(toInclude,include);
		strcat(toInclude,s->name);
		strcat(toInclude,".h");
		strcat(toInclude,"\"\n");
		fputs(toInclude, fp);
		free(toInclude);
		
		
		connection* connections = current->connections;
		int includeCount = 0;

		data->includes = connections;

		//No longer needed atm, but for organizing methods and methods
		//need to create buffer and in that instance this is needed.
	/*	while (connections != NULL){
		
			includeCount += 1;
			printf("connection %s\n", connections->identifier);
			connections = connections->next;
		}

		*/
		//For buffering purposes in writing files.
		data->includeCount = includeCount;
		data->members = getMembers(s);
		data->memberCount = getAmountOfMembers(s);

		writeHeaderFile(data);

		//Start thread to write to file, for now just in one go. In hindsight don't need linkedlist
		free(data);
	}

	char* mainString = "int main()\n";
	
	char* mainFunction = (char*)malloc(strlen(mainString) + 3);

	strcpy(mainFunction, mainString);

	strcat(mainFunction, "{\n}");
	free(mainFunction);


	
	//Then inbetween here. Maybe add stuff to main
	//Maybe queue up sequence diagrams till after all done or let c++ compiler deal with that.
	
	fputs(mainFunction, fp);
	
	fclose(fp);


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

void writeHeaderFile(ReadyToWrite *s){


	char* fileName = (char*)malloc(strlen("output/") + strlen(s->className) + 3);
	strcpy(fileName,"output/");
	strcat(fileName, s->className);
	strcat(fileName,".h");
	printf("file name writing is [%s]\n", fileName);

	FILE *fp = fopen(fileName, "w");

	static char* ifndef = "#ifndef ";
	static char* def = "#define ";
	static char* endif = "#endif ";

	char* ifndefThis = (char*)malloc(strlen(ifndef) + strlen(s->className) + 2);
	strcpy(ifndefThis, ifndef);
	strcat(ifndefThis, s->className);
	strcat(ifndefThis,"_h\n");

	fputs(ifndefThis, fp);
	free(ifndefThis);
	ifndefThis = (char*)malloc(strlen(def) + strlen(s->className) + 2);
	
	strcpy(ifndefThis, def);
	strcat(ifndefThis, s->className);
	strcat(ifndefThis,"_h\n");
	
	fputs(ifndefThis,fp);
	free(ifndefThis);


	//write includes.
	
	connection* currentInclude = s->includes;
	
	//Got it working with includes for other classes
	//but need to differentate primitives and std stuff so that I can include those respectively.
	//many additions can be amde to this.
	
	char* include = "#include \"";
	

	while (currentInclude != NULL){

		int includeBuffer = strlen(include) + strlen(currentInclude->identifier) + 2;

		char* includeString = (char*)malloc(includeBuffer);
	
		strcpy(includeString, include);
		strcat(includeString, currentInclude->identifier);
		
		//Catting the literal should should give it the null terminator.
		strcat(includeString, ".h\"\n");
		
		fputs(includeString, fp);
		



		currentInclude = currentInclude->next;
		free(includeString);
	}

	char* className = (char*)malloc(strlen("class ") + strlen(s->className) +3);
	strcpy(className, "class ");
	className = strcat(className,s->className);
	className = strcat(className, "\n");

	fputs(className, fp);

	free(className);
		
	fputs("{\n",fp);


/*
	FILE *functionWriter = fp;
	FILE *variableWriter = fp;

	Originally for organzing them, by seeking into different points on file.

	for(int i = 0; i < lineCount + offSet; ++i){


		fputc('\n', fp);
	}
*/

	memberLL* members = s->members;
	
	while (members != NULL){

		printf("member looking at %s\n", members->data->name);

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
					
					fputs(parameterString,fp);
					free(parameterString);
					free(prevRoot);

				}

			}
			
			fputs(");\n", fp);
			free(dest);
							

		}
		else if (memberType == VARIABLE){


			char* dest = (char*)malloc(buffer + 5);

			strcpy(dest,"\t");
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
	fputs(endif, fp);	

	fclose(fp);
}
