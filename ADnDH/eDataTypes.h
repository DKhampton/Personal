
#ifndef EDATATYPES_H_
#ifndef XENUM_IMPLEMENT_MODE
#define EDATATYPES_H_
#endif

#include "eXEnum.h"

XENUM_BEGIN(DataTypes)
XENUM_LINE(eDTVoid, "VOID", 0)
XENUM_LINE(eDTCollection, "COLLECTION", 1)
XENUM_LINE(eDTReference, "REFERENCE", 2)
XENUM_LINE(eDTReferedID, "REFEREDID", 3)
XENUM_LINE(eDTReserved0, "RESERVED0", 4)
XENUM_LINE(eDTReserved1, "RESERVED1", 5)
XENUM_LINE(eDTReserved2, "RESERVED2", 6)
XENUM_LINE(eDTReserved3, "RESERVED3", 7)
XENUM_LINE(eDTByte, "BYTE", 8)
XENUM_LINE(eDTChar, "CHAR", 9)
XENUM_LINE(eDTWord, "WORD", 10)
XENUM_LINE(eDTShort, "SHORT", 11)
XENUM_LINE(eDTDword, "DWORD", 12)
XENUM_LINE(eDTInt, "INT", 13)
XENUM_LINE(eDTString, "STR", 14)
XENUM_LINE(eDTsCharacter, "S_CHAR", 15)
XENUM_LINE(eDTsMemFile, "S_MEMF", 16)
XENUM_LINE(eDTCOUNT, "COUNTER", 17)
XENUM_END(DataTypes)

#endif /* EDATATYPES_H_ */
