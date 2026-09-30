#include "global.h"

#include "ADnD/character.h"
#include "ADnD/charutils.h"
#include "files.h"
#include "nodes.h"
#include "consoles.h"
#include "sprintb.h"

int SystemTimer = 100;
int WorldTimer = 1000;
QWORD global64bit = 11111111111111;

int mainMod(int argc, char **argv) {

	console("Hello\n"); consoleFillDelimiterLine();
	sXNode* search;

	sXNode* xSystem = nodesAddNodeAndValue(&AllData, "System", eDTCollection, UNIZEROVALUE, false);
	//sXNode* xRealTime =
			nodesAddNodeAndValue(xSystem, "RealTime", eDTPtrU32, (uUniValue)((void*)&SystemTimer), false);
	//sXNode* xWorldTime =
			nodesAddNodeAndValue(xSystem, "WorldTime", eDTPtrU32, (uUniValue)((void*)&WorldTimer), false);
			nodesAddNodeAndValue(xSystem, "TryUserData", eDTPtrUser, (uUniValue)((void*)&global64bit), false);

	sXNode* xPlayerList = nodesAddNodeAndValue(&AllData, "PlayerList", eDTCollection, UNIZEROVALUE, false);
	sXNode* xPlayerDenDi =
			nodesAddNodeAndValue(xPlayerList, "DenDi", eDTString, (uUniValue)"Den Di Khampton", false);
	//sXNode* xPlayerVarg =
			nodesAddNodeAndValue(xPlayerList, "Varg", eDTString, (uUniValue)"Varg Varconous", false);

	sXNode* xCharacterList = nodesAddNodeAndValue(&AllData, "CharacterList", eDTCollection, UNIZEROVALUE, false);
	createCharacterFromFunc(xCharacterList, xPlayerDenDi, "Eric", "Eric Airslasher IV");

	nodesConsoleTree(&AllData,0,1,1,1,1); consoleFillDelimiterLine();

	search = nodesFindNode(&AllData, "System", "TryUserData", NULL);
	if (search) { console("0x%08x",search->Value.aDword); }

	search = nodesFindNode(&AllData, "cfgport", "miditx", "1", "shift", NULL);


	nodesConsoleTree(xPlayerDenDi,0,1,1,1,1);	nodesConsoleTree(&AllData,0,40,1,1,1);	nodesConsoleTree(&AllData,0,40,1,1,1);	consoleFillDelimiterLine();


	// -------------------------------------------------------------------------------------------------------

	search = nodesFindNode(&AllData, "CharacterList", "Eric", NULL);

	setParamE(search,epsBase,epnSTR,10);
	setParamE(search,epsBase,epnDEX,8);
	setParamE(search,epsBase,epnCON,5);
	setParamE(search,epsBase,epnINT,6);
	setParamE(search,epsBase,epnWIS,8);
	setParamE(search,epsBase,epnWILL,9);

	countUseful(search);

	nodesConsoleTree(search,0,40,1,1,1); consoleFillDelimiterLine();

	// -------------------------------------------------------------------------------------------------------

	console("\nBonus: %d\n", getTwoParamsPlus(search,epnDEX,epnSTR));

	// -------------------------------------------------------------------------------------------------------

	search = nodesFindNode(&AllData, "PlayerList/Varg", NULL);
	nodesConsoleTree(search,0,0,1,1,1); consoleFillDelimiterLine();

	search = nodesFindNode(&AllData, "System", "WorldTime", NULL);
	nodesConsoleTree(search,0,0,1,1,1); consoleFillDelimiterLine();

	search = nodesFindNode(&AllData, "CharacterList", NULL);
	nodesKillTree(search);

	nodesConsoleTree(&AllData,0,40,1,1,1); consoleFillDelimiterLine();


	// -------------------------------------------------------------------------------------------------------

	nodesKillTree(&AllData); nodesKillUniques();

#ifdef DEBUG_MALLOC
	debug("\nMallocs Left: %d\n", mallocsMade);
#endif

	return 0;
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
