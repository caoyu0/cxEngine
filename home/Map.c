//
//  Map.c
//  Home
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <algorithm/cxTile.h>
#include <views/cxLabelTTF.h>
#include <views/cxSprite.h>
#include <engine/cxEngine.h>
#include "Map.h"
#include "Node.h"

static cxBool MapTouch(cxAny pview,cxTouch *touch)
{
    cxEngine engine = cxEngineInstance();
    Map this = pview;
    cxVec2f cpos;
    if(!cxViewHitTest(pview, touch->current, &cpos)){
        return false;
    }
    cxSize2f msize = cxViewSize(this);
    cxSize2f wsize = engine->winsize;
    if(touch->type == cxTouchTypeDown){
        cxVec2f scale = cxViewScale(this);
        this->box.r = (msize.w * scale.x - wsize.w)/2.0f;
        this->box.l = -this->box.r;
        this->box.t = (msize.h * scale.y - wsize.h)/2.0f;
        this->box.b = -this->box.t;
        return false;
    }
    if(touch->type == cxTouchTypeMove){
        cxVec2f vpos = cxViewPosition(this);
        cxVec2f delta = cxViewTouchDelta(pview, touch);
        kmVec2Add(&vpos, &vpos, &delta);
        if(CX_TOUCH_IS_DOWN(touch) && vpos.y <= this->box.b){
            vpos.y = this->box.b;
        }else if(CX_TOUCH_IS_UP(touch) && vpos.y >= this->box.t){
            vpos.y = this->box.t;
        }
        if(CX_TOUCH_IS_LEFT(touch) && vpos.x <= this->box.l){
            vpos.x = this->box.l;
        }else if(CX_TOUCH_IS_RIGHT(touch) && vpos.x >= this->box.r){
            vpos.x = this->box.r;
        }
        cxViewSetPos(this, vpos);
        return true;
    }
    return false;
}

static cxInt MapSortCmpFunc(cxListElement *lp,cxListElement *rp)
{
    cxView v1 = (cxView)lp->any;
    cxView v2 = (cxView)rp->any;
    return v2->position.y - v1->position.y;
}

void MapSortNode(Map this)
{
    cxListSort(this->cxAtlas.cxSprite.cxView.subViews, MapSortCmpFunc);
}

CX_OBJECT_TYPE(Map, cxAtlas)
{
    
}
CX_OBJECT_INIT(Map, cxAtlas)
{
    CX_METHOD_SET(this->cxAtlas.cxSprite.cxView.Touch, MapTouch);
    //set size
    cxEngine engine = cxEngineInstance();
    this->unitNum = cxVec2iv(20, 20);
    this->items = allocator->calloc(this->unitNum.x * this->unitNum.y,sizeof(cxAny));
    this->nodes = CX_ALLOC(cxList);
    
    cxSize2f size = cxSize2fv(engine->winsize.w * 1.2f, 0);
    size.h = size.w * 0.75f;
    this->unitSize = cxSize2fv(size.w/this->unitNum.x, size.h/this->unitNum.y);
    cxViewSetSize(this, size);
    
    cxViewSetScale(this, cxVec2fv(2.0f, 2.0f));
    cxViewSetBorderColor(this, cxYELLOW);
    //
    
    //test
    cxSpriteSetTextureURL(this, "bg1.png", false);
    for(cxInt x = 0; x < this->unitNum.x; x++){
        for (cxInt y = 0; y < this->unitNum.y; y++) {
            cxVec2f pos = MapIdxToPos(this, cxVec2fv(x, y));
            cxAtlasAppendBoxPoint(this, pos, this->unitSize, cxBoxTex2fDefault(), cxColor4fv(1, 1, 1, 1));
        }
    }
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(3, 3),cxVec2fv(8, 8));
        cxSpriteSetTextureURL(node, "bg1.png", false);
        cxViewSetColor(node, cxRED);
        MapAppendNode(this, node);
    }
    
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(4, 4),cxVec2fv(0, 0));
        cxSpriteSetTextureURL(node, "bg1.png", false);
        cxViewSetColor(node, cxRED);
        MapAppendNode(this, node);
    }
    
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(1, 1),cxVec2fv(18, 18));
        cxSpriteSetTextureURL(node, "bg1.png", false);
        cxViewSetColor(node, cxRED);
        MapAppendNode(this, node);
    }
    
    {
        Node node = NodeCreate(this);
        NodeInit(node, cxSize2fv(2, 2),cxVec2fv(7, 18));
        cxSpriteSetTextureURL(node, "bg1.png", false);
        cxViewSetColor(node, cxRED);
        MapAppendNode(this, node);
    }
    
    //按Y位置排序
    MapSortNode(this);
}
CX_OBJECT_FREE(Map, cxAtlas)
{
    CX_RELEASE(this->nodes);
    allocator->free(this->items);
}
CX_OBJECT_TERM(Map, cxAtlas)

void MapNodeIdxChanged(Map this,cxAny node)
{
    
}

void MapAppendNode(Map this,cxAny node)
{
    Node snode = node;
    cxViewAppend(this, snode);
    snode->element = cxListAppend(this->nodes, snode);
}

cxInt MapOffsetIdx(Map this,cxInt x,cxInt y)
{
    return y * this->unitNum.x + x;
}
                            
cxAny MapItem(Map this,cxVec2f idx)
{
    if(!MapIsValidIdx(this, idx)){
        return NULL;
    }
    cxInt off = MapOffsetIdx(this, idx.x, idx.y);
    return this->items[off];
}

cxBool MapRemoveNode(Map this,cxAny node)
{
    CX_ASSERT(node != NULL, "node null");
    Node n = node;
    if(n->map != this){
        return false;
    }
    cxInt off = 0;
    cxSize2i size = NodeSize(node);
    cxVec2i curr = NodeIndex(node);
    for(cxInt x = curr.x; x < curr.x + size.w; x ++){
        for (cxInt y = curr.y; y < curr.y + size.h; y++) {
            off = MapOffsetIdx(this, x, y);
            this->items[off] = NULL;
        }
    }
    n->map = NULL;
    if(n->element != NULL){
        cxListRemove(this->nodes, n->element);
        n->element = NULL;
    }
    cxViewRemoved(node);
    return true;
}

void MapSetNode(Map this,cxVec2i idx,cxAny node)
{
    Node snode = node;
    cxSize2i size = NodeSize(node);
    if(!MapIsValidIdx(this, snode->idx)){
        goto setnewpos;
    }
    //清楚旧位置上的建筑
    cxVec2i curr = NodeIndex(node);
    for(cxInt x = curr.x; x < curr.x + size.w; x ++){
        for (cxInt y = curr.y; y < curr.y + size.h; y++) {
            cxInt off = MapOffsetIdx(this, x, y);
            this->items[off] = NULL;
        }
    }
setnewpos:
    //将建筑设置到新位置
    for(cxInt x = idx.x; x < idx.x + size.w; x ++){
        for (cxInt y = idx.y; y < idx.y + size.h; y++) {
            cxInt off = MapOffsetIdx(this, x, y);
            this->items[off] = node;
        }
    }
}

cxVec2f MapPosToIdx(Map this,cxVec2f pos)
{
    cxSize2f size = cxViewSize(this);
    pos.y += (size.h - this->unitSize.h)/2.0f;
    return cxTilePosToIdx(pos, this->unitSize);
}

cxBool MapIsValidIdx(Map this,cxVec2f idx)
{
    return idx.x >= 0 && idx.x < this->unitNum.x && idx.y >= 0 && idx.y < this->unitNum.y;
}

cxVec2f MapIdxToPos(Map this,cxVec2f idx)
{
    cxSize2f size = cxViewSize(this);
    cxVec2f pos = cxTileIdxToPos(idx, this->unitSize);
    pos.y -= (size.h - this->unitSize.h)/2.0f;
    return pos;
}











