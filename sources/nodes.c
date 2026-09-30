#include "global.h"

#include <stdarg.h>

#include "nodes.h"
#include "ADnD/character.h"
#include "files.h"

#define XENUM_IMPLEMENT_MODE
#include "nodes.enums.h"

#define OFFSETTAB (1)
#define ALL_ACTIVE_DATA "AllLoadedData"
#define NODE_MARK (0xBABE4DAD)

uUniValue UNIZEROVALUE = { .aInt = 0 };
sXNode AllData = { NODE_MARK, 0, eDTVoid, 0, 0, { .pVoid = NULL }, ALL_ACTIVE_DATA, NULL, NULL, NULL }; //NULL,
sUNIQItem AllUniques = { 0, 1, { 0, 0, 0 }, &AllData };

void nodesKillUniques(void) {
	sUNIQItem* aItem = &AllUniques;
	sUNIQItem* aNext = aItem->Next;
	while (aNext) {
		aItem = aNext; aNext = aItem->Next; xFree(aItem);
	};
}

static sUNIQItem* nodesFindUniqueID(DWORD uID) {
	sUNIQItem* aItem;
	for (aItem = &AllUniques; aItem && (aItem->UniqueID != uID); aItem = aItem->Next);
	return aItem;
}

/*
static void nodesFindUniqueIDAndRemoveRef(int uID) {
	sUNIQItem* aItem;
	if ((aItem = nodesFindUniqueID(uID))) { if (aItem->Used < 2) { debug("[NODES] No Reference to remove"); } else { aItem->Used--; } }
}
*/

static void nodesFindAndFreeUniqueID(DWORD uID) {
	sUNIQItem* aItem;
	if ((aItem = nodesFindUniqueID(uID))) { if (--aItem->Used) { debug("[NODES] Cross Reference left"); } aItem->Used = 0; }
	else { debug("[NODES] uID was not found"); }
}

static DWORD nodesFindAndBindEmptyUnique(sXNode* aNode) {
	sUNIQItem* aItem; DWORD aValue = 0;
	for (aItem = &AllUniques; ; aItem = aItem->Next) {
		if (!aItem->Used) { aValue = aItem->UniqueID; aItem->Used++; break; }
		if (!aItem->Next) { break; } }
	if (aValue) { aItem->Node = aNode; }
	else { AddObjectClean(aItem->Next, sUNIQItem); aValue = aItem->UniqueID + 1; aItem->Next->UniqueID = aValue; aItem->Next->Node = aNode; aItem->Next->Used++; }
	return aValue;
}

//static int checkValidMark(sXNode* aNode) { return ((aNode == &AllData) || (aNode && (aNode->NodeMark == NODE_MARK))); }
static int nodesCheckValidMark(sXNode* aNode) { return ((aNode && (aNode->NodeMark == NODE_MARK))); }

static sXNode* nodesFindLast(sXNode* aNode) {
	sXNode* aLast = aNode;
	if (aLast) { while (aLast->Next) { aLast = aLast->Next; } }
	return aLast;
}

static sXNode* nodesFindPrevious(sXNode* aNode) {
	sXNode* aPrev;
	for (aPrev = aNode->Parent->FirstSon; aPrev; aPrev = aPrev->Next)
		if (aPrev->Next == aNode) return aPrev;
	GlobalError("Error in index search");
	return aPrev;
}

static sXNode* nodesFindNodeByName(sXNode* aNode, char* aName) {
	for (aNode = aNode->FirstSon; aNode; aNode = aNode->Next)
		if (!strcmp(aNode->Name, aName)) break;
	return aNode;
}

// adds node as First Son
static void nodesInsertFirst(sXNode* aParent, sXNode* newSon) {
	sXNode* aOldFirst;
	if (!nodesCheckValidMark(aParent)) { GlobalError("Given parent problem"); }
	aOldFirst = aParent->FirstSon;
	aParent->FirstSon = newSon;
	newSon->Next = aOldFirst;
	newSon->Parent = aParent;
}

static void nodesInsertBefore(sXNode* aTarget, sXNode* newSon) {
	if (!nodesCheckValidMark(aTarget)) { GlobalError("Given target problem"); }
	if (aTarget->Parent->FirstSon == aTarget) { nodesInsertFirst(aTarget->Parent, newSon); }
	else {
		sXNode* aPrev = nodesFindPrevious(aTarget);
		aPrev->Next = newSon;
		newSon->Next = aTarget;
		newSon->Parent = aTarget->Parent;
	}
}

static void nodesInsertAfter(sXNode* aTarget, sXNode* newSon) {
	if (!nodesCheckValidMark(aTarget)) { GlobalError("Given target problem"); }
	newSon->Next = aTarget->Next;
	newSon->Parent = aTarget->Parent;
	aTarget->Next = newSon;
}

static void nodesInsertLast(sXNode* aParent, sXNode* newSon) {
	if (!nodesCheckValidMark(aParent)) { GlobalError("Given parent problem"); }
	sXNode* aNode;
	if ((aNode = nodesFindLast(aParent->FirstSon))) { nodesInsertAfter(aNode, newSon); }
	else { nodesInsertFirst(aParent, newSon); }
}

static sXNode* nodesAddNode(sXNode* aParent, char* aName) {
	NewObjectClean(newNode, sXNode);
	AddStringCopy(newNode->Name, aName);
	newNode->UniqueID = nodesFindAndBindEmptyUnique(newNode);
	newNode->NodeMark = NODE_MARK;
	nodesInsertLast(aParent, newNode);
	return newNode;
}

void temporary_makevalid(void) {
	nodesInsertBefore(NULL,NULL);
	nodesInsertLast(NULL,NULL);
	nodesInsertFirst(NULL,NULL);
	nodesInsertAfter(NULL,NULL);
}

static void nodesAddValue(sXNode* aNode, eDataTypes aDataType, uUniValue aValue, DWORD autoClean) {
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

static void nodesCleanNodeValue(sXNode* aNode) {
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

sXNode* nodesAddNodeAndValue(sXNode* aParent, char* aName, eDataTypes aDataType, uUniValue aValue, DWORD autoClean) {
	sXNode* aNode = nodesAddNode(aParent, aName);
	nodesAddValue(aNode, aDataType, aValue, autoClean);
	return aNode;
}

static char* nodesTokenFindFirstNonSeparator(char* aString) {
	char* aFound = NULL; int aValue;
	while ((aValue = *aString)) {
		if (strchr(NODES_SEPARATOR_LIST, aValue)) { aString++; }
		else { aFound = aString; break; } }
	return aFound;
}

static int nodesTokenCountNonSeparator(char* aString) {
	int aValue, aCount = 0;
	while ((aValue = *aString)) {
		if (strchr(NODES_SEPARATOR_LIST, aValue)) { break; }
		else { aString++; aCount++; } }
	return aCount;
}

static sXNode* nodesFindNodeByPath(sXNode* aNode, char* aName) {
	sXNode* aFound = NULL;
	int aLen; char* aTmp;
	if ((aTmp = aName) && (aLen = strlen(aName))) {
		aFound = aNode;
		while ((aTmp = nodesTokenFindFirstNonSeparator(aTmp))) {
			aLen = nodesTokenCountNonSeparator(aTmp);
			NewStringCopyFixed(aToken, aTmp, aLen);
			aFound = nodesFindNodeByName(aFound, aToken);
			xFree(aToken);
			aTmp += aLen;
			if (!aFound) break;
		}
	}
	return aFound;
}

sXNode* nodesFindNode(sXNode* aNode, ... ) {
	sXNode* aFound = aNode;
	char* aStringArg;
	if (aFound) {
		va_list argList;
		va_start( argList, aNode );
		while ((aStringArg = va_arg(argList, char*)))
			if (!(aFound = nodesFindNodeByPath(aFound, aStringArg))) break;
		va_end( argList );
	}
	return aFound;
}

static DWORD nodesAmIFirstSon(sXNode* aNode) { return (aNode->Parent->FirstSon == aNode); }

static void nodesUnlinkTree(sXNode* aNode) {
	if (aNode->Parent) {
		if (nodesAmIFirstSon(aNode)) { aNode->Parent->FirstSon = aNode->Next; }
		else nodesFindPrevious(aNode)->Next = aNode->Next; }
	aNode->Next = NULL;
	aNode->Parent = NULL;
}

// Recursive nodes kill
void nodesKillTree(sXNode* aNode) {
	while (aNode->FirstSon) { nodesKillTree(aNode->FirstSon); }
	//Prevent to kill Global Root Node
	if (aNode->UniqueID) {
		nodesUnlinkTree(aNode);
		nodesCleanNodeValue(aNode);
		if (aNode->Name) xFree(aNode->Name);
		nodesFindAndFreeUniqueID(aNode->UniqueID);
		xFree(aNode);
	}
}

void nodesMoveNodeTo(sXNode* aParent, sXNode* aNode) {
	nodesUnlinkTree(aNode);
	nodesInsertFirst(aParent, aNode);
}

//void nodesKillAllSons(sXNode* aNode) { while (aNode->FirstSon) { nodesKillTree(aNode->FirstSon); } }

static void nodesConsoleOffset(int level, char aChar) {
	int offset = level * OFFSETTAB;
	console("\n"); while (offset--) { console(" "); }
	if (aChar) console("%c",aChar);
}

static void nodesConsoleValue(sXNode* aNode) {
	NewString(valueString, CONSOLE_STRING_VALUE_MAX_LEN);
	switch (aNode->DataType) {
		case eDTDataU8: case eDTDataS8: { sprintf(valueString, "== BYTE:['%d']", aNode->Value.aChar); break; }
		case eDTDataU16: case eDTDataS16: { sprintf(valueString, "== WORD:['%d']", aNode->Value.aShort); break; }
		case eDTDataU32: case eDTDataS32: { sprintf(valueString, "== DWORD:['%d']", aNode->Value.aInt); break; }
		case eDTPtrU8: case eDTPtrS8: { sprintf(valueString, "-> BYTE:['%d']", *(BYTE*)aNode->Value.pVoid); break; }
		case eDTPtrU16: case eDTPtrS16: { sprintf(valueString, "-> WORD:['%d']", *(WORD*)aNode->Value.pVoid); break; }
		case eDTPtrU32: case eDTPtrS32: { sprintf(valueString, "-> DWORD:['%d']", *(DWORD*)aNode->Value.pVoid); break; }
//		case eDTPtrU64: case eDTPtrS64: { sprintf(valueString, "-> QWORD:['%d-%d']", *((DWORD*)aNode->Value.pVoid + 0),*((DWORD*)aNode->Value.pVoid + sizeof(int))); break; }
		case eDTPtrUser: { sprintf(valueString, "-> PTR:['0x%08x']", aNode->Value.aDword); break; }
		case eDTString: { snprintf(valueString, CONSOLE_STRING_VALUE_MAX_LEN - 1, "== STR:['%s']", aNode->Value.pString); break; }
		case eDTReference: { snprintf(valueString, CONSOLE_STRING_VALUE_MAX_LEN - 1, "-> '%s'", (aNode->Value.pXNode->Name)); break; } // ((sXNode*)(aNode->Value.pVoid))->Name));
		case eDTReferedID: { snprintf(valueString, CONSOLE_STRING_VALUE_MAX_LEN - 1, "-> (ID 0x%08x)", (((sUNIQItem*)(aNode->Value.pVoid))->UniqueID)); break; }
		default: { valueString[0] = 0; break; } }
	console("%s", valueString);
	xFree(valueString);
}

static void nodesConsoleNodeType(sXNode* aNode) {
	switch (aNode->DataType) {
		case eDTCollection: { console(" {'%s'} ", aNode->Name); break; }
		case eDTVoid: { console(" '%s' ", aNode->Name); break; }
		case eDTReference: case eDTReferedID: { console(" <'%s'> ", aNode->Name); break; }
		default: { console(" ['%s'] ", aNode->Name); break; }
	}
}

static void nodesConsoleTreeInternal(sXNode* aNode, int level, int maxl, int showID, int showName, int showValue) {
	sXNode* aSubs;
	if (!aNode->UniqueID) { console("\n<<<'%s'>>>", ALL_ACTIVE_DATA); }
	else {
		nodesConsoleOffset(level, 0);
		console("*");
		if (showID) console(" (ID 0x%08x)", aNode->UniqueID);
		if (showName) nodesConsoleNodeType(aNode);
		if (showValue) nodesConsoleValue(aNode);
	}

	aSubs = aNode->FirstSon;
	if (aSubs) {
		if (level < maxl) {
			nodesConsoleOffset(level, '{');
			do nodesConsoleTreeInternal(aSubs, level + 1, maxl, showID, showName, showValue);
			while ((aSubs = aSubs->Next));
			nodesConsoleOffset(level, '}');
		} else console(" { }");
	}
}

void nodesConsoleTree(sXNode* aNode, int level, int maxl, int showID, int showName, int showValue) {
	if (aNode && (level <= maxl)) { nodesConsoleTreeInternal(aNode, level, maxl, showID, showName, showValue); console("\n"); }
}

