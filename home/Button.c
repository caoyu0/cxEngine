//
//  Button.c
//  Home
//
//  Created by xuhua on 9/12/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <actions/cxScale.h>
#include "Button.h"

#define BUTTON_SCALE_ACTIION_ID 100000

static void ButtonEnter(cxAny pview)
{
    CX_ASSERT_THIS(pview, Button);
    cxScale scale = cxScaleCreate(0.1f, cxVec2fv(0.9f, 0.9f));
    cxActionSetActionId(scale, BUTTON_SCALE_ACTIION_ID);
    cxViewAppendAction(this, scale);
}

static void ButtonLeave(cxAny pview)
{
    CX_ASSERT_THIS(pview, Button);
    cxScale scale = cxScaleCreate(0.1f, cxVec2fv(1.0f, 1.0f));
    cxActionSetActionId(scale, BUTTON_SCALE_ACTIION_ID);
    cxViewAppendAction(this, scale);
}

static void ButtonRelease(cxAny pview)
{
    CX_ASSERT_THIS(pview, Button);
    cxScale scale = cxScaleCreate(0.1f, cxVec2fv(1.0f, 1.0f));
    cxActionSetActionId(scale, BUTTON_SCALE_ACTIION_ID);
    cxViewAppendAction(this, scale);
    CX_EVENT_FIRE(this, onTap);
}

CX_SETTER_DEF(Button, title)
{
    cxString title = cxJsonToString(value);
    cxLabelTTFSetText(this->ttf, title);
}

CX_TYPE(Button, cxButton)
{
    CX_PROPERTY_SETTER(Button, title);
}
CX_INIT(Button, cxButton)
{
    CX_ADD(cxButton, this, onEnter, ButtonEnter);
    CX_ADD(cxButton, this, onLeave, ButtonLeave);
    CX_ADD(cxButton, this, onRelease, ButtonRelease);
    this->ttf = cxLabelTTFCreate(UTF8("button"), NULL, 50);
    cxViewAppend(this, this->ttf);
}
CX_FREE(Button, cxButton)
{
    CX_EVENT_RELEASE(this->onTap);
}
CX_TERM(Button, cxButton)