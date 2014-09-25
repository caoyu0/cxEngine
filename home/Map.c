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
#include "Move.h"
#include <types/Attack.h>
#include <types/Defence.h>
#include <types/Wall.h>

static cxInt MapSortCmpFunc(cxConstAny lv,cxConstAny rv)
{
    cxListElement *lp = (cxListElement *)lv;
    cxListElement *rp = (cxListElement *)rv;
    cxView v1 = CX_TYPE_CAST(cxView, lp->any);
    cxView v2 = CX_TYPE_CAST(cxView, rp->any);
    return v2->position.y - v1->position.y;
}

void MapNodeOnNewIdx(cxAny pmap,cxAny pnode)
{
//    CX_ASSERT_THIS(pmap, Map);
//    Node node = CX_TYPE_CAST(Node, pnode);
//    CX_LOGGER("%p new idx=%f idx.y=%f",pnode,node->idx.x,node->idx.y);
}

void MapSortNode(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    this->isSort = true;
}


static void MapNodesEach(cxAny ps,cxAny pview,cxAny data)
{
    Node node = pview;
    cxBool *has = data;
    if(!(*has) && node->isSelected){
        *has = true;
    }
    
}
static cxBool hasSelectNode(Map this)
{
    cxBool has = false;
    cxSpatialEach(this->defences, MapNodesEach, &has);
    return has;
}

static cxBool MapEditTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Map);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    Node snode = this->node;
    if(item->type == cxTouchTypeMove){
        return false;
    }
    if(snode == NULL){
        return false;
    }
    if(item->type == cxTouchTypeUp && snode->isValidIdx){
        NodeUpdateIdx(snode);
    }
    cxBool isSelected = hasSelectNode(this);
    if(item->type == cxTouchTypeUp && !isSelected){
        NodeResetPosition(snode);
        cxViewSetColor(snode, cxRED);
        this->node = NULL;
    }
    return false;
}

cxBool MapInit(cxAny pmap,cxJson data)
{
    CX_ASSERT_THIS(pmap, Map);

    cxEngine engine = cxEngineInstance();
    
    cxSize2f size = cxSize2fv(engine->winsize.w * 1.2f, 0);
    size.h = size.w * 0.75f;
    global.unitSize = cxSize2fv(size.w/global.unitNum.x, size.h/global.unitNum.y);
    cxViewSetSize(this, size);
    cxViewSetSize(this->bullet, size);

    
    //test
    cxSpriteSetTextureURL(this, "bg3.png");
    cxBoxTex2f tex = cxBoxTex2fDefault();
    cxVec2f pos = cxVec2fv(size.w / 4, size.h / 4);
    cxColor4f color = cxColor4fv(1, 1, 1, 1);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex, color);
    
    cxBoxTex2f tex1 = cxBoxTex2fFlip(tex, true, false);
    pos = cxVec2fv(-size.w / 4, size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex1, color);

    cxBoxTex2f tex2 = cxBoxTex2fFlip(tex, true, true);
    pos = cxVec2fv(-size.w / 4, -size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex2, color);

    cxBoxTex2f tex3 = cxBoxTex2fFlip(tex, false, true);
    pos = cxVec2fv(size.w / 4, -size.h / 4);
    cxAtlasAppendBoxPoint(this, pos, cxSize2fv(size.w/2.0f, size.h/2.0f), tex3, color);

    //按Y位置排序
    MapSortNode(this);
    return true;
}

static void MapUpdate(cxAny pview)
{
    CX_ASSERT_THIS(pview, Map);
    if(this->isSort){
        cxViewSortWithFunc(this, MapSortCmpFunc);
        this->isSort = false;
    }
}

static cxBool MapFightTouch(cxAny pview,cxTouchItems *points)
{
    CX_ASSERT_THIS(pview, Map);
    if(points->number != 1){
        return false;
    }
    cxTouchItem item = points->items[0];
    cxVec2f cpos;
    if(!cxViewHitTest(pview, item->position, &cpos)){
        return false;
    }
    if(item->type == cxTouchTypeDown){
        this->isSelectUnit = true;
        return false;
    }
    if(item->type == cxTouchTypeUp && item->movement < 30){
        cxVec2f idx = MapPosToIdx(this, cpos);
        CX_LOGGER("fight mode selected:%f %f",idx.x,idx.y);
        //test
        if(this->tag == 1){
            Attack node = AttackCreate(this, cxSize2fv(1, 1), idx);
            NodeSetLife(node, 20000);
            NodeSetPower(node, 20);
            NodeAppend(node);
        }else if(this->tag == 2){
            Defence node = DefenceCreate(this, cxSize2fv(2, 2), idx);
            cxViewSetColor(node, cxRED);
            NodeSetLife(node, 200);
            NodeAppend(node);
        }else if(this->tag == 3){
            if(MapNode(this, idx.x, idx.y) != NULL){
                return false;
            }
            Wall node = WallCreate(this, cxSize2fv(1, 1), idx);
            cxViewSetColor(node, cxBLACK);
            NodeSetLife(node, 100);
            NodeAppend(node);
        }else if(this->tag == 4){
            
        }
        return true;
    }
    return false;
}

static void mapSubType(cxAny dst,cxAny src)
{
    CX_ASSERT_THIS(dst, Map);
    this->tag = cxViewTag(src);
}

static cpBB NodeIndexBB(cxAny pview)
{
    CX_ASSERT_THIS(pview, Node);
    cxFloat l = this->idx.x * global.sideLen;
    cxFloat b = this->idx.y * global.sideLen;
    cxFloat r = (this->idx.x + this->size.w) * global.sideLen;
    cxFloat t = (this->idx.y + this->size.h) * global.sideLen;
    return cpBBNew(l, b, r, t);
}

CX_SETTER_DEF(Map, mode)
{
    cxConstChars mode = cxJsonToConstChars(value);
    if(cxConstCharsHas(mode, "normal")){
        this->mode = MapModeNormal;
        CX_METHOD_SET(CX_TYPE(cxView, this)->Touch, MapEditTouch);
    }else if(cxConstCharsHas(mode, "fight")){
        this->mode = MapModeFight;
        CX_METHOD_SET(CX_TYPE(cxView, this)->Touch, MapFightTouch);
    }
}

static cxBool MapSearchIsAppend(cxAny pstar,cxVec2i *idx)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    Map map = CX_TYPE_CAST(Map, info->map);
    cxVec2f index = cxVec2fv(idx->x, idx->y);
    //如果索引超出最大范围
    if(!MapIsValidIdx(map, index)){
        return false;
    }
    Node sx = info->snode;
    Node dx = info->dnode;
    //超过搜索范围
    cxFloat dis = kmVec2DistanceBetween(&sx->idx, &index);
    if(dis > sx->searchMax){
        return false;
    }
    //背面的点不参与搜索
    dis = kmVec2DistanceBetween(&dx->idx, &index);
    if(dis > info->dis){
        return false;
    }
    //
    Node node = MapNode(map, idx->x, idx->y);
    if(node == NULL){
        return true;
    }
    if(node == sx || node == dx){
        return true;
    }
    //如果是阻挡类型
    if(node->type == NodeTypeBlock && !NodeIsDie(node)){
        return false;
    }
    return true;
}

static void MapSearchVisited(cxAny pstar,cxVec2i *idx)
{
    CX_ASSERT_THIS(pstar, cxAStar);
    MapSearchInfo *info = this->data;
    cxVec2f index = cxVec2fv(idx->x, idx->y);
    Node dx = info->dnode;
    cxFloat d = kmVec2DistanceBetween(&index, &dx->idx);
    if(d < info->vdis){
        info->vidx = index;
        info->vdis = d;
    }
}

CX_OBJECT_TYPE(Map, cxAtlas)
{
    CX_PROPERTY_SETTER(Map, mode);
}
CX_OBJECT_INIT(Map, cxAtlas)
{
    //当选中按钮执行
    cxMessageAppend(this, mapSubType, "selectSubType");
    
    CX_EVENT_APPEND(CX_TYPE(cxView, this)->onUpdate, MapUpdate);
    this->mode = MapModeNormal;
    this->isSort = true;
    
    //路劲搜索算法
    this->astar = CX_ALLOC(cxAStar);
    cxAStarSetType(this->astar, cxAStarTypeA8);
    CX_METHOD_SET(this->astar->IsAppend, MapSearchIsAppend);
    CX_METHOD_SET(this->astar->Visited, MapSearchVisited);
    
    //类型搜索索引
    this->defences      = cxSpatialAlloc(NodeIndexBB);
    this->attacks       = cxSpatialAlloc(NodeIndexBB);
    this->blocks        = cxSpatialAlloc(NodeIndexBB);
    this->resources     = cxSpatialAlloc(NodeIndexBB);
    this->decorations   = cxSpatialAlloc(NodeIndexBB);
    
    //弹药效果层
    this->bullet = CX_CREATE(cxView);
    cxViewAppend(this, this->bullet);
    
    //所有节点存储
    this->nodes = CX_ALLOC(cxHash);
    
    //路径索引
    this->paths = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(Map, cxAtlas)
{
    cxMessageRemove(this);
    CX_RELEASE(this->paths);
    CX_RELEASE(this->resources);
    CX_RELEASE(this->decorations);
    CX_RELEASE(this->bullet);
    CX_RELEASE(this->blocks);
    CX_RELEASE(this->astar);
    CX_RELEASE(this->attacks);
    CX_RELEASE(this->defences);
    CX_RELEASE(this->nodes);
}
CX_OBJECT_TERM(Map, cxAtlas)

static void MapAppendNode(cxAny pmap,cxSpatial sp,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    Node snode = CX_TYPE_CAST(Node, node);
    cxViewAppend(this, snode);
    cxSpatialInsert(sp, snode);
}

static void MapRemoveNode(cxSpatial sp,cxAny node)
{
    cxSpatialRemove(sp, node);
    cxViewRemove(node);
}

void MapAppendAttack(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->attacks, node);
}

void MapAppendDefence(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->defences, node);
}

void MapAppendBlock(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->blocks, node);
}

void MapRemoveAttack(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->attacks, node);
}

void MapRemoveDefence(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->defences, node);
}

void MapRemoveBlock(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->blocks, node);
}

//加入资源类型
void MapAppendResource(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->resources, node);
}

//移除资源类型
void MapRemoveResource(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->resources, node);
}

//搜索离curr最近的资源类单位
cxAny MapNearestResources(cxAny curr,cxRange2f range,NodeSubType subType)
{
    CX_ASSERT_THIS(curr, Node);
    Map map = NodeMap(this);
    return NodeNearest(map->resources, this->idx, range, NodeTypeResource, subType);
}

//搜索src dst之间的资源类单位
cxAny MapSegmentResources(cxAny src,cxAny dst,NodeSubType subType)
{
    Node sp = CX_TYPE_CAST(Node, src);
    Node dp = CX_TYPE_CAST(Node, dst);
    Map map = NodeMap(sp);
    return NodeSegment(map->resources, sp->idx, dp->idx, NodeTypeResource, subType);
}

//加入装饰类型
void MapAppendDecoration(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapAppendNode(map, map->decorations, node);
}

//移除装饰类型
void MapRemoveDecoration(cxAny node)
{
    CX_ASSERT_THIS(node, Node);
    Map map = CX_TYPE_CAST(Map, this->map);
    MapRemoveNode(map->decorations, node);
}

//搜索离curr最近的装饰类单位
cxAny MapNearestDecorations(cxAny curr,cxRange2f range,NodeSubType subType)
{
    CX_ASSERT_THIS(curr, Node);
    Map map = NodeMap(this);
    return NodeNearest(map->decorations, this->idx, range, NodeTypeDecoration, subType);
}

//搜索src dst之间的装饰类单位
cxAny MapSegmentDecorations(cxAny src,cxAny dst,NodeSubType subType)
{
    Node sp = CX_TYPE_CAST(Node, src);
    Node dp = CX_TYPE_CAST(Node, dst);
    Map map = NodeMap(sp);
    return NodeSegment(map->decorations, sp->idx, dp->idx, NodeTypeDecoration, subType);
}

//搜索离node最近的防御类单位
cxAny MapNearestDefences(cxAny curr,cxRange2f range,NodeSubType subType)
{
    CX_ASSERT_THIS(curr, Node);
    Map map = NodeMap(this);
    return NodeNearest(map->defences, this->idx, range, NodeTypeDefence, subType);
}

//搜索离node最近的攻击类单位
cxAny MapNearestAttacks(cxAny curr,cxRange2f range,NodeSubType subType)
{
    CX_ASSERT_THIS(curr, Node);
    Map map = NodeMap(this);
    return NodeNearest(map->attacks, this->idx, range, NodeTypeAttack, subType);
}

//搜索离node最近的阻挡类单位
cxAny MapNearestBlocks(cxAny curr,cxRange2f range,NodeSubType subType)
{
    CX_ASSERT_THIS(curr, Node);
    Map map = NodeMap(this);
    return NodeNearest(map->attacks, this->idx, range, NodeTypeBlock, subType);
}

//搜索src dst之间的防御类单位
cxAny MapSegmentDefences(cxAny src,cxAny dst,NodeSubType subType)
{
    Node sp = CX_TYPE_CAST(Node, src);
    Node dp = CX_TYPE_CAST(Node, dst);
    Map map = NodeMap(sp);
    return NodeSegment(map->defences, sp->idx, dp->idx, NodeTypeDefence, subType);
}

//搜索src dst之间的攻击类单位
cxAny MapSegmentAttacks(cxAny src,cxAny dst,NodeSubType subType)
{
    Node sp = CX_TYPE_CAST(Node, src);
    Node dp = CX_TYPE_CAST(Node, dst);
    Map map = NodeMap(sp);
    return NodeSegment(map->attacks, sp->idx, dp->idx, NodeTypeAttack, subType);
}

//搜索src dst之间的阻挡类单位
cxAny MapSegmentBlocks(cxAny src,cxAny dst,NodeSubType subType)
{
    Node sp = CX_TYPE_CAST(Node, src);
    Node dp = CX_TYPE_CAST(Node, dst);
    Map map = NodeMap(sp);
    return NodeSegment(map->blocks, sp->idx, dp->idx, NodeTypeBlock, subType);
}

cxAnyArray MapVisiedPoints(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    return this->astar->visits;
}

cxAnyArray MapSearchPoints(cxAny pmap)
{
    CX_ASSERT_THIS(pmap, Map);
    return this->astar->points;
}

#define CACHE_PATH_IDX(a,b) ((a.y * global.unitNum.x + a.x) << 16) + (b.y * global.unitNum.x + b.x)

void MapCacheDelPath(cxAny pmap,cxVec2i a,cxVec2i b)
{
    CX_ASSERT_THIS(pmap, Map);
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    cxHashDel(this->paths, key);
}

void MapCacheSetPath(cxAny pmap,cxVec2i a,cxVec2i b,cxAnyArray path)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT(cxAnyArrayLength(path) > 0, "save empty path points");
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    //复制一份保存
    cxHashSet(this->paths, key, cxAnyArrayClone(path));
}

cxBool MapCachePath(cxAny pmap,cxVec2i a,cxVec2i b)
{
    CX_ASSERT_THIS(pmap, Map);
    cxInt idx = CACHE_PATH_IDX(a,b);
    cxHashKey key = cxHashLongKey(idx);
    cxAnyArray path = cxHashGet(this->paths, key);
    if(path == NULL){
        return false;
    }
    cxAStarCleanPath(this->astar);
    cxAnyArrayAppends(this->astar->points, path);
    return true;
}

cxBool MapSearchPath(cxAny snode,cxAny dnode,cxVec2f *npos)
{
    Map map = NodeMap(snode);
    Node sx = CX_TYPE_CAST(Node, snode);
    Node dx = CX_TYPE_CAST(Node, dnode);
    //获取离snode最近的dnode本地点
    cxVec2f didx = NodeNearestPoint(dnode, sx->idx);
    //
    cxVec2i a = cxVec2iv(sx->idx.x, sx->idx.y);
    cxVec2i b = cxVec2iv(didx.x, didx.y);
    //获取缓存路径
    if(MapCachePath(map, a, b)){
        return true;
    }
    //
    MapSearchInfo info = {NULL};
    //设置搜索参数
    info.dis = kmVec2DistanceBetween(&sx->idx, &dx->idx);
    info.map = map;
    info.snode = snode;
    info.dnode = dnode;
    info.vdis = INT32_MAX;
    info.vidx = cxVec2fv(-1, -1);
    //开始搜索
    cxBool ret = cxAStarRun(map->astar, a, b, &info);
    if(!ret){
        //未成功时返回最近点
        if(npos != NULL)*npos = info.vidx;
    }else{
        //保存到缓存
        MapCacheSetPath(map, a, b, map->astar->points);
    }
    return ret;
}

void MapDelNode(cxAny pmap,cxInt x,cxInt y)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    cxHashDel(this->nodes, key);
}

void MapSetNode(cxAny pmap,cxInt x,cxInt y,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    cxHashSet(this->nodes, key, node);
}

cxAny MapNode(cxAny pmap,cxInt x,cxInt y)
{
    CX_ASSERT_THIS(pmap, Map);
    cxHashKey key = cxHashIntKey(y * global.unitNum.x + x);
    return cxHashGet(this->nodes, key);
}
                            
cxAny MapItem(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    if(!MapIsValidIdx(this, idx)){
        return NULL;
    }
    return MapNode(pmap, idx.x, idx.y);
}

void MapFillNode(cxAny pmap,cxVec2i idx,cxAny node)
{
    CX_ASSERT_THIS(pmap, Map);
    CX_ASSERT_TYPE(node, Node);
    Node snode = node;
    cxSize2i size = NodeSize(node);
    if(!MapIsValidIdx(this, snode->idx)){
        goto setnewpos;
    }
    //清除旧位置上的建筑
    cxVec2i curr = NodeIndex(node);
    for(cxInt x = curr.x; x < curr.x + size.w; x ++){
        for (cxInt y = curr.y; y < curr.y + size.h; y++) {
            MapDelNode(this, x, y);
        }
    }
setnewpos:
    //将建筑设置到新位置
    for(cxInt x = idx.x; x < idx.x + size.w; x ++){
        for (cxInt y = idx.y; y < idx.y + size.h; y++) {
            MapSetNode(this, x, y, node);
        }
    }
}

cxVec2f MapPosToIdx(cxAny pmap,cxVec2f pos)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    pos.y += (size.h - global.unitSize.h)/2.0f;
    return cxTilePosToIdx(pos, global.unitSize);
}

cxBool MapIsValidIdx(cxAny pmap,cxVec2f idx)
{
    return idx.x >= 0 && idx.x < global.unitNum.x && idx.y >= 0 && idx.y < global.unitNum.y;
}

cxVec2f MapIdxToPos(cxAny pmap,cxVec2f idx)
{
    CX_ASSERT_THIS(pmap, Map);
    cxSize2f size = cxViewSize(this);
    cxVec2f pos = cxTileIdxToPos(idx, global.unitSize);
    pos.y -= (size.h - global.unitSize.h)/2.0f;
    return pos;
}











