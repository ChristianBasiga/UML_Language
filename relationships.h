#ifndef RELATIONSHIPS_H
#define	RELATIONSHIPS_H


//Now because don't have parents stuck within classes, but independanlty can be explicit and verbose in relationships
//Also, I'll have ll of identifiers of structures for iterating through all of them
//so now will just be adjacency list for graph.
typedef enum relationship_type{AGGREGATION, COMPOSITION, INHERITS} relationship_type;



//What I'll store here is identifiers of structures in relationships
//not structures themselves, much easier for taking commands to form relationships.
//has to be structure to structure, but will make that check in adding rather than here
//so guess eitherway gotta iterate through structures for validity checking.


typedef struct connection{

	char* identifier;
	relationship_type type;
	struct connection* next;

} connection;

//So will have linked lists  of linked lists. To represent graph.
//Then in parsing these to generate code and uml, will do bfs.
//hmm unique identifiers, I could make it an array, but problem still persists of idk how many relationships there will be
//bst is alternative, but for getting done now, ll of ll fine, another problem with this though is identifiers not unique with it not being BST.
//for final project purposes, ll of ll, but for future turn this to bst of bst, where bst for connection is identifier and type, actually BST of LL, cause type means there are connections of same identifier
typedef struct relationship{

	char* identifier;
	//so next in linkedlists of relationships
	struct relationship* next;

	//Then linked list of connections it has
	connection* connections;
	

} relationship;





relationship* getRelationship(char* identifier);
relationship_type parseType(char* type);
char* typeToString(relationship_type type);
connection* getConnections(relationship* graph, char* identifier);
int addToGraph(relationship* root, relationship* toAdd);
int addRelationship(relationship* root, char* from, char* to, relationship_type type);
//Need type in removal too as multiple relationships can be made.
int removeRelationship(relationship* root, char* from, char* to, relationship_type type);

#endif
