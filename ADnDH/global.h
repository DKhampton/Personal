#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "setup.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern void fillDelimiterLine(void);
extern void GlobalError(char* errorDesc);

typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;

typedef struct {
	union {
		BYTE bytes[16];
		WORD words[8];
		DWORD dwords[4];
	};
} aHash;

#ifdef DEBUG_INFO
extern int mallocsMade;
#define xDebug(...) printf(__VA_ARGS__)
void*   xMalloc(DWORD size);
void    xFree(void* addr);
#else
#define xDebug(...) {}
#define xMalloc(A) malloc(A);
#define xFree(A) free(A);
#endif

#define AddBlock(A,B) A = xMalloc(B); if (!A) { GlobalError("Memory Leak"); }
#define AddString(A,B) AddBlock(A,B); *A = 0; *(A+B-1) = 0
#define AddStringCopy(A,B) if (!B) { GlobalError("No String"); } A = xMalloc(strlen(B)+1); if (!A) { GlobalError("Memory Leak"); } strcpy(A,B)
#define NewString(A,B) char* A; AddString(A,B)
#define NewStringCopy(A,B) char* A; AddStringCopy(A,B)
#define NewFixedCopy(A,B,C) NewString(A,C+1); memcpy(A,B,C)
#define AddObject(A,B) A = xMalloc(sizeof(B)); if (!A) { GlobalError("Memory Leak"); } *((char*)A) = 0
#define AddObjectCopy(A,B,C) AddObject(A,B); memcpy(A,C,sizeof(B))
#define AddCleanObject(A,B) AddObject(A,B); memset(A,0,sizeof(B))
#define NewBlock(A,B) void* A; AddBlock(A,B);
#define NewCleanBlock(A,B) NewBlock(A,B); memset(A,0,B);
#define AddCleanBlock(A,B) AddBlock(A,B); memset(A,0,B);
#define NewObject(A,B) B* A; AddObject(A,B)
#define NewObjectCopy(A,B,C) NewObject(A,B); memcpy(A,C,sizeof(B))
#define NewCleanObject(A,B) NewObject(A,B); memset(A,0,sizeof(B))
#define Delete(A) if (A) { free(A); A = NULL; } else { GlobalError("Tried to free NULL pointer"); }

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void*)(0))
#endif

#endif /* GLOBAL_H_ */
