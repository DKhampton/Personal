#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "nodes.h"
#include "eParSets.h"
#include "eParNames.h"

extern sXMLTNode* createCharacterFromFunc(sXMLTNode* aBase, sXMLTNode* aOwner, char* aAlias, char* aName);
extern void deleteCharacter(sXMLTNode* aChar);
extern void setParamS(sXMLTNode* aChar, char* aPar, char* aSet, int aValue);
extern void setParamE(sXMLTNode* aChar, eParNames aPar, eParSets aSet, int aValue);
extern int  getParamS(sXMLTNode* aChar, char* aPar, char* aSet);
extern int  getParamE(sXMLTNode* aChar, eParNames aPar, eParSets aSet);

#endif /* CHARACTER_H_ */

