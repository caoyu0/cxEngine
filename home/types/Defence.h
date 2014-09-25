//
//  Defence.h
//  Home
//  被动防御单位炮塔，防御单位
//  Created by xuhua on 9/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Defence_h
#define Home_Defence_h

#include <Node.h>

CX_C_BEGIN

CX_OBJECT_DEF(Defence, Node)
    cxInt attackNum;        //同时攻击数量

    //移动方向变化时
    cxInt index;                //当前的角度索引
    CX_EVENT_ALLOC(onIndex);
    //攻击目标时
    cxAny target;               //当前攻击的目标
    CX_EVENT_ALLOC(onAttack);
CX_OBJECT_END(Defence, Node)

void DefenceSearch(cxAny pview);

void DefenceAttack(cxAny pview);

Defence DefenceCreate(cxAny map,cxSize2f size,cxVec2f pos);

CX_C_END

#endif
