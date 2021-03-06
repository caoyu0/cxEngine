//
//  cxScroll.h
//  cxEngine
//
//  Created by xuhua on 11/5/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxScroll_h
#define cxEngine_cxScroll_h

#include <engine/cxView.h>

CX_C_BEGIN

typedef enum {
    cxScrollMoveTypeNone        = 0,
    cxScrollMoveTypeHorizontal  = 1 << 0,
    cxScrollMoveTypeVertical    = 1 << 1,
}cxScrollMoveType;

CX_DEF(cxScroll, cxView)
    //move data
    cxFloat speed;
    cxFloat moveTime;
    cxBool isEnable;
    cxBox4f box;
    cxScrollMoveType type;
    //can move view
    CX_FIELD_DEF(cxAny Body);
    //scale data
    cxFloat scaleTime;
    cxFloat scaleinc;           //increase scale
    cxBool scalable;            //suppot body scale
    cxFloat scaling;
    cxRange2f range;
    cxFloat startDis;
CX_END(cxScroll, cxView)

CX_FIELD_GET(cxScroll, cxAny, Body);
void cxScrollSetBody(cxAny pview,cxAny body);

void cxScrollUpdateBox(cxAny pview);

cxBool cxScrollCheckPos(cxAny pview,cxVec2f *pos);

void cxScrollLocationTo(cxAny pview,cxVec2f pos,cxBool animate);

CX_C_END

#endif
