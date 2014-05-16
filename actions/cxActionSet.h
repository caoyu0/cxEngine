//
//  cxActionSet.h
//  cxEngine
//
//  Created by xuhua on 10/29/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxActionSet_h
#define cxEngine_cxActionSet_h

#include <engine/cxAction.h>

CX_C_BEGIN

typedef enum {
    cxActionSetTypeNone,        //none
    cxActionSetTypeMultiple,    //multiple
    cxActionSetTypeSequence,    //sequence
}cxActionSetType;

CX_OBJECT_DEF(cxActionSet, cxAction)
    cxActionSetType type;
    cxArray items;
    cxInt index;
CX_OBJECT_END(cxActionSet)

void cxActionSetSetType(cxAny pav,cxActionSetType type);

void cxActionSetAppend(cxAny pav,cxAny action);

CX_C_END

#endif
