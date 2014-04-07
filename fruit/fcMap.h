//
//  fcMap.h
//  cxEngine
//
//  Created by xuhua on 3/20/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef fruit_fcMap_h
#define fruit_fcMap_h

#include <core/cxView.h>
#include "fcDefine.h"

CX_C_BEGIN

CX_OBJECT_DEF(fcMap, cxView)
    fcPath path;//begin point -> end point
    cxSize2f gridSize;
    cxAny sprites[DM_MAP_WIDTH][DM_MAP_HEIGHT];
    cxList intruder;    //闯关列表
    cxList defenser;    //防御者
    cxList props;       //道具列表
    cxList points;      //开始点和终点
    fcMapMode mode;
    //当点击到起点或者终点时
    CX_METHOD_ALLOC(void, TouchLocation,cxAny map,cxAny loc);
    //当有sprite到达end location
    CX_METHOD_ALLOC(void, Arrive,cxAny map,cxAny sprite);
CX_OBJECT_END(fcMap)

//设置地图模式
void fcMapSetMode(cxAny this,fcMapMode mode);
//起到释放能到达终点
cxBool fcMapHasPass(cxAny map);
//获取终点
cxVec2i fcMapEndIndex(cxAny map);
//获取起点
cxVec2i fcMapBegindex(cxAny map);
//获取实际的像素
cxFloat fcMapScaleValue(fcMap this,cxFloat v);

cxBool fcMapFindPath(fcMap this,fcPath *path,cxVec2i start,cxVec2i stop);

cxVec2i fcMapToIdx(fcMap this,cxVec2f pos);

cxVec2f fcMapToPos(fcMap this,cxVec2i idx);

cxInt fcMapValue(fcMap this,cxVec2i idx);

cxAny fcMapSprite(fcMap this,cxVec2i idx);

cxBool fcMapCheckIdx(cxVec2i idx);
//添加活动点
void fcMapAppendPoints(fcMap this,cxAny loc);

void fcMapRemovePoints(fcMap this,cxAny loc);

//添加闯关者
void fcMapAppendIntruder(fcMap this,cxAny sprite);

void fcMapRemoveIntruder(fcMap this,cxAny sprite);

//添加防御者
void fcMapAppendDefenser(fcMap this,cxAny sprite);

void fcMapRemoveDefenser(fcMap this,cxAny sprite);

//添加道具
void fcMapAppendProps(fcMap this,cxAny props);

void fcMapRemoveProps(fcMap this,cxAny props);

CX_C_END

#endif
