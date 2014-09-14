//
//  Button.c
//  Home
//
//  Created by xuhua on 9/12/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxScale.h>
#include "Button.h"

static void ButtonEnter(cxAny pview)
{
    CX_ASSERT_THIS(pview, Button);
    cxScale scale = cxScaleCreate(0.1f, cxVec2fv(0.8f, 0.8f));
    cxViewAppendAction(this, scale);
}

static void ButtonLeave(cxAny pview)
{
    CX_ASSERT_THIS(pview, Button);
    cxScale scale = cxScaleCreate(0.1f, cxVec2fv(1.0f, 1.0f));
    cxViewAppendAction(this, scale);
}

static void ButtonRelease(cxAny pview)
{
    CX_ASSERT_THIS(pview, Button);
    cxViewSetScale(this, cxVec2fv(1.0f, 1.0f));
    CX_EVENT_FIRE(this, onTap);
}

CX_OBJECT_TYPE(Button, cxButton)
{
    
}
CX_OBJECT_INIT(Button, cxButton)
{
    CX_EVENT_APPEND(this->cxButton.onEnter, ButtonEnter);
    CX_EVENT_APPEND(this->cxButton.onLeave, ButtonLeave);
    CX_EVENT_APPEND(this->cxButton.onRelease, ButtonRelease);
}
CX_OBJECT_FREE(Button, cxButton)
{
    CX_EVENT_RELEASE(this->onTap);
}
CX_OBJECT_TERM(Button, cxButton)