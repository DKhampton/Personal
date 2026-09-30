#include "global.h"

#include <stdarg.h>

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

bool isSilentMode = false;
void nothing(char* fmt, ...) { }
void console(char* fmt, ...) {
	if (!isSilentMode) {
		va_list args;
		va_start(args, fmt);
		vprintf(fmt, args);
		va_end(args);
	}
}

void fillDelimiterLine(void (*outputFunc)(char* fmt, ...)) {
	int i;
	outputFunc("\n");
	for (i=0; i<CONSOLE_LINE_MAXLENGTH; i++) { outputFunc(CONSOLE_CMD_DELIMITERCHARSTR); }
	outputFunc("\n");
}

void consoleFillDelimiterLine() { fillDelimiterLine(console); }

#ifdef FUNCTION_GLOBAL_ERRORS_STUCK
#define FGEEOS_TEMPORARY for(;;)
#else
#define FGEEOS_TEMPORARY exit(-1)
#endif

int GlobalError(char* errorDesc) { if (errorDesc) { debug("Error: %s\n", errorDesc); } else { debug("Empty Error\n"); } FGEEOS_TEMPORARY; }
void GlobalCheckNull(void* ptr) { if (!ptr) GlobalError(TEXT_MEMORY_LEAK); }