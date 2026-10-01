#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "setup.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>	

typedef unsigned int bool;
typedef unsigned long long QWORD;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

/* Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C" book. */
#define abs(v) 		((v)<0?(-(v)):(v))
#define min(x,y) 	(((x)<(y))?(x):(y))
#define max(x,y) 	(((x)>(y))?(x):(y))

#ifdef BIG_ENDIAN
#define defsIPv4 struct { BYTE aIP1; BYTE aIP2; BYTE aIP3; BYTE aIP4; }
#else
#define defsIPv4 struct { BYTE aIP4; BYTE aIP3; BYTE aIP2; BYTE aIP1; }
#endif

typedef defsIPv4 sIPv4;

typedef struct {
	union {
		BYTE bytes[16];
		WORD words[8];
		DWORD dwords[4];
	};
} aHash;

extern void 	GlobalCheckNull(void* ptr);
extern int 		GlobalError(char* errorDesc);
extern void*	localMalloc(DWORD size);
extern void		localFree(void* addr);
extern bool		isSilentMode;
extern void		console(char* fmt, ...);
extern void		consoleFillDelimiterLine(void);

#ifdef DEBUG_MODE
	#define debug(...) console(__VA_ARGS__)
	#define debugFillDelimiterLine() consolefillDelimiterLine()
#else
	#define debug(...) { }
	#define debugFillDelimiterLine() { }
#endif

#ifdef DEBUG_MALLOC
extern int mallocsMade;
#endif

#define xMalloc(XXX)		localMalloc(XXX)
#define xFree(XXX)			localFree(XXX)
#define xFreeN(XXX)			localFree(XXX); XXX=NULL
#define xFreeNN(XXX)		if (XXX) { localFree(XXX); XXX=NULL; }

#ifdef true
#undef true
#endif
#define true (1)

#ifdef false
#undef false
#endif
#define false (0)

#ifdef NULL
#undef NULL
#endif
#define NULL ((void*)(0))

#define NULLTEXT					"(null)"
#define TEXT(A) 					((char*)(A))
#define TEXT_MEMORY_LEAK 			TEXT("Memory Leak\n")
#define TEXT_NO_STRING 				TEXT("No String\n")
#define TEXT_FREE_NULLPOINTER 		TEXT("Tried to free NULL pointer\n")
;;

#define AddBlockTyped(AAA,BBB,CCC) AAA = (CCC)xMalloc(BBB); GlobalCheckNull(AAA)
#define AddBlock(XXX,YYY) AddBlockTyped(XXX,YYY,BYTE*)
#define AddBlockClean(XXX,YYY) AddBlock(XXX,YYY); memset(XXX,0,YYY)
#define AddString(XXX,YYY) AddBlockTyped(XXX,YYY,char*); *(XXX) = 0; *((XXX)+(YYY)-1) = 0
#define AddStringCopy(XXX,YYY) GlobalCheckNull(YYY); AddBlockTyped(XXX,(strlen(YYY)+1),char*); GlobalCheckNull(XXX); strcpy(XXX, YYY)
#define AddStringCopyFixed(XXX,YYY,ZZZ) AddString(XXX,((ZZZ)+1)); memcpy(XXX,YYY,ZZZ); *((XXX)+(ZZZ)) = 0	
#define AddObjectClean(XXX,YYY) AddBlockTyped(XXX,sizeof(YYY),YYY*); memset(XXX,0,sizeof(YYY))
#define AddObject(XXX,YYY) AddObjectClean(XXX,YYY)
#define AddObjectArray(XXX,YYY,ZZZ) AddBlockTyped(XXX,(sizeof(YYY)*(ZZZ)),YYY*)
#define AddObjectArrayClean(XXX,YYY,ZZZ) AddObjectArray(XXX,YYY,ZZZ); memset(XXX,0,(sizeof(YYY)*(ZZZ)))
#define AddObjectCopy(XXX,YYY,ZZZ) AddObject(XXX,YYY); memcpy(XXX,ZZZ,sizeof(YYY))
#define NewBlock(XXX,YYY) BYTE* XXX; AddBlock(XXX,YYY)
#define NewBlockClean(XXX,YYY) NewBlock(XXX,YYY); memset(XXX,0,YYY)
#define NewBlockCopy(XXX,YYY,ZZZ) NewBlock(XXX,YYY); memcpy(XXX,ZZZ,YYY)
#define NewString(XXX,YYY) char* XXX; AddString(XXX,YYY)
#define NewStringCopy(XXX,YYY) char* XXX; AddStringCopy(XXX,YYY)
#define NewStringCopyFixed(XXX,YYY,ZZZ) NewString(XXX,((ZZZ)+1)); memcpy(XXX,YYY,ZZZ)
#define NewObject(XXX,YYY) YYY* XXX; AddObject(XXX,YYY)
#define NewObjectArray(XXX,YYY,ZZZ) YYY* XXX; AddObjectArray(XXX,YYY,ZZZ)
#define NewObjectCopy(XXX,YYY,ZZZ) NewObject(XXX,YYY); memcpy(XXX,ZZZ,sizeof(YYY))
#define NewObjectClean(XXX,YYY) NewObject(XXX,YYY); memset(XXX,0,sizeof(YYY))
#define UniDelete(XXX) xFreeNN(XXX)

#endif /* GLOBAL_H_ */
