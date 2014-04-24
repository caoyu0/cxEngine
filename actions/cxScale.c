//
//  cxScale.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxScale.h"

static void cxScaleInit(cxAny pav)
{
    cxScale this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->oldScale = this->super.view->scale;
    this->delta.x = this->newScale.x - this->oldScale.x;
    this->delta.y = this->newScale.y - this->oldScale.y;
}

static void cxScaleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxScale this = pav;
    cxVec2f nscale;
    nscale.x = this->oldScale.x + this->delta.x * time;
    nscale.y = this->oldScale.y + this->delta.y * time;
    cxViewSetScale(this->super.view, nscale);
}

CX_OBJECT_INIT(cxScale, cxAction)
{
    CX_METHOD_OVERRIDE(this->super.Init, cxScaleInit);
    CX_METHOD_OVERRIDE(this->super.Step, cxScaleStep);
}
CX_OBJECT_FREE(cxScale, cxAction)
{

}
CX_OBJECT_TERM(cxScale, cxAction)

void cxScaleSetScale(cxAny pav,cxVec2f scale)
{
    cxScale this = pav;
    this->newScale = scale;
}

cxScale cxScaleCreate(cxFloat duration,cxVec2f scale)
{
    cxScale this = CX_CREATE(cxScale);
    this->super.duration = duration;
    this->newScale = scale;
    return this;
}


