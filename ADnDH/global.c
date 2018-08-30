
#include "global.h"

#ifdef DEBUG_INFO

typedef struct {
	void* Address;
	DWORD Size;
} mallocsData;
int mallocsIndex = 0;
int mallocsMade = 0;
mallocsData mallocsInfo[256];

void* xMalloc(DWORD aSize) {
	void* aAddr;
	if ((aAddr = malloc(aSize))) {
		mallocsInfo[mallocsIndex].Address = aAddr;
		mallocsInfo[mallocsIndex].Size = aSize;
		mallocsIndex++;
		mallocsMade++;
	}
	return aAddr;
}

void xFree(void* aAddr) {
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

#endif

void fillDelimiterLine(void) {
	int i;
	printf("\n");
	for (i=0; i<MAXLINELENGTH; i++) { printf(DELIMITERCHARSTR); }
	printf("\n");
}

void GlobalError(char* errorDesc) { printf("Error: %s\n", errorDesc); exit(-1); }






























