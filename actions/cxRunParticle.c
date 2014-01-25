//
//  cxRunParticle.c
//  cxEngine
//
//  Created by xuhua on 12/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEngine.h>
#include <views/cxParticle.h>
#include "cxRunParticle.h"

void __cxRunParticleTypeInit()
{
    
}

static void cxActionViewDraw(cxAny pav)
{
    cxRunParticle this = pav;
    cxAtlasDraw(this->particleView);
}

static void cxRunParticleInit(cxAny pav)
{
    cxRunParticle this = pav;
    cxView view = cxActionView(pav);
    cxParticle particle = this->particleView;
    CX_ASSERT(particle != NULL, "particle not set");
    cxParticleReset(particle);
    CX_SLOT_CONNECT(view->onDraw, this, onDraw, cxActionViewDraw);
}

static void cxRunParticleOver(cxAny pav)
{
    cxRunParticle this = pav;
    cxParticleStop(this->particleView);
    CX_SLOT_RELEASE(this->onDraw);
}

static void cxRunParticleStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxRunParticle this = pav;
    cxParticleUpdate(this->particleView);
}

static void cxRunParticleReadAttr(cxReaderAttrInfo *info)
{
    cxRunParticle this = info->object;
    cxActionReadAttr(info);
    cxConstChars pex = cxXMLAttr(info->reader, "cxRunParticle.pex");
    if(pex != NULL){
        cxParticle particle = cxParticleCreateFromPEX(pex);
        CX_RETAIN_SWAP(this->particleView, particle);
        this->super.duration = particle->duration;
    }
}

CX_OBJECT_INIT(cxRunParticle, cxAction)
{
    cxObjectSetReadAttrFunc(this, cxRunParticleReadAttr);
    CX_METHOD_OVERRIDE(this->super.Init, cxRunParticleInit);
    CX_METHOD_OVERRIDE(this->super.Over, cxRunParticleOver);
    CX_METHOD_OVERRIDE(this->super.Step, cxRunParticleStep);
}
CX_OBJECT_FREE(cxRunParticle, cxAction)
{
    CX_SLOT_RELEASE(this->onDraw);
    CX_RELEASE(this->particleView);
}
CX_OBJECT_TERM(cxRunParticle, cxAction)

cxRunParticle cxRunParticleCreateWithPEX(cxConstChars file)
{
    cxParticle v = cxParticleCreateFromPEX(file);
    return cxRunParticleCreate(v);
}

cxRunParticle cxRunParticleCreate(cxAny particleView)
{
    CX_ASSERT(particleView != NULL, "args error");
    cxParticle particle = particleView;
    cxRunParticle this = CX_CREATE(cxRunParticle);
    CX_RETAIN_SWAP(this->particleView, particleView);
    this->super.duration = particle->duration;
    return this;
}



