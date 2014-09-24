//
//  Node.h
//  cxCore
//
//  Created by xuhua on 8/28/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Node_h
#define Home_Node_h

#include <engine/cxSpatial.h>
#include <views/cxSprite.h>
#include "Define.h"

CX_C_BEGIN

CX_OBJECT_DEF(Node, cxSprite)
    cxAny map;
    cxSize2f size;      //占用的格子数
    cxVec2f idx;        //左下角格子坐标
    cxAnyArray box;
    cxVec2f curr;       //当前所在位置
    cxBool canSelected; //可选择
    cxBool isSelected;  //是否悬选中
    cxBool isTouch;     //按下时是否选中
    cxBool isValidIdx;  //是否在有效的位置
    cxVec2f start;
    NodeSubType subType;//子类型
    NodeType type;      //主类型
    NodeState state;    //当前状态
    cxRange2f range;    //攻击范围
    cxFloat attackRate; //攻击频率
    cxFloat speed;      //移动速度
    cxFloat power;       //攻击力
    cxRange2i life;     //生命
    cxInt level;        //等级
    cxBool isArrive;    //到达攻击点
    cxTimer searchTimer;      //搜索用定时器
    CX_METHOD_DEF(void, Search,cxAny);
    cxTimer attackTimer;    //攻击用定时器
    CX_METHOD_DEF(void, Attack,cxAny);
    //生命值变化
    CX_METHOD_DEF(void, LifeChanged, cxAny);
    //移除或加入时
    CX_METHOD_DEF(void, Remove,cxAny);
    CX_METHOD_DEF(void, Append,cxAny);
CX_OBJECT_END(Node, cxSprite)

void NodeAppend(cxAny pview);

void NodeRemove(cxAny pview);

//设置生命 等级 攻击力
void NodeSetLife(cxAny pview,cxInt life);
void NodeAddLife(cxAny pview,cxInt life);
void NodeSetLevel(cxAny pview,cxInt level);
void NodeSetPower(cxAny pview,cxFloat power);
void NodeSetSpeed(cxAny pview,cxFloat speed);

//设置攻击范围
void NodeSetRange(cxAny pview,cxRange2f range);

//是否在node的索引范围内
cxBool NodeAtRange(cxAny pview,cxAny node);

//设置攻击频率
void NodeSetAttackRate(cxAny pview,cxFloat rate);

typedef struct {
    cxAny node;         //最近的view
    cxVec2f idx;        //与此点的
    cxFloat dis;        //距离
    cxRange2f range;    //范围内
    NodeType type;      //搜索的类型
    NodeSubType subType;//搜索子类型
} NodeNearestInfo;

typedef struct {
    cxAny node;         //最近的view
    cxVec2f a;        //第1点
    cxVec2f b;        //第2点
    cxFloat dis;        //距离
    cxFloat exit;       //退出值
    NodeType type;      //搜索的类型
    NodeSubType subType;//搜索子类型
} NodeSegmentInfo;

cxAny NodeMap(cxAny pview);

//节点死亡
cxBool NodeIsDie(cxAny pview);

//设置状态
void NodeSetState(cxAny pview,NodeState state);


//搜索离a点最近的线段上的单位
cxAny NodeSegment(cxAny ps,cxVec2f a,cxVec2f b,NodeType type,NodeSubType subType);

//搜索最近的单位，可设置多种类型组合 
cxAny NodeNearest(cxAny ps,cxVec2f idx,cxRange2f range,NodeType type,NodeSubType subType);

//暂停搜索
void NodePauseSearch(cxAny pview);

//重新启动搜索
void NodeResumeSearch(cxAny pview);

//启动搜索定时器
void NodeSearchRun(cxAny pview);

//暂停攻击
void NodePauseAttack(cxAny pview);

//重新启动攻击
void NodeResumeAttack(cxAny pview);

//启动攻击定时器
void NodeAttackRun(cxAny pview);

cxRange2f NodeRange(cxAny pview);

cxFloat NodePower(cxAny pview);

cxRange2i NodeLife(cxAny pview);

cxBool NodeHasPoint(cxAny pview,cxVec2i idx);

cxInt NodeLevel(cxAny pview);

cxVec2i NodeIndex(cxAny pview);

cxSize2i NodeSize(cxAny pview);

//获取Node当前位置索引
cxVec2f NodePosIndex(cxAny pview);

//从当前坐标转换为索引坐标
cxVec2f NodePosToIdx(cxAny pview,cxVec2f pos);

//update to curr
void NodeUpdateIdx(cxAny pview);

cxVec2f NodeCurrIdx(cxAny pview);
//设置并放置
void NodeSetIdx(cxAny pview,cxVec2f idx);

//重置到旧位置
void NodeResetPosition(cxAny pview);

//检测新索引是否可以放置
cxBool NodeIdxIsValid(cxAny pview,cxVec2f curr);

//移动成功返回true
cxBool NodeSetPosition(cxAny pview,cxVec2f idx,cxBool animate);

void NodeSetSize(cxAny pview,cxSize2f size);

void NodeInit(cxAny pview,cxAny map,cxSize2f size,cxVec2f idx);

CX_C_END

#endif
