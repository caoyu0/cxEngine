//
//  cxSpline.h
//  cxEngine
//
//  Created by xuhua on 12/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSpline_h
#define cxEngine_cxSpline_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_DEF(cxSpline, cxAction)
    cxVec2f diff;
    cxVec2f prev;
    cxAnyArray points;
    cxFloat tension;
    cxFloat delta;
    cxInt index;
    cxFloat angle;
    CX_EVENT_ALLOC(onIndex);
    CX_EVENT_ALLOC(onAngle);
CX_END(cxSpline, cxAction)

cxVec2f cxSplinePointAt(cxAny pav,cxInt idx);

//remove last num point
void cxSplineRemove(cxAny pav,cxInt num);

void cxSplineAppend(cxAny pav,cxVec2f pos);

cxSpline cxSplineCreate(cxFloat time, cxAnyArray points);

CX_C_END

#endif
