
#include "global.h"
#include "character.h"
#include "files.h"
#include "nodes.h"
#include "consoles.h"

static int SystemTimer = 100;
static int WorldTimer = 1000;


int mainMod(int argc, char **argv) {

	int retValue = 0;

	printf("Hello\n"); fillDelimiterLine();
	sXMLTNode* search;

	sXMLTNode* xSystem = addNodeAndValue(&AllData, "System", eDTCollection, UNIZEROVALUE);
	//sXMLTNode* xRealTime =
			addNodeAndValue(xSystem, "RealTime", eDTDword, (uUniValue)SystemTimer);
	//sXMLTNode* xWorldTime =
			addNodeAndValue(xSystem, "WorldTime", eDTDword, (uUniValue)WorldTimer);

	sXMLTNode* xPlayerList = addNodeAndValue(&AllData, "PlayerList", eDTCollection, UNIZEROVALUE);
	sXMLTNode* xPlayerDenDi =
			addNodeAndValue(xPlayerList, "DenDi", eDTString, (uUniValue)"Den Di Khampton");
	//sXMLTNode* xPlayerVarg =
			addNodeAndValue(xPlayerList, "Varg", eDTString, (uUniValue)"Varg Varconous");

	sXMLTNode* xCharacterList = addNodeAndValue(&AllData, "CharacterList", eDTCollection, UNIZEROVALUE);
	createCharacterFromFunc(xCharacterList, xPlayerDenDi, "Eric", "Eric Airslasher IV");

	printNodeTree(&AllData,0,40,1,1,1);

	fillDelimiterLine();

	search = findNode(&AllData, "PlayerList/Varg", NULL);
	printNodeTree(search,0,0,1,1,1);

	search = findNode(&AllData, "System", "WorldTime", NULL);
	printNodeTree(search,0,0,1,1,1);


	fillDelimiterLine();
	killNodeTree(&AllData); killUniques();

#ifdef DEBUG_INFO
	retValue = mallocsMade;
	printf("\nMallocs Left: %d", retValue);
#endif

	printf("\nBye Bye\n");

	printf("sizeof(void*) = %d\n", sizeof(void*));

	return retValue;
}


int main(int argc, char **argv) {
	//return tmpFunc();
	return mainMod(argc, argv);
}

