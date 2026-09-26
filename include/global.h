#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "setup.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int GlobalError(char* errorDesc);

typedef unsigned long long QWORD;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

/* Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C" book. */
#define absx(v)  ((v) < 0 ? -(v) : (v))
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))

#ifdef BIG_ENDIAN
#define IPv4Struct struct { BYTE aIP1; BYTE aIP2; BYTE aIP3; BYTE aIP4; }
#else
#define IPv4Struct struct { BYTE aIP4; BYTE aIP3; BYTE aIP2; BYTE aIP1; }
#endif

typedef struct {
	union {
		BYTE bytes[16];
		WORD words[8];
		DWORD dwords[4];
	};
} aHash;

extern void*   localMalloc(DWORD size);
extern void    localFree(void* addr);

#ifdef DEBUG_MALLOC
extern int mallocsMade;
#endif

#define xMalloc(X)							localMalloc(X)
#define xFree(X)							localFree(X)
#define xFreeN(X)							localFree(X); X=NULL
#define xFreeNN(X)							if (X) { localFree(X); X=NULL; }

#ifndef true
#define true (1)
#endif

#ifndef false
#define false (0)
#endif

#ifndef NULL
#define NULL ((void*)(0))
#endif

#define NULLTEXT	"(null)"
#define TEXT(A) ((char*)(A))
#define TEXT_MEMORY_LEAK TEXT("Memory Leak\n")
#define TEXT_NO_STRING TEXT("No String\n")
#define TEXT_FREE_NULLPOINTER TEXT("Tried to free NULL pointer\n")

#define AddBlock(A,B) A = (BYTE*)xMalloc(B); if (!(A)) { GlobalError(TEXT_MEMORY_LEAK); }
#define AddBlockClean(A,B) AddBlock(A,B); memset(A,0,B)
#define AddString(A,B) A = (char*)xMalloc(B); if (!(A)) { GlobalError(TEXT_MEMORY_LEAK); } *(A) = 0; *((A)+(B)-1) = 0
#define AddStringCopy(A,B) if (!B) { GlobalError(TEXT_NO_STRING); } A = (char*)xMalloc(strlen(B)+1); if (!A) { GlobalError(TEXT_MEMORY_LEAK); } strcpy(A,B)
#define AddStringCopyFixed(A,B,C) AddString(A,((C)+1)); memcpy(A,B,C)
#define AddObjectSized(A,B,C) A = (B*)xMalloc(C); if (!(A)) { GlobalError(TEXT_MEMORY_LEAK); } *((char*)(A)) = 0
#define AddObjectArray(A,B,C) AddObjectSized(A,B,(sizeof(B)*(C)))
#define AddObjectArrayClean(A,B,C) AddObjectArray(A,B,C); memset(A,0,(sizeof(B)*(C)))
#define AddObject(A,B) AddObjectSized(A,B,sizeof(B))
#define AddObjectCopy(A,B,C) AddObject(A,B); memcpy(A,C,sizeof(B))
#define AddObjectClean(A,B) AddObject(A,B); memset(A,0,sizeof(B))
#define NewBlock(A,B) BYTE* A; AddBlock(A,B)
#define NewBlockClean(A,B) NewBlock(A,B); memset(A,0,B)
#define NewBlockCopy(A,B,C) NewBlock(A,C); memcpy(A,B,C)
#define NewString(A,B) char* A; AddString(A,B)
#define NewStringCopy(A,B) char* A; AddStringCopy(A,B)
#define NewStringCopyFixed(A,B,C) NewString(A,((C)+1)); memcpy(A,B,C)
#define NewObject(A,B) B* A; AddObject(A,B)
#define NewObjectArray(A,B,C) B* A; AddObjectSized(A,B,(sizeof(B)*(C)))
#define NewObjectCopy(A,B,C) NewObject(A,B); memcpy(A,C,sizeof(B))
#define NewObjectClean(A,B) NewObject(A,B); memset(A,0,sizeof(B))
#define UniDelete(A) xFreeNN(A)

extern void fillDelimiterLine(void);

#endif /* GLOBAL_H_ */
