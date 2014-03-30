//
//  cxFollow.h
//  cxEngine
//
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFollow_h
#define cxEngine_cxFollow_h

#include <core/cxAction.h>

CX_C_BEGIN

typedef cxBool (cxFollowExitFunc)(cxAny pav);

CX_OBJECT_DEF(cxFollow, cxAction)
    cxAny target;
    cxFloat angle;
    cxFloat speed;
    cxFloat initSpeed;
    cxFloat minDistance;
    CX_METHOD_ALLOC(cxBool, Exit, cxAny);
    CX_METHOD_ALLOC(cxFloat, Speed,cxAny,cxFloat);
CX_OBJECT_END(cxFollow)

void cxFollowInit(cxAny pav);

void cxFollowSetInitSpeed(cxAny pav,cxFloat speed);

cxBool cxFollowDefaultExit(cxAny pav);

void cxFollowSetVec2f(cxAny pav,cxVec2f pt);

void cxFollowSetTarget(cxAny pav,cxAny target);

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target);

cxAny cxFollowTarget(cxAny pav);

CX_C_END

#endif