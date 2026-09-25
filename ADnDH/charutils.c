/*
 * charutils.c
 *
 *  Created on: Jul 11, 2018
 *      Author: denys.prokhorov
 */
#include "global.h"
#include "nodes.h"
#include "character.h"


static int mBonusValueTable[MAXBONUSTABLEVALUE+1] =
#ifdef PERVERTION
//      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
	{ -10, -6, -5, -4, -3, -2, -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13 };
#else
	{ -10, -5, -4, -4, -3, -3, -2, -2, -1, -1,  0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10 };
#endif


void checkLimits(sXNode* aCharBase) {

}

void countUseful(sXNode* aCharBase) {
	int i; for (i=1; i<epnCOUNT; i++) {
		setParamE(aCharBase, epsUseful, i, getParamE(aCharBase, epsBase, i));
	}
}

static int fixSingleParamValue(int aValue) {
	if (aValue < 0) { aValue = 0; }
	if (aValue > MAXBONUSTABLEVALUE) { aValue = MAXBONUSTABLEVALUE; }
	return aValue;
}

static int countSingleParamPlus(int aValue) {
	return mBonusValueTable[fixSingleParamValue(aValue)];
}

int getTwoParamsPlus(sXNode* aCharBase, eParNames aPar1, eParNames aPar2) {
	// Perfect mathematics, unrounded part just ignored:  (0+0)/2=0;    (0+1)/2=0;    (1+1)/2=1;    (-1+0)/2=0;   (2+3)/2=2;
	return (countSingleParamPlus(getParamE(aCharBase, epsUseful, aPar1)) + countSingleParamPlus(getParamE(aCharBase, epsUseful, aPar2))) / 2;
}

/*
	int tmp1, tmp2, tmp3;

	tmp1 = getParamE(aCharBase, aPar1, epsUseful);
	tmp1 = countSingleParamPlus(tmp1);

	tmp2 = getParamE(aCharBase, aPar2, epsUseful);
	tmp2 = countSingleParamPlus(tmp2);

	tmp3 = (tmp1 + tmp2) / 2;
	return tmp3;
*/

