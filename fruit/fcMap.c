//
//  fcMap.c
//  dome
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//
#include <algorithm/cxAStar.h>
#include <core/cxEngine.h>
#include "fcMap.h"
#include "fcAttacker.h"
#include "fcFruit.h"
#include "fcFollow.h"

static void PathNodeNeighbors(ASNeighborList neighbors, void *node, void *context)
{
    fcMap map = context;
    cxVec2i *pathNode = (cxVec2i *)node;
    //left
    cxVec2i l = cxVec2iv(pathNode->x - 1, pathNode->y);
    cxInt lv = fcMapValue(map, l);
    if(lv == 0){
        ASNeighborListAdd(neighbors, &l, 1);
    }
    //right
    cxVec2i r = cxVec2iv(pathNode->x + 1, pathNode->y);
    cxInt rv = fcMapValue(map, r);
    if(rv == 0){
        ASNeighborListAdd(neighbors, &r, 1);
    }
    //up
    cxVec2i u = cxVec2iv(pathNode->x, pathNode->y + 1);
    cxInt uv = fcMapValue(map, u);
    if(uv == 0){
        ASNeighborListAdd(neighbors, &u, 1);
    }
    //down
    cxVec2i d = cxVec2iv(pathNode->x, pathNode->y - 1);
    cxInt dv = fcMapValue(map, d);
    if(dv == 0){
        ASNeighborListAdd(neighbors, &d, 1);
    }
    //left up
    cxVec2i lu = cxVec2iv(pathNode->x - 1, pathNode->y + 1);
    cxInt luv = fcMapValue(map, lu);
    if(luv == 0 && (lv ==0 || uv == 0)){
        ASNeighborListAdd(neighbors, &lu, 1);
    }
    //right up
    cxVec2i ru = cxVec2iv(pathNode->x + 1, pathNode->y + 1);
    cxInt ruv = fcMapValue(map, ru);
    if(ruv == 0 && (rv ==0 || uv == 0)){
        ASNeighborListAdd(neighbors, &ru, 1);
    }
    //left down
    cxVec2i ld = cxVec2iv(pathNode->x - 1, pathNode->y - 1);
    cxInt ldv = fcMapValue(map, ld);
    if(ldv == 0 && (lv ==0 || dv == 0)){
        ASNeighborListAdd(neighbors, &ld, 1);
    }
    //right down
    cxVec2i rd = cxVec2iv(pathNode->x + 1, pathNode->y - 1);
    cxInt rdv = fcMapValue(map, rd);
    if(rdv == 0 && (rv ==0 || dv == 0)){
        ASNeighborListAdd(neighbors, &rd, 1);
    }
}

static int PathEarlyExit(size_t visitedCount, void *visitingNode, void *goalNode, void *context)
{
    return 0;
}

static float PathNodeHeuristic(void *fromNode, void *toNode, void *context)
{
    cxVec2i *from = (cxVec2i *)fromNode;
    cxVec2i *to = (cxVec2i *)toNode;
    return (fabs(from->x - to->x) + fabs(from->y - to->y));
}

static const ASPathNodeSource PathNodeSource =
{
    sizeof(cxVec2i),
    &PathNodeNeighbors,
    &PathNodeHeuristic,
    &PathEarlyExit,
    NULL
};

cxBool fcMapCheckIdx(cxVec2i idx)
{
    return idx.x >= 0 && idx.x < DM_MAP_WIDTH && idx.y >= 0 && idx.y < DM_MAP_HEIGHT;
}

cxAny fcMapSprite(fcMap this,cxVec2i idx)
{
    if(!fcMapCheckIdx(idx)){
        return NULL;
    }
    return this->sprites[idx.x][idx.y];
}

cxInt fcMapValue(fcMap this,cxVec2i idx)
{
    fcSprite sprite = fcMapSprite(this, idx);
    if(sprite == NULL){
        return 0;
    }
    return fcSpritePathValue(sprite);
}

cxVec2i fcMapToIdx(fcMap this,cxVec2f pos)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = pos.x + size.w / 2.0f;
    cxFloat y = pos.y + size.h / 2.0f;
    return cxVec2iv(x/this->gridSize.w, y/this->gridSize.h);
}

cxVec2f fcMapToPos(fcMap this,cxVec2i idx)
{
    cxSize2f size = cxViewSize(this);
    cxFloat x = idx.x * this->gridSize.w - size.w / 2.0f;
    cxFloat y = idx.y * this->gridSize.h - size.h / 2.0f;
    return cxVec2fv(x + this->gridSize.w/2.0f, y + this->gridSize.h/2.0f);
}

fcSprite m;

static cxBool fcMapTouch(cxAny pview,cxTouch *touch)
{
    fcMap this = pview;
    cxVec2f pos;
    if(!cxViewHitTest(this, touch->current, &pos)){
        return false;
    }
    if(touch->type == cxTouchTypeMove){
        cxViewSetPos(m, pos);
    }
//    if(touch->type == cxTouchTypeDown){
//        cxVec2i idx = fcMapToIdx(this, pos);
//        this->values[idx.x][idx.y] = 1;
//        fcPath path;
//        if(!fcMapFindPath(this, &path, cxVec2iv(0, 0), cxVec2iv(9, 9))){
//            this->values[idx.x][idx.y] = 0;
//            return false;
//        }
//        
//        fcSprite sp = fcSpriteCreate(this, idx);
//        cxViewAppend(this, sp);
//        cxViewAppend(this, sp);
//        cxObjectSetTag(sp, 1);
//        cxSpriteSetImage(sp, "item.xml?blue.png");
//        
//        CX_LIST_FOREACH_SAFE(this->super.subViews, ele, tmp){
//            cxInt tag = cxObjectTag(ele->any);
//            if(tag != 1){
//                cxViewRemoved(ele->any);
//            }
//        }
//        
//        for(cxInt i=0; i < path.number; i++){
//            cxVec2i idx = path.points[i];
//            fcSprite sp = fcSpriteCreate(this, idx);
//            cxSpriteSetImage(sp, "item.xml?white.png");
//            cxViewAppend(this, sp);
//        }
//    }
    return false;
}

cxFloat fcMapScaleValue(fcMap this,cxFloat v)
{
    return this->gridSize.w * v;
}

cxBool fcMapFindPath(fcMap this,fcPath *path,cxVec2i start,cxVec2i stop)
{
    cxBool rv = false;
    ASPath apath = ASPathCreate(&PathNodeSource, this, &start, &stop);
    path->number = 0;
    for (int i=0; i<ASPathGetCount(apath); i++) {
        cxVec2i *pathNode = ASPathGetNode(apath, i);
        path->points[path->number++] = *pathNode;
    }
    rv = (ASPathGetCount(apath) > 0);
    ASPathDestroy(apath);
    return rv;
}

void fcMapAppendFights(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    m->element = cxListAppend(this->fights, sprite);
    cxViewAppend(this, sprite);
}

void fcMapRemoveFights(fcMap this,cxAny sprite)
{
    fcSprite m = sprite;
    if(m->element != NULL){
        cxListRemove(this->fights, m->element);
        m->element = NULL;
    }
    cxViewRemoved(m);
}

static void attackedTest(cxAny sprite,cxAny fruit,cxAny attacker)
{
    fcAttacker s = sprite;
    fcFruit f = fruit;
    CX_LOGGER("%p attacked %p,attacker = %p",f, s, attacker);
//    fcAttackerPauseTime(attacker,5);
//    fcSpriteRemoved(s);
}

static cxBool isAttackMe(cxAny sprite, cxAny fruit,cxAny attacker)
{
    fcSprite s = sprite;
    fcFruit f = fruit;
    CX_LOGGER("%p will attacked %p,use fruit = %p",attacker, s, f);
    return true;
}

static cxAny attackerFruitMaker(cxAny attacker)
{
    fcAttacker this = attacker;
    fcFollow follow = CX_CREATE(fcFollow);
    fcFollowInit(follow, this->super.map, 6);
    cxSpriteSetImage(follow, "item.xml?fire.png");
    return follow;
}

CX_OBJECT_INIT(fcMap, cxView)
{
    this->fights = CX_ALLOC(cxList);
    cxSize2f size = cxEngineInstance()->winsize;
    cxFloat vw = size.w - 20;
    this->gridSize.w = vw / DM_MAP_WIDTH;
    this->gridSize.h = this->gridSize.w;
    cxViewSetSize(this, cxSize2fv(vw, vw));
    
    CX_METHOD_OVERRIDE(this->super.Touch, fcMapTouch);
    //
    {
        fcAttacker a = CX_CREATE(fcAttacker);
        fcAttackerInit(a, this);
        fcSpriteInitIndex(a, cxVec2iv(0, 0));
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, attackerFruitMaker);
        a->super.group = fcGroupTypeDefenser;
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendFights(this, a);
        //开始搜索攻击
        fcAttackerLoop(a);
    }
    
    {
        fcAttacker a = CX_CREATE(fcAttacker);
        fcAttackerInit(a, this);
        fcSpriteInitIndex(a, cxVec2iv(9, 9));
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, attackerFruitMaker);
        a->super.group = fcGroupTypeDefenser;
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendFights(this, a);
        //开始搜索攻击
        fcAttackerLoop(a);
    }
    
    {
        fcAttacker a = CX_CREATE(fcAttacker);
        fcAttackerInit(a, this);
        fcSpriteInitIndex(a, cxVec2iv(0, 9));
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, attackerFruitMaker);
        a->super.group = fcGroupTypeDefenser;
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendFights(this, a);
        //开始搜索攻击
        fcAttackerLoop(a);
    }
    
    {
        fcAttacker a = CX_CREATE(fcAttacker);
        fcAttackerInit(a, this);
        fcSpriteInitIndex(a, cxVec2iv(9, 0));
        cxSpriteSetImage(a, "item.xml?red.png");
        CX_METHOD_OVERRIDE(a->FruitMaker, attackerFruitMaker);
        a->super.group = fcGroupTypeDefenser;
        a->attackRange = 4;
        a->attackNumber = 2;
        a->attackPower = 5;
        fcMapAppendFights(this, a);
        //开始搜索攻击
        fcAttackerLoop(a);
    }
    
    fcSprite b = CX_CREATE(fcSprite);
    fcSpriteInit(b, this);
    fcSpriteInitIndex(b, cxVec2iv(6, 6));
    cxSpriteSetImage(b, "item.xml?blue.png");
    
    CX_METHOD_OVERRIDE(b->IsAttack, isAttackMe);
    CX_METHOD_OVERRIDE(b->Attacked, attackedTest);
    
    b->group = fcGroupTypeAttacker;
    fcMapAppendFights(this, b);

    m = b;
}
CX_OBJECT_FREE(fcMap, cxView)
{
    CX_RELEASE(this->fights);
}
CX_OBJECT_TERM(fcMap, cxView)










