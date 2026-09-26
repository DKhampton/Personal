
#include <stdarg.h>
#include <stdlib.h>

#include "nodes.h"
#include "character.h"
#include "files.h"

#define IMPLEMENTATION_ENUM
#include "nodes.enums.h"

#define OFFSETTAB (1)
#define DEBUG_STRING_VALUE_LEN (64)
#define ALL_ACTIVE_DATA "AllLoadedData"
#define NODE_MARK (0xBABE4DAD)

uUniValue UNIZEROVALUE = { .aInt = 0 };
sXNode AllData = { NODE_MARK, 0, eDTVoid, 0, 0, { .pVoid = NULL }, ALL_ACTIVE_DATA, NULL, NULL, NULL }; //NULL,
sUNIQItem AllUniques = { 0, 1, { 0, 0, 0 }, &AllData };

void killUniques(void) {
	sUNIQItem* aItem = &AllUniques;
	sUNIQItem* aNext = aItem->Next;
	while (aNext) {
		aItem = aNext; aNext = aItem->Next; xFree(aItem);
	};
}

static sUNIQItem* findUniqueID(DWORD uID) {
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

static void findAndFreeUniqueID(DWORD uID) {
	sUNIQItem* aItem;
	if ((aItem = findUniqueID(uID))) { if (--aItem->Used) { debug("Cross Reference left"); } aItem->Used = 0; }
	else { debug("uID was not found"); }
}

static DWORD findAndBindEmptyUnique(sXNode* aNode) {
	sUNIQItem* aItem; DWORD aValue = 0;
	for (aItem = &AllUniques; ; aItem = aItem->Next) {
		if (!aItem->Used) { aValue = aItem->UniqueID; aItem->Used++; break; }
		if (!aItem->Next) { break; } }
	if (aValue) { aItem->Node = aNode; }
	else { AddObjectClean(aItem->Next, sUNIQItem); aValue = aItem->UniqueID + 1; aItem->Next->UniqueID = aValue; aItem->Next->Node = aNode; aItem->Next->Used++; }
	return aValue;
}

//static int checkValidMark(sXNode* aNode) { return ((aNode == &AllData) || (aNode && (aNode->NodeMark == NODE_MARK))); }
static int checkValidMark(sXNode* aNode) { return ((aNode && (aNode->NodeMark == NODE_MARK))); }

static sXNode* findLast(sXNode* aNode) {
	sXNode* aLast = aNode;
	if (aLast) { while (aLast->Next) { aLast = aLast->Next; } }
	return aLast;
}

static sXNode* findPrevious(sXNode* aNode) {
	sXNode* aPrev;
	for (aPrev = aNode->Parent->FirstSon; aPrev; aPrev = aPrev->Next)
		if (aPrev->Next == aNode) return aPrev;
	GlobalError("Error in index search");
	return aPrev;
}

static sXNode* findNodeByName(sXNode* aNode, char* aName) {
	for (aNode = aNode->FirstSon; aNode; aNode = aNode->Next)
		if (!strcmp(aNode->Name, aName)) break;
	return aNode;
}

// adds node as First Son
static void insertFirst(sXNode* aParent, sXNode* newSon) {
	sXNode* aOldFirst;
	if (!checkValidMark(aParent)) { GlobalError("Given parent problem"); }
	aOldFirst = aParent->FirstSon;
	aParent->FirstSon = newSon;
	newSon->Next = aOldFirst;
	newSon->Parent = aParent;
}

static void insertBefore(sXNode* aTarget, sXNode* newSon) {
	if (!checkValidMark(aTarget)) { GlobalError("Given target problem"); }
	if (aTarget->Parent->FirstSon == aTarget) { insertFirst(aTarget->Parent, newSon); }
	else {
		sXNode* aPrev = findPrevious(aTarget);
		aPrev->Next = newSon;
		newSon->Next = aTarget;
		newSon->Parent = aTarget->Parent;
	}
}

static void insertAfter(sXNode* aTarget, sXNode* newSon) {
	if (!checkValidMark(aTarget)) { GlobalError("Given target problem"); }
	newSon->Next = aTarget->Next;
	newSon->Parent = aTarget->Parent;
	aTarget->Next = newSon;
}

static void insertLast(sXNode* aParent, sXNode* newSon) {
	if (!checkValidMark(aParent)) { GlobalError("Given parent problem"); }
	sXNode* aNode;
	if ((aNode = findLast(aParent->FirstSon))) { insertAfter(aNode, newSon); }
	else { insertFirst(aParent, newSon); }
}

static sXNode* addNode(sXNode* aParent, char* aName) {
	NewObjectClean(newNode, sXNode);
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

static void addValue(sXNode* aNode, eDataTypes aDataType, uUniValue aValue, DWORD autoClean) {
	aNode->DataType = aDataType;


	// Check or Set cleaner Pass
	switch (aDataType) {

		// ignores Cleaner cause data is in Value cell
		case eDTVoid:
		case eDTDataU8: case eDTDataS8: case eDTDataU16: case eDTDataS16: case eDTDataU32: case eDTDataS32:
		case eDTReserved0: case eDTReserved1: case eDTReserved2: case eDTReserved3:
		case eDTReferedID: case eDTReference: case eDTCollection: { if (autoClean) { GlobalError("AutoClean Set for Static"); } aNode->AutoClean = autoClean; break; }

		// set cleaners for types
		case eDTPtrU8: case eDTPtrU16: case eDTPtrU32: case eDTPtrS8: case eDTPtrS16: case eDTPtrS32: //case eDTPtrU64: case eDTPtrS64:
		case eDTPtrUser: case eDTsCharacter: case eDTsMemFile: case eDTString: { aNode->AutoClean = autoClean; break; }

		default: { GlobalError("Unknown DataType"); break; }
	}

	// Check for empty object value
	switch (aDataType) {
		case eDTPtrU8: case eDTPtrU16: case eDTPtrU32: case eDTPtrS8: case eDTPtrS16: case eDTPtrS32: //case eDTPtrU64: case eDTPtrS64:
		case eDTString: case eDTsMemFile: { if (!aValue.pVoid) { GlobalError("Empty object not allowed"); } break; }
		default: { break; }
	}


	switch (aDataType) {
		// duplicates string if cleaner is set or passes char* for static strings
		case eDTVoid: { aNode->Value.aDword = 0; break; };
		case eDTString: { if (autoClean) { AddStringCopy(aNode->Value.pString, aValue.pString); } else { aNode->Value = aValue; } break; }

		default: { aNode->Value = aValue; break; }
	}
}

static void cleanNodeValue(sXNode* aNode) {
	// call cleaner if set
	if (aNode->AutoClean) {
		switch (aNode->DataType) {

			case eDTPtrU8: case eDTPtrU16: case eDTPtrU32: //case eDTPtrU64:
			case eDTPtrS8: case eDTPtrS16: case eDTPtrS32: //case eDTPtrS64:
			case eDTPtrUser: case eDTString: { if (aNode->Value.pVoid) { xFree(aNode->Value.pVoid); } break; }
			case eDTsCharacter: { deleteCharacter(aNode->Value.pVoid); break; }
			case eDTsMemFile: { killMemFile(aNode->Value.pVoid); break; }

			default: { GlobalError("AutoClean Act for Static"); break; }
		}
	}
}

sXNode* addNodeAndValue(sXNode* aParent, char* aName, eDataTypes aDataType, uUniValue aValue, DWORD autoClean) {
	sXNode* aNode = addNode(aParent, aName);
	addValue(aNode, aDataType, aValue, autoClean);
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

static sXNode* findNodeByPath(sXNode* aNode, char* aName) {
	sXNode* aFound = NULL;
	int aLen; char* aTmp;
	if ((aTmp = aName) && (aLen = strlen(aName))) {
		aFound = aNode;
		while ((aTmp = tokenFindFirstNonSeparator(aTmp))) {
			aLen = tokenCountNonSeparator(aTmp);
			NewStringCopyFixed(aToken, aTmp, aLen);
			aFound = findNodeByName(aFound, aToken);
			xFree(aToken);
			aTmp += aLen;
			if (!aFound) break;
		}
	}
	return aFound;
}

sXNode* findNode(sXNode* aNode, ... ) {
	sXNode* aFound = aNode;
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

static DWORD amIFirstSon(sXNode* aNode) { return (aNode->Parent->FirstSon == aNode); }

static void unlinkNodeTree(sXNode* aNode) {
	if (aNode->Parent) {
		if (amIFirstSon(aNode)) { aNode->Parent->FirstSon = aNode->Next; }
		else findPrevious(aNode)->Next = aNode->Next; }
	aNode->Next = NULL;
	aNode->Parent = NULL;
}

// Recursive nodes kill
void killNodeTree(sXNode* aNode) {
	while (aNode->FirstSon) { killNodeTree(aNode->FirstSon); }
	//Prevent to kill Global Root Node
	if (aNode->UniqueID) {
		unlinkNodeTree(aNode);
		cleanNodeValue(aNode);
		if (aNode->Name) xFree(aNode->Name);
		findAndFreeUniqueID(aNode->UniqueID);
		xFree(aNode);
	}
}

void moveNodeTo(sXNode* aParent, sXNode* aNode) {
	unlinkNodeTree(aNode);
	insertFirst(aParent, aNode);
}

//void killAllSons(sXNode* aNode) { while (aNode->FirstSon) { killNode(aNode->FirstSon); } }

static void debugOffset(int level, char aChar) {
	int offset = level * OFFSETTAB;
	debug("\n"); while (offset--) { debug(" "); }
	if (aChar) debug("%c",aChar);
}

static void debugValue(sXNode* aNode) {
	NewString(valueString, DEBUG_STRING_VALUE_LEN);
	switch (aNode->DataType) {
		case eDTDataU8: case eDTDataS8: { sprintf(valueString, "== BYTE:['%d']", aNode->Value.aChar); break; }
		case eDTDataU16: case eDTDataS16: { sprintf(valueString, "== WORD:['%d']", aNode->Value.aShort); break; }
		case eDTDataU32: case eDTDataS32: { sprintf(valueString, "== DWORD:['%d']", aNode->Value.aInt); break; }
		case eDTPtrU8: case eDTPtrS8: { sprintf(valueString, "-> BYTE:['%d']", *(BYTE*)aNode->Value.pVoid); break; }
		case eDTPtrU16: case eDTPtrS16: { sprintf(valueString, "-> WORD:['%d']", *(WORD*)aNode->Value.pVoid); break; }
		case eDTPtrU32: case eDTPtrS32: { sprintf(valueString, "-> DWORD:['%d']", *(DWORD*)aNode->Value.pVoid); break; }
//		case eDTPtrU64: case eDTPtrS64: { sprintf(valueString, "-> QWORD:['%d-%d']", *((DWORD*)aNode->Value.pVoid + 0),*((DWORD*)aNode->Value.pVoid + sizeof(int))); break; }
		case eDTPtrUser: { sprintf(valueString, "-> PTR:['0x%08x']", aNode->Value.aDword); break; }
		case eDTString: { snprintf(valueString, DEBUG_STRING_VALUE_LEN - 1, "== STR:['%s']", aNode->Value.pString); break; }
		case eDTReference: { snprintf(valueString, DEBUG_STRING_VALUE_LEN - 1, "-> '%s'", (aNode->Value.pXNode->Name)); break; } // ((sXNode*)(aNode->Value.pVoid))->Name));
		case eDTReferedID: { snprintf(valueString, DEBUG_STRING_VALUE_LEN - 1, "-> (ID 0x%08x)", (((sUNIQItem*)(aNode->Value.pVoid))->UniqueID)); break; }
		default: { valueString[0] = 0; break; } }
	debug("%s", valueString);
	xFree(valueString);
}

static void debugNodeType(sXNode* aNode) {
	switch (aNode->DataType) {
		case eDTCollection: { debug(" {'%s'} ", aNode->Name); break; }
		case eDTVoid: { debug(" '%s' ", aNode->Name); break; }
		case eDTReference: case eDTReferedID: { debug(" <'%s'> ", aNode->Name); break; }
		default: { debug(" ['%s'] ", aNode->Name); break; }
	}
}

static void debugNodeTreeInternal(sXNode* aNode, int level, int maxl, int showID, int showName, int showValue) {
	sXNode* aSubs;
	if (!aNode->UniqueID) { debug("\n<<<'%s'>>>", ALL_ACTIVE_DATA); }
	else {
		debugOffset(level, 0);
		debug("*");
		if (showID) debug(" (ID 0x%08x)", aNode->UniqueID);
		if (showName) debugNodeType(aNode);
		if (showValue) debugValue(aNode);
	}

	aSubs = aNode->FirstSon;
	if (aSubs) {
		if (level < maxl) {
			debugOffset(level, '{');
			do debugNodeTreeInternal(aSubs, level + 1, maxl, showID, showName, showValue);
			while ((aSubs = aSubs->Next));
			debugOffset(level, '}');
		} else debug(" { }");
	}
}

void debugNodeTree(sXNode* aNode, int level, int maxl, int showID, int showName, int showValue) {
	if (aNode && (level <= maxl)) { debugNodeTreeInternal(aNode, level, maxl, showID, showName, showValue); debug("\n"); }
}

