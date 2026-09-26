
#include "global.h"
#include "character.h"
#include "charutils.h"
#include "files.h"
#include "nodes.h"
#include "consoles.h"
#include "sprintb.h"

int SystemTimer = 100;
int WorldTimer = 1000;
QWORD global64bit = 11111111111111;

int mainMod(int argc, char **argv) {

	debug("Hello\n"); fillDelimiterLine();
	sXNode* search;

	sXNode* xSystem = addNodeAndValue(&AllData, "System", eDTCollection, UNIZEROVALUE, false);
	//sXNode* xRealTime =
			addNodeAndValue(xSystem, "RealTime", eDTPtrU32, (uUniValue)((void*)&SystemTimer), false);
	//sXNode* xWorldTime =
			addNodeAndValue(xSystem, "WorldTime", eDTPtrU32, (uUniValue)((void*)&WorldTimer), false);
			addNodeAndValue(xSystem, "TryUserData", eDTPtrUser, (uUniValue)((void*)&global64bit), false);

	sXNode* xPlayerList = addNodeAndValue(&AllData, "PlayerList", eDTCollection, UNIZEROVALUE, false);
	sXNode* xPlayerDenDi =
			addNodeAndValue(xPlayerList, "DenDi", eDTString, (uUniValue)"Den Di Khampton", false);
	//sXNode* xPlayerVarg =
			addNodeAndValue(xPlayerList, "Varg", eDTString, (uUniValue)"Varg Varconous", false);

	sXNode* xCharacterList = addNodeAndValue(&AllData, "CharacterList", eDTCollection, UNIZEROVALUE, false);
	createCharacterFromFunc(xCharacterList, xPlayerDenDi, "Eric", "Eric Airslasher IV");

	debugNodeTree(&AllData,0,1,1,1,1);	fillDelimiterLine();

	search = findNode(&AllData, "System", "TryUserData", NULL);
	if (search) { debug("0x%08x",search->Value.aDword); }

	search = findNode(&AllData, "cfgport", "miditx", "1", "shift", NULL);


	debugNodeTree(xPlayerDenDi,0,1,1,1,1);	fillDelimiterLine();

	debugNodeTree(&AllData,0,40,1,1,1);	fillDelimiterLine();


	// -------------------------------------------------------------------------------------------------------

	search = findNode(&AllData, "CharacterList", "Eric", NULL);

	setParamE(search,epsBase,epnSTR,10);
	setParamE(search,epsBase,epnDEX,8);
	setParamE(search,epsBase,epnCON,5);
	setParamE(search,epsBase,epnINT,6);
	setParamE(search,epsBase,epnWIS,8);
	setParamE(search,epsBase,epnWILL,9);

	countUseful(search);

	debugNodeTree(search,0,40,1,1,1); fillDelimiterLine();

	// -------------------------------------------------------------------------------------------------------

	debug("\nBonus: %d\n", getTwoParamsPlus(search,epnDEX,epnSTR));

	// -------------------------------------------------------------------------------------------------------
/*
	search = findNode(&AllData, "PlayerList/Varg", NULL);
	debugNodeTree(search,0,0,1,1,1); fillDelimiterLine();

	search = findNode(&AllData, "System", "WorldTime", NULL);
	debugNodeTree(search,0,0,1,1,1); fillDelimiterLine();

	search = findNode(&AllData, "CharacterList", NULL);
	killNodeTree(search);

	debugNodeTree(&AllData,0,40,1,1,1);	fillDelimiterLine();
*/

	// -------------------------------------------------------------------------------------------------------

	killNodeTree(&AllData); killUniques();

#ifdef DEBUG_MALLOC
	debug("\nMallocs Left: %d\n", mallocsMade);
#endif

	return GlobalError(NULL);
}

int main(int argc, char **argv) {
	//return tmpFunc();
	//return mainMod(argc, argv);
	//consoleModule(*argv);
	mainMod(argc,argv);
	//mainSub(argc, argv);
	//mainWrt(argc,argv);
	//mainCountQuick(argc,argv);
	//printf("%s \n",strDataTypes[eDTNone]);
	return 0;
}
