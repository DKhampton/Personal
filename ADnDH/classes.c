/*
 * classes.c
 *
 *  Created on: Mar 26, 2018
 *      Author: denys.prokhorov
 */

#include "global.h"

typedef struct _sList {
	int ValueID;
	void* Value;
	struct _sList* Next;
} sList;

typedef struct {
	BYTE versionMinor;
	BYTE versionMajor;
	WORD versionBuild;
	DWORD dataSize;
	char ClassName[32];
} sClassInfo;

typedef struct {
	sClassInfo classInfo;
	void (*Destructor)(void* this);
	void (*Constructor)(void* this);  // could be later changed to constructor list with types
	struct _sDynamicData* (*Duplicate)(void* this);
	struct _sDynamicData* (LoadFromXML);
	char* (*SaveAsXML)(void* this);
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


/*
sDynamicData* class_findClassByID(int classID) {
	return NULL;
}

int myDynamicTypes[10]; // holds module's dynamic type IDs
sDynamicData* class_createInstance(int classID) {
	DWORD tmpValue;
	NewCleanObject(aInstance,sDynamicData);
	if ((aInstance->classType = class_findClassByID(classID))) {
		if ((tmpValue = aInstance->classType->classStatic->classInfo.dataSize)) {
			AddCleanBlock(aInstance->dataLocation,tmpValue);
		}
	} else Delete(aInstance);
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
*/

//#define New(aVar,sClass) sDynamicData(aVar)=system_SearchType(sClass)->classBody->Constructor


/*
class Probe {

};
*/




































