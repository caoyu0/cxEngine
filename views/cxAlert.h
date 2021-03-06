//
//  cxAlert.h
//  cxCore
//
//  Created by xuhua on 6/1/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxCore_cxAlert_h
#define cxCore_cxAlert_h

#include "cxAtlas.h"

CX_C_BEGIN

CX_DEF(cxAlert, cxAtlas)
    CX_EVENT_ALLOC(OnShow);
    CX_EVENT_ALLOC(OnHide);
CX_END(cxAlert, cxAtlas)

void cxAlertFireShow(cxAny pview);

void cxAlertFireHide(cxAny pview);

CX_C_END

#endif
