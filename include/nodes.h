#ifndef NODES_H_
#define NODES_H_

#include "global.h"

#define NODES_SEPARATOR_LIST " \\/;:,.>"

#include "nodes.enums.h"

typedef union {
	void* pVoid;
	char* pString;
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
	sIPv4 aIPv4;
	struct ssXNode* pXNode;
} uUniValue;

typedef void (*xCleaner)(void*);

typedef struct ssXNode {
	DWORD NodeMark;					// +4
	DWORD UniqueID;					// +4
	eDataTypes DataType;			// +4
	DWORD AutoClean:1;				// +4
	DWORD ssXNReserved:31;
	uUniValue Value;				// PtrSize
	char* Name;						// PtrSize
	struct ssXNode* FirstSon;		// PtrSize
	struct ssXNode* Parent;			// PtrSize
	struct ssXNode* Next;			// PtrSize
} sXNode;

typedef struct ssUNIQItem {
	DWORD UniqueID;					// +4
	BYTE Used;						// +4
	BYTE ssUNIReserved[3];
	struct ssXNode* Node;			// PtrSize
	struct ssUNIQItem* Next;		// PtrSize
} sUNIQItem;

extern uUniValue UNIZEROVALUE;
extern sXNode AllData;
extern void nodesConsoleTree(sXNode* aNode, int level, int maxl, int showID, int showName, int showValue);
extern sXNode* nodesAddNodeAndValue(sXNode* aParent, char* aName, eDataTypes aDataType, uUniValue aValue, DWORD autoClean);
extern sXNode* nodesFindNode(sXNode* aNode, ... ); // NULL as last argument is a MUST!!!
extern void nodesKillTree(sXNode* aNode);
extern void nodesKillUniques(void);
extern void nodesMoveNodeTo(sXNode* aParent, sXNode* aNode);

#endif /* NODES_H_ */
