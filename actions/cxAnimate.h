//
//  cxAnimate.h
//  cxEngine
//
//  Created by xuhua on 11/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAnimate_h
#define cxEngine_cxAnimate_h

#include <engine/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxAnimateItem, cxObject)
    cxTexture texture;
    cxString key;
    cxString id;
    cxFloat time;
    cxFloat value;
    cxBool flipx;
    cxBool flipy;
CX_OBJECT_END(cxAnimateItem, cxObject)

CX_OBJECT_DEF(cxAnimate, cxAction)
    cxHash groups;              //array item
    cxHash frames;
    cxInt index;
    cxFloat time;
    cxBool forever;
    cxString name;              //group name
    CX_EVENT_ALLOC(onFrame);    //play frame competed index=0 - (n-1)
CX_OBJECT_END(cxAnimate, cxAction)

cxArray cxAnimateGetGroup(cxAny pav,cxString name);

void cxAnimateRunGroup(cxAny pav,cxConstChars name);

CX_C_END

#endif
