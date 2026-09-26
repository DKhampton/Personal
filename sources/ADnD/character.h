#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "global.h"

#include "parNames.enums.h"
#include "parSets.enums.h"

#include "nodes.h"

extern sXNode* createCharacterFromFunc(sXNode* aBase, sXNode* aOwner, char* aAlias, char* aName);
extern void deleteCharacter(void* aChar);
extern void setParamS(sXNode* aChar, char* aSet, char* aPar, int aValue);
extern void setParamE(sXNode* aChar, eParSets aSet, eParNames aPar, int aValue);
extern int  getParamS(sXNode* aChar, char* aSet, char* aPar);
extern int  getParamE(sXNode* aChar, eParSets aSet, eParNames aPar);

#endif /* CHARACTER_H_ */

