#pragma once
#include "types.h"

#ifndef __cplusplus
#ifndef bool
typedef u8 bool;
#endif

#ifndef true
#define true ((bool) 1)
#endif

#ifndef false
#define false ((bool) 0)
#endif
#endif