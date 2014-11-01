//
//  cxLoading.c
//  cxEngineIOS
//
//  Created by xuhua on 11/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include <actions/cxTimer.h>
#include "cxLoading.h"

CX_TYPE(cxLoadingItem, cxObject)
{
    //
}
CX_INIT(cxLoadingItem, cxObject)
{
    //
}
CX_FREE(cxLoadingItem, cxObject)
{
    
}
CX_TERM(cxLoadingItem, cxObject)

cxLoadingResult cxLoadingItemDrive(cxAny pview, cxAny pitem)
{
    CX_ASSERT_THIS(pitem, cxLoadingItem);
    cxLoadingItemSetView(this, pview);
    cxLoadingResult ret = this->Running(pview,this);
    //wait
    if(ret == cxLoadingResultWait){
        this->Count ++;
        this->Time += cxEngineGetFrameDelta();
    }
    return ret;
}

static cxBool cxLoadingTouch(cxAny pview,const cxTouchItems *points)
{
    return true;
}

static cxBool cxLoadingKey(cxAny pview,const cxKey *key)
{
    return true;
}

cxLoadingItem cxLoadingCurrentItem(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxLoadingItem item = NULL;
    if(this->Index >= 0 && this->Index < cxArrayLength(this->items)){
        item = cxArrayAtIndex(this->items, this->Index);
    }
    return item;
}

static void cxLoadingTimerArrive(cxAny sender)
{
    CX_ASSERT_VALUE(cxActionGetView(sender), cxLoading, this);
    //run step method
    cxLoadingItem item =  cxLoadingCurrentItem(this);
    CX_ASSERT(item != NULL && item->Running != NULL, "item error");
    cxLoadingResult ret = cxLoadingItemDrive(this, item);
    //wait
    if(ret == cxLoadingResultWait){
        return;
    }
    //success
    if(ret == cxLoadingResultSuccess){
        this->Index ++;
        CX_METHOD_RUN(this->onStep, this);
    }
    //break init
    if(ret == cxLoadingResultFailed){
        this->Success = false;
        cxActionStop(this->stepTimer);
    }
    if(this->Index == this->Step){
        cxActionStop(this->stepTimer);
    }
}

static void cxLoadingTimerExit(cxAny sender)
{
    CX_ASSERT_VALUE(cxActionGetView(sender), cxLoading, this);
    CX_METHOD_RUN(this->onExit, this);
    cxViewRemove(this);
}

void cxLoadingAppendItem(cxAny pview,cxAny pitem)
{
    CX_ASSERT_THIS(pview, cxLoading);
    CX_ASSERT_VALUE(pitem, cxLoadingItem, item);
    CX_ASSERT(item->Running != NULL, "item not set running method");
    cxArrayAppend(this->items, item);
    this->Step ++;
}

void cxLoadingAppend(cxAny pview,cxLoadingFunc running)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxLoadingItem item = CX_ALLOC(cxLoadingItem);
    item->Running = running;
    cxArrayAppend(this->items, item);
    this->Step ++;
    CX_RELEASE(item);
}

void cxLoaingFinished(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxActionStop(this->stepTimer);
}

CX_TYPE(cxLoading, cxView)
{
    
}
CX_INIT(cxLoading, cxView)
{
    this->Success = true;
    this->Step = 0;
    CX_SET(cxView, this, Touch, cxLoadingTouch);
    CX_SET(cxView, this, Key, cxLoadingKey);
    this->items = CX_ALLOC(cxArray);
}
CX_FREE(cxLoading, cxView)
{
    CX_RELEASE(this->items);
}
CX_TERM(cxLoading, cxView)

void cxLoadingStop(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    CX_RETURN(this->stepTimer == NULL);
    cxActionStop(this->stepTimer);
}

void cxLoadingStart(cxAny pview)
{
    CX_ASSERT_THIS(pview, cxLoading);
    cxEngine engine = cxEngineInstance();
    //run start and step 1
    CX_METHOD_RUN(this->onStart, this);
    CX_METHOD_RUN(this->onStep, this);
    //begin timer
    this->stepTimer = cxViewAppendTimer(this, 1.0f/30.0f, CX_FOREVER);
    CX_ADD(cxTimer, this->stepTimer, onArrive, cxLoadingTimerArrive);
    CX_ADD(cxAction, this->stepTimer, onExit, cxLoadingTimerExit);
    cxViewAppend(engine->Window, this);
}


