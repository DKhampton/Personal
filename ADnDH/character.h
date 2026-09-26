#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "global.h"

#include "nodes.h"

typedef enum {
	epnVoid,
	epnSTR,
	epnDEX,
	epnCON,
	epnPER,
	epnWILL,
	epnWIS,
	epnINT,
	epnCHA,
	epnLUCK,
	epnCOUNT
} eParNames;
extern char* sParNames[];

typedef enum {
	epsVoid,
	epsBase,
	epsUseful,
	epsCOUNT
} eTheSets;
extern char* sTheSets[];

extern sXNode* createCharacterFromFunc(sXNode* aBase, sXNode* aOwner, char* aAlias, char* aName);
extern void deleteCharacter(void* aChar);
extern void setParamS(sXNode* aChar, char* aSet, char* aPar, int aValue);
extern void setParamE(sXNode* aChar, eTheSets aSet, eParNames aPar, int aValue);
extern int  getParamS(sXNode* aChar, char* aSet, char* aPar);
extern int  getParamE(sXNode* aChar, eTheSets aSet, eParNames aPar);

#endif /* CHARACTER_H_ */

