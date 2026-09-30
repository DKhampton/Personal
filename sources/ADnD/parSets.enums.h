#if (!defined EPARSETS_H_) || (defined XENUM_IMPLEMENT_MODE)
#ifndef XENUM_IMPLEMENT_MODE
#define EPARSETS_H_
#endif

#include "enums.h"

XENUM_BEGIN(ParSets)
XENUM_LINE(epsVoid, "Void", 0)
XENUM_LINE(epsBase, "Base", 1)
XENUM_LINE(epsTemporary, "Temporary", 2)
XENUM_LINE(epsUseful, "Useful", 3)
XENUM_LINE(epsCOUNT, "COUNT", 4)
XENUM_END(ParSets)

#endif /* EPARSETS_H_ */
