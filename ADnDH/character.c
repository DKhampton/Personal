
#include "global.h"

#define XENUM_IMPLEMENT_MODE
#include "eParSets.h"
#define XENUM_IMPLEMENT_MODE
#include "eParNames.h"

#include "character.h"
#include "nodes.h"

void clearSetValues(sXMLTNode* aBase) {
	sXMLTNode* xTmpParam = aBase->FirstSon;
	while (xTmpParam) { xTmpParam->Value.aInt = 0; xTmpParam = xTmpParam->Next; }
}

void addSingleParamsSet(sXMLTNode* aBase) {
	int i; for (i = 1; i < epnCOUNT; i++) {
		addNodeAndValue(aBase, sParNames[i], eDTInt, UNIZEROVALUE);	} }

void addFullParamsSet(sXMLTNode* aBase) {
	sXMLTNode* xTmpParamsTmp;
	int i; for (i = 1; i < epsCOUNT; i++) {
		xTmpParamsTmp = addNodeAndValue(aBase, sParSets[i], eDTCollection, UNIZEROVALUE);
		addSingleParamsSet(xTmpParamsTmp); } }

sXMLTNode* createCharacterFromFunc(sXMLTNode* aBase, sXMLTNode* aOwner, char* aAlias, char* aName) {
	sXMLTNode* xTmpChar =
		addNodeAndValue(aBase, aAlias, eDTsCharacter, UNIZEROVALUE);
		addNodeAndValue(xTmpChar, "Name", eDTString, (uUniValue)aName);
		addNodeAndValue(xTmpChar, "Owner", eDTReference, (uUniValue)aOwner);
		sXMLTNode* xTmpParams =	addNodeAndValue(xTmpChar, "Parameters", eDTCollection, UNIZEROVALUE); addFullParamsSet(xTmpParams);
	return xTmpChar;
}

void deleteCharacter(sXMLTNode* aChar) { }

void setParamS(sXMLTNode* aChar, char* aPar, char* aSet, int aValue) {
	sXMLTNode* aParamNode = findNode(aChar, "Parameters", aSet, aPar, NULL);
	if (!aParamNode) { GlobalError("Parameter NOT FOUND in collection"); }
	aParamNode->Value.aInt = aValue;
}

void setParamE(sXMLTNode* aChar, eParNames aPar, eParSets aSet, int aValue) {
	setParamS(aChar, sParNames[aPar], sParSets[aSet], aValue);
}

int getParamS(sXMLTNode* aChar, char* aPar, char* aSet) {
	sXMLTNode* aParamNode = findNode(aChar, "Parameters", aSet, aPar, NULL);
	if (!aParamNode) { GlobalError("Parameter NOT FOUND in collection"); }
	return aParamNode->Value.aInt;
}

int getParamE(sXMLTNode* aChar, eParNames aPar, eParSets aSet) {
	return getParamS(aChar, sParNames[aPar], sParSets[aSet]);
}

/*
sCharacter* loadCharacter(int CharID) {
	NewObject(tmpChar, sCharacter);
	NewString(tmpString, MAX_PATH_LEN);
	snprintf(tmpString,MAX_PATH_LEN-1, "%04x.chr", CharID);
	sMemFile* memFile = loadMemFile(tmpString);
	xFree(tmpString);
	xFree(memFile);
	return tmpChar;
}
*/

