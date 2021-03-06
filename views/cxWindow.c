//
//  cxWindow.c
//  cxEngine
//
//  Created by xuhua on 10/2/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#include "cxWindow.h"

CX_METHOD_DEF(cxWindow,OnKey,cxBool,const cxKey *key)
{
    if(key->code == CX_KEYCODE_BACK && key->type == cxKeyTypeUp){
        cxEngineExit();
    }
    return true;
}

CX_TYPE(cxWindow, cxView)
{
    CX_METHOD(cxWindow, OnKey);
}
CX_INIT(cxWindow, cxView)
{
    this->views = CX_ALLOC(cxStack);
}
CX_FREE(cxWindow, cxView)
{
    CX_RELEASE(this->views);
}
CX_TERM(cxWindow, cxView)

void cxWindowClear()
{
    cxEngine engine = cxEngineInstance();
    cxViewClear(engine->Window);
    cxStackClear(engine->Window->views);
}

cxAny cxWindowTopView()
{
    cxEngine engine = cxEngineInstance();
    return cxStackTop(engine->Window->views);
}

void cxWindowPushView(cxAny pview)
{
    CX_ASSERT_TYPE(pview, cxView);
    cxEngine engine = cxEngineInstance();
    cxView new = pview;
    cxView top = cxWindowTopView();
    if(top != NULL && cxViewGetSleepTop(new)){
        cxViewSetIsSleep(top, true);
        cxViewExit(top);
    }
    cxStackPush(engine->Window->views, new);
    cxViewAppend(engine->Window, new);
}

void cxWindowPopView()
{
    cxEngine engine = cxEngineInstance();
    cxBool hideTop = false;
    cxView prev = cxWindowTopView();
    cxStackPop(engine->Window->views);
    cxView top = cxWindowTopView();
    if(prev != NULL){
        hideTop = cxViewGetSleepTop(prev);
        cxViewRemove(prev);
    }
    if(top != NULL && hideTop){
        cxViewEnter(top);
        cxViewSetIsSleep(top, false);
    }
}

void cxWindowReplaceView(cxAny pview)
{
    CX_ASSERT_TYPE(pview, cxView);
    cxWindowPopView();
    cxWindowPushView(pview);
}












