#ifndef NODES_H_
#define NODES_H_

#include "global.h"
#include "eDataTypes.h"

typedef union {
	void* aVoid;
	char* aString;
	char  aChar;
	char  aChars[4];
	BYTE  aByte;
	BYTE  aBytes[4];
	short aShort;
	short aShorts[2];
	WORD  aWord;
	WORD  aWords[2];
	int   aInt;
	DWORD aDword;
	struct sXMLTNode* sXMLTNode;
} uUniValue;

typedef struct sXMLTNode {
	int UniqueID;
	DWORD NodeMark;
	eDataTypes DataType;
	uUniValue Value;
	char* Name;
	struct sXMLTNode* FirstSon;
	struct sXMLTNode* Parent;
	struct sXMLTNode* Next;
} sXMLTNode;

typedef struct sUNIQItem {
	int UniqueID;
	int Used;
	struct sXMLTNode* Node;
	struct sUNIQItem* Next;
} sUNIQItem;

extern uUniValue UNIZEROVALUE;
extern sXMLTNode AllData;
extern void printNodeTree(sXMLTNode* aNode, int level, int maxl, int showID, int showName, int showValue);
extern sXMLTNode* addNodeAndValue(sXMLTNode* aParent, char* aName, eDataTypes aDataType, uUniValue aValue);
extern sXMLTNode* findNode(sXMLTNode* aNode, ... ); // NULL as last argument is a MUST!!!
extern void killNodeTree(sXMLTNode* aNode);
extern void killUniques(void);
extern void moveNodeTo(sXMLTNode* aParent, sXMLTNode* aNode);

#endif /* NODES_H_ */
