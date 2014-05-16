//
//  cxFollow.h
//  cxEngine
//
//  Created by xuhua on 3/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFollow_h
#define cxEngine_cxFollow_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxFollow, cxAction)
    cxBool isNum;
    cxAny target;   //follow target, cxNumber(cxVec2f) or cxView
    cxFloat angle;  //current angle
    cxFloat speed;  //current speed
    cxFloat init;   //init speed
    cxFloat min;    //exit min distance
    CX_METHOD_DEF(cxBool, Exit, cxAny);
    CX_METHOD_DEF(cxFloat, Speed,cxAny,cxFloat);
CX_OBJECT_END(cxFollow)

void cxFollowInit(cxAny pav);

void cxFollowSetInit(cxAny pav,cxFloat init);

cxBool cxFollowDefaultExit(cxAny pav);

void cxFollowSetVec2f(cxAny pav,cxVec2f pt);

void cxFollowSetTarget(cxAny pav,cxAny target,cxBool isNum);

cxFollow cxFollowCreate(cxFloat initSpeed,cxAny target,cxBool isNum);

cxAny cxFollowTarget(cxAny pav);

CX_C_END

#endif
