#include "global.h"

#include "files.h"

typedef struct _sList {
	int ValueID;
	void* Value;
	struct _sList* Next;
} sList;

typedef enum {
	ecerNoError,
	ecerWrongParameters,
	ecerMemoryError
} eClassError;

typedef struct _sDynamicData sDynamicData;

typedef struct {
	char ClassName[32];
	BYTE versionMinor;
	BYTE versionMajor;
	WORD versionBuild;
	DWORD dataSize;
	eClassError (*Destructor)(sDynamicData* this);
	eClassError (*Constructor)(sDynamicData* this);  // could be later changed to constructor list with types
	sDynamicData* (*Duplicate)(sDynamicData* this);
//	sDynamicData* (*LoadFromXML)(sMemFile* xmlFile);
//	sMemFile* (*SaveAsXML)(sDynamicData* this);
} sClassStatic;

typedef struct {
	int classID;
	int classRegisterCount;
	aHash classHash;
	sClassStatic* classStatic;
} sDynamicType;

typedef struct _sDynamicData {
	void* dataLocation; // NULL for no data class
	sDynamicType* classType;
} sDynamicData;

// each Module/Application has it's own section "sDynamicType"
// on load, system will register all listed types and sets typeID in given list
// on unload, system will unregister all listed types

/*
#define New(A,B) A=B;

void probe(int a) {
	New(jopka, sDynamicData);


}
*/
sDynamicType* class_findClassByID(int classID) {
	return NULL;
}

int myDynamicTypes[10]; // holds module's dynamic type IDs
sDynamicData* class_createInstance(int classID) {
	DWORD tmpValue;
	NewObjectClean(aInstance,sDynamicData);
	if ((aInstance->classType = class_findClassByID(classID))) {
		if ((tmpValue = aInstance->classType->classStatic->dataSize)) {
			AddBlockClean(aInstance->dataLocation,tmpValue);
		}
	} else UniDelete(aInstance);
	return aInstance;
}

void class_destroyInstance(sDynamicData* aInstance) {
	if (aInstance->dataLocation) { xFree(aInstance->dataLocation); }
	xFree(aInstance);
}

void system_typeListRegister(sList classList) {

}

void system_typeListUnregister(sList classList) {

}

//#define New(aVar,sClass) sDynamicData(aVar)=system_SearchType(sClass)->classBody->Constructor


/*
class Probe {

};
*/




