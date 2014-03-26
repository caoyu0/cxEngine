//
//  dmSprite.c
//  dome
//
//  Created by xuhua on 3/21/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcMap.h"
#include "fcSprite.h"

CX_OBJECT_INIT(fcSprite, cxSprite)
{
    this->targets = CX_ALLOC(cxHash);
    this->murderers = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(fcSprite, cxSprite)
{
    CX_RELEASE(this->targets);
    CX_RELEASE(this->murderers);
}
CX_OBJECT_TERM(fcSprite, cxSprite)

void fcSpriteUnsetTarget(cxAny this)
{
    fcSprite s = this;
    cxHashKey key = cxHashIntKey((cxInt)s);
    //解除目标是我
    CX_HASH_FOREACH(s->targets, sele, stmp){
        fcSprite st = sele->any;
        cxHashDel(st->murderers, key);
    }
    //解除我的所有的目标
    cxHashClean(s->targets);
    //解除凶手的目标是我的
    CX_HASH_FOREACH(s->murderers, tele, ttmp){
        fcSprite tt = tele->any;
        cxHashDel(tt->targets,key);
    }
    //解除我的所有的凶手
    cxHashClean(s->murderers);
}

void fcSpriteSetTarget(cxAny this,cxAny target)
{
    CX_ASSERT(this != target, "target can't this");
    fcSprite s = this;
    fcSprite t = target;
    cxHashKey sk = cxHashIntKey((cxInt)s);
    cxHashKey tk = cxHashIntKey((cxInt)t);
    //设置目标的凶手我自己
    cxHashSet(t->murderers, sk, s);
    //设置我的目标为target
    cxHashSet(s->targets, tk, t);
}

void fcSpriteInit(cxAny sprite,cxAny map,cxVec2i idx)
{
    fcMap dm = map;
    fcSprite this = sprite;
    cxVec2f pos = fcMapToPos(dm, idx);
    cxViewSetPos(this, pos);
    cxViewSetSize(this, dm->gridSize);
    this->map = map;
    this->idx = idx;
}

cxTimer fcSpriteTimer(cxAny this,cxFloat time)
{
    return cxViewAppendTimer(this, time, -1);
}

