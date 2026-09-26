
#include "global.h"

#ifdef DEBUG_MALLOC

typedef struct {
	void* Address;
	DWORD Size;
} mallocsData;
int mallocsIndex = 0;
int mallocsMade = 0;
mallocsData mallocsInfo[256];

void* localMalloc(DWORD aSize) {
	void* aAddr;
	if ((aAddr = malloc(aSize))) {
		mallocsInfo[mallocsIndex].Address = aAddr;
		mallocsInfo[mallocsIndex].Size = aSize;
		mallocsIndex++;
		mallocsMade++;
	}
	return aAddr;
}

void localFree(void* aAddr) {
	int i;
	if (aAddr) {
		for (i = 0; i <= mallocsIndex; i++) {
			if (mallocsInfo[i].Address == aAddr) {
				free(aAddr);
				mallocsInfo[i].Address = 0;
				mallocsInfo[i].Size = 0;
				mallocsMade--;
				break;
			} else if (i == mallocsIndex) { GlobalError("Unknown Free Address"); }
		}
	} else { GlobalError("Freed Zero Pointer"); }
}

#else

void* localMalloc(DWORD aSize) { return malloc(aSize); }
void  localFree(void* aData) { return free(aData); }

#endif

#define DELIMITERCHARSTR "~"
#define MAXLINELENGTH 40
void fillDelimiterLine(void) {
	int i;
	debug("\n");
	for (i=0; i<MAXLINELENGTH; i++) { debug(DELIMITERCHARSTR); }
	debug("\n");
}

#ifdef FUNCTION_IGNORE_GLOBAL_ERRORS
int GlobalError(char* errorDesc) { if (errorDesc) { printf("Error: %s\n", errorDesc); return -1; } else { printf("No Error\n"); return 0; } }
#else
int GlobalError(char* errorDesc) { if (errorDesc) { printf("Error: %s\n", errorDesc); exit(-1); } else { printf("No Error\n"); return 0; } }
#endif


