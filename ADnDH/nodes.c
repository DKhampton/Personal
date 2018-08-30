
#include <stdarg.h>
#include <stdlib.h>

#include "global.h"

#define XENUM_IMPLEMENT_MODE
#include "eDataTypes.h"

#include "nodes.h"
#include "character.h"

#define OFFSETTAB (1)
#define VALUE_STRING_LEN (64)
#define ALL_ACTIVE_DATA "AllLoadedData"
#define NODE_MARK (0xBABE4DAD)

uUniValue UNIZEROVALUE = { .aInt = 0 };
sXMLTNode AllData = { 0, NODE_MARK, eDTVoid, { .aVoid = NULL } , ALL_ACTIVE_DATA, NULL, NULL, NULL }; //NULL,
sUNIQItem AllUniques = { 0, 1, &AllData };

void killUniques(void) {
	sUNIQItem* aItem = &AllUniques;
	sUNIQItem* aNext = aItem->Next;
	while (aNext) {
		aItem = aNext; aNext = aItem->Next; xFree(aItem);
	};
}

static sUNIQItem* findUniqueID(int uID) {
	sUNIQItem* aItem;
	for (aItem = &AllUniques; aItem && (aItem->UniqueID != uID); aItem = aItem->Next);
	return aItem;
}

/*
static void findUniqueIDAndRemoveRef(int uID) {
	sUNIQItem* aItem;
	if ((aItem = findUniqueID(uID))) { if (aItem->Used < 2) { XDebug("No Reference to remove"); } else { aItem->Used--; } }
}
*/

static void findAndFreeUniqueID(int uID) {
	sUNIQItem* aItem;
	if ((aItem = findUniqueID(uID))) { if (--aItem->Used) { xDebug("Cross Reference left"); } aItem->Used = 0; }
	else { xDebug("uID was not found"); }
}

static int findAndBindEmptyUnique(sXMLTNode* aNode) {
	sUNIQItem* aItem; int aValue = 0;
	for (aItem = &AllUniques; ; aItem = aItem->Next) {
		if (!aItem->Used) { aValue = aItem->UniqueID; aItem->Used++; break; }
		if (!aItem->Next) { break; } }
	if (aValue) { aItem->Node = aNode; }
	else { AddCleanObject(aItem->Next, sUNIQItem); aValue = aItem->UniqueID + 1; aItem->Next->UniqueID = aValue; aItem->Next->Node = aNode; aItem->Next->Used++; }
	return aValue;
}

//static int checkValidMark(sXMLTNode* aNode) { return ((aNode == &AllData) || (aNode && (aNode->NodeMark == NODE_MARK))); }
static int checkValidMark(sXMLTNode* aNode) { return ((aNode && (aNode->NodeMark == NODE_MARK))); }

static sXMLTNode* findLast(sXMLTNode* aNode) {
	sXMLTNode* aLast = aNode;
	if (aLast) { while (aLast->Next) { aLast = aLast->Next; } }
	return aLast;
}

static sXMLTNode* findPrevious(sXMLTNode* aNode) {
	sXMLTNode* aPrev;
	for (aPrev = aNode->Parent->FirstSon; aPrev; aPrev = aPrev->Next)
		if (aPrev->Next == aNode) return aPrev;
	GlobalError("Error in index search");
	return aPrev;
}

static sXMLTNode* findNodeByName(sXMLTNode* aNode, char* aName) {
	for (aNode = aNode->FirstSon; aNode; aNode = aNode->Next)
		if (!strcmp(aNode->Name, aName)) break;
	return aNode;
}

// adds node as First Son
static void insertFirst(sXMLTNode* aParent, sXMLTNode* newSon) {
	sXMLTNode* aOldFirst;
	if (!checkValidMark(aParent)) { GlobalError("Given parent problem"); }
	aOldFirst = aParent->FirstSon;
	aParent->FirstSon = newSon;
	newSon->Next = aOldFirst;
	newSon->Parent = aParent;
}

static void insertBefore(sXMLTNode* aTarget, sXMLTNode* newSon) {
	if (!checkValidMark(aTarget)) { GlobalError("Given target problem"); }
	if (aTarget->Parent->FirstSon == aTarget) { insertFirst(aTarget->Parent, newSon); }
	else {
		sXMLTNode* aPrev = findPrevious(aTarget);
		aPrev->Next = newSon;
		newSon->Next = aTarget;
		newSon->Parent = aTarget->Parent;
	}
}

static void insertAfter(sXMLTNode* aTarget, sXMLTNode* newSon) {
	if (!checkValidMark(aTarget)) { GlobalError("Given target problem"); }
	newSon->Next = aTarget->Next;
	newSon->Parent = aTarget->Parent;
	aTarget->Next = newSon;
}

static void insertLast(sXMLTNode* aParent, sXMLTNode* newSon) {
	if (!checkValidMark(aParent)) { GlobalError("Given parent problem"); }
	sXMLTNode* aNode;
	if ((aNode = findLast(aParent->FirstSon))) { insertAfter(aNode, newSon); }
	else { insertFirst(aParent, newSon); }
}

static sXMLTNode* addNode(sXMLTNode* aParent, char* aName) {
	NewCleanObject(newNode, sXMLTNode);
	AddStringCopy(newNode->Name, aName);
	newNode->UniqueID = findAndBindEmptyUnique(newNode);
	newNode->NodeMark = NODE_MARK;
	insertLast(aParent, newNode);
	return newNode;
}

void temporary_makevalid(void) {
	insertBefore(NULL,NULL);
	insertLast(NULL,NULL);
	insertFirst(NULL,NULL);
	insertAfter(NULL,NULL);
}

static void addValue(sXMLTNode* aNode, eDataTypes aDataType, uUniValue aValue) {
	aNode->DataType = aDataType;

	switch (aDataType) {

		case eDTByte: case eDTChar: case eDTWord: case eDTShort: case eDTDword: case eDTInt:
		case eDTReserved0: case eDTReserved1: case eDTReserved2: case eDTReserved3:
		case eDTVoid: case eDTsCharacter: case eDTCollection: { break; }

		case eDTString: case eDTsMemFile: case eDTReferedID: case eDTReference: { if (!aValue.aVoid) { GlobalError("Empty NewObjectValue"); } break; }

		default: { GlobalError("Unknown DataType"); break; }
	}

	switch (aDataType) {

		case eDTByte: case eDTChar: case eDTWord: case eDTShort: case eDTDword: case eDTInt:
		case eDTReferedID: case eDTReference: { aNode->Value = aValue; break; }

		case eDTString: { AddStringCopy(aNode->Value.aString, aValue.aString); break; }

		default: { break; }
	}
}

static void cleanNodeValue(sXMLTNode* aNode) {
	switch (aNode->DataType) {

		case eDTString: { xFree(aNode->Value.aString); break; }
		case eDTsCharacter: { deleteCharacter(aNode->Value.aVoid); break; }

		case eDTByte: case eDTShort: case eDTInt:
		case eDTChar: case eDTWord:	case eDTDword:
		case eDTReference: case eDTReferedID:
		case eDTVoid: case eDTCollection:
		case eDTReserved0: case eDTReserved1:
		case eDTReserved2: case eDTReserved3:
		default: { break; }
	}
}

sXMLTNode* addNodeAndValue(sXMLTNode* aParent, char* aName, eDataTypes aDataType, uUniValue aValue) {
	sXMLTNode* aNode = addNode(aParent, aName);
	addValue(aNode, aDataType, aValue);
	return aNode;
}

#define SEPARATORSLIST " \\/;:,.>"
static char* tokenFindFirstNonSeparator(char* aString) {
	char* aFound = NULL; int aValue;
	while ((aValue = *aString)) {
		if (strchr(SEPARATORSLIST, aValue)) { aString++; }
		else { aFound = aString; break; } }
	return aFound;
}

static int tokenCountNonSeparator(char* aString) {
	int aValue, aCount = 0;
	while ((aValue = *aString)) {
		if (strchr(SEPARATORSLIST, aValue)) { break; }
		else { aString++; aCount++; } }
	return aCount;
}

static sXMLTNode* findNodeByPath(sXMLTNode* aNode, char* aName) {
	sXMLTNode* aFound = NULL;
	int aLen; char* aTmp;
	if ((aTmp = aName) && (aLen = strlen(aName))) {
		aFound = aNode;
		while ((aTmp = tokenFindFirstNonSeparator(aTmp))) {
			aLen = tokenCountNonSeparator(aTmp);
			NewFixedCopy(aToken, aTmp, aLen);
			aFound = findNodeByName(aFound, aToken);
			xFree(aToken);
			aTmp += aLen;
			if (!aFound) break;
		}
	}
	return aFound;
}

sXMLTNode* findNode(sXMLTNode* aNode, ... ) {
	sXMLTNode* aFound = aNode;
	char* aStringArg;
	if (aFound) {
		va_list argList;
		va_start( argList, aNode );
		while ((aStringArg = va_arg(argList, char*)))
			if (!(aFound = findNodeByPath(aFound, aStringArg))) break;
		va_end( argList );
	}
	return aFound;
}

static int amIFirstSon(sXMLTNode* aNode) { return (aNode->Parent->FirstSon == aNode); }

static void unlinkNodeTree(sXMLTNode* aNode) {
	if (aNode->Parent) {
		if (amIFirstSon(aNode)) { aNode->Parent->FirstSon = aNode->Next; }
		else findPrevious(aNode)->Next = aNode->Next; }
	aNode->Next = NULL;
	aNode->Parent = NULL;
}

// Recursive nodes kill
void killNodeTree(sXMLTNode* aNode) {
	while (aNode->FirstSon) { killNodeTree(aNode->FirstSon); }
	//Prevent to kill Root Node
	if (aNode->UniqueID) {
		unlinkNodeTree(aNode);
		cleanNodeValue(aNode);
		if (aNode->Name) xFree(aNode->Name);
		findAndFreeUniqueID(aNode->UniqueID);
		xFree(aNode);
	}
}

void moveNodeTo(sXMLTNode* aParent, sXMLTNode* aNode) {
	unlinkNodeTree(aNode);
	insertFirst(aParent, aNode);
}

//void killAllSons(sXMLTNode* aNode) { while (aNode->FirstSon) { killNode(aNode->FirstSon); } }

static void printOffset(int level, char aChar) {
	int offset = level * OFFSETTAB;
	printf("\n"); while (offset--) { printf(" "); }
	if (aChar) printf("%c",aChar);
}

static void printValue(sXMLTNode* aNode) {
	NewString(valueString, VALUE_STRING_LEN);
	switch (aNode->DataType) {
	case eDTByte: case eDTChar: { sprintf(valueString, "== BYTE:['%d']", aNode->Value.aChar); break; }
	case eDTWord: case eDTShort: { sprintf(valueString, "== WORD:['%d']", aNode->Value.aShort); break; }
	case eDTDword: case eDTInt: { sprintf(valueString, "== DWORD:['%d']", aNode->Value.aInt); break; }
	case eDTString: { snprintf(valueString, VALUE_STRING_LEN - 1, "== STR:['%s']", aNode->Value.aString); break; }
	case eDTReference: { snprintf(valueString, VALUE_STRING_LEN - 1, "-> '%s'", (((sXMLTNode*)(aNode->Value.aVoid))->Name)); break; }
	case eDTReferedID: { snprintf(valueString, VALUE_STRING_LEN - 1, "-> (ID 0x%08x)", (((sUNIQItem*)(aNode->Value.aVoid))->UniqueID)); break; }
	default: { valueString[0] = 0; break; } }
	printf("%s", valueString);
	xFree(valueString);
}

static void printNodeType(sXMLTNode* aNode) {
	switch (aNode->DataType) {
	case eDTCollection: { printf(" {'%s'} ", aNode->Name); break; }
	case eDTVoid: { printf(" '%s' ", aNode->Name); break; }
	case eDTReference: case eDTReferedID: { printf(" <'%s'> ", aNode->Name); break; }
	default: { printf(" ['%s'] ", aNode->Name); break; }
	}
}

static void printNodeTreeInternal(sXMLTNode* aNode, int level, int maxl, int showID, int showName, int showValue) {
	sXMLTNode* aSubs;

	if (!aNode->UniqueID) { printf("\n<<<'%s'>>>", ALL_ACTIVE_DATA); }
	else {
		printOffset(level, 0);
		printf("*");
		if (showID) printf(" (ID 0x%08x)", aNode->UniqueID);
		if (showName) printNodeType(aNode);
		if (showValue) printValue(aNode);
	}

	aSubs = aNode->FirstSon;
	if (aSubs) {
		if (level < maxl) {
			printOffset(level, '{');
			do printNodeTreeInternal(aSubs, level + 1, maxl, showID, showName, showValue);
			while ((aSubs = aSubs->Next));
			printOffset(level, '}');
		} else printf(" { }");
	}
}

void printNodeTree(sXMLTNode* aNode, int level, int maxl, int showID, int showName, int showValue) {
	if (aNode && (level <= maxl)) { printNodeTreeInternal(aNode, level, maxl, showID, showName, showValue); printf("\n"); }
}

