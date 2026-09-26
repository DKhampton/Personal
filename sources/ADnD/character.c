
#include "global.h"
#include "character.h"

#define XENUM_IMPLEMENT_MODE
#include "parNames.enums.h"

#define XENUM_IMPLEMENT_MODE
#include "parSets.enums.h"

#include "nodes.h"

char* sSkillNames[] = { "VOID", "Two-Handed Swords", "Two-Handed Maces", "COUNT", };
char* sInfoNames[] = { "VOID", "idDungeon", "posX", "posY", "posZ", "widthX", "widthY", "widthZ", "COUNT" };

void clearSetValues(sXNode* aBase) {
	sXNode* xTmpParam = aBase->FirstSon;
	while (xTmpParam) { xTmpParam->Value.aInt = 0; xTmpParam = xTmpParam->Next; }
}

void addSingleSet(sXNode* aBase) {
	int i; for (i = 1; i < epnCOUNT; i++) {
		addNodeAndValue(aBase, sParNames[i], eDTDataS32, UNIZEROVALUE, false); }

}

void addFullSet(sXNode* aBase) {
	sXNode* xTmpParamsTmp;
	int i; for (i = 1; i < epsCOUNT; i++) {
		xTmpParamsTmp = addNodeAndValue(aBase, sParSets[i], eDTCollection, UNIZEROVALUE, false);
		addSingleSet(xTmpParamsTmp); } }

sXNode* createCharacterFromFunc(sXNode* aBase, sXNode* aOwner, char* aAlias, char* aName) {
	sXNode* xTmpChar =
		addNodeAndValue(aBase, aAlias, eDTsCharacter, UNIZEROVALUE, true);
		addNodeAndValue(xTmpChar, "Name", eDTString, (uUniValue)aName, true);
		addNodeAndValue(xTmpChar, "Owner", eDTReference, (uUniValue)aOwner, false);
		sXNode* xTmpParams =	addNodeAndValue(xTmpChar, "Parameters", eDTCollection, UNIZEROVALUE, false); addFullSet(xTmpParams);
	return xTmpChar;
}

void deleteCharacter(void* aChar) { }

void setParamS(sXNode* aChar, char* aSet, char* aPar, int aValue) {
	sXNode* aParamNode = findNode(aChar, "Parameters", aSet, aPar, NULL);
	if (!aParamNode) { GlobalError("Parameter NOT FOUND in collection"); }
	aParamNode->Value.aInt = aValue;
}

void setParamE(sXNode* aChar, eParSets aSet, eParNames aPar, int aValue) {
	setParamS(aChar, sParSets[aSet], sParNames[aPar], aValue);
}

int getParamS(sXNode* aChar, char* aSet, char* aPar) {
	sXNode* aParamNode = findNode(aChar, "Parameters", aSet, aPar, NULL);
	if (!aParamNode) { GlobalError("Parameter NOT FOUND in collection"); }
	return aParamNode->Value.aInt;
}

int getParamE(sXNode* aChar, eParSets aSet, eParNames aPar) {
	return getParamS(aChar, sParSets[aSet], sParNames[aPar]);
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

