//
//  cxLabelTTF.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLabelTTF_h
#define cxEngine_cxLabelTTF_h

#include "cxSprite.h"

CX_C_BEGIN

CX_DEF(cxLabelTTF, cxSprite)
    cxString text;
    cxString font;
    cxBool isDirty;
    cxTextAttr attr;
CX_END(cxLabelTTF, cxSprite)

void cxLabelTTFSetFont(cxAny pview,cxString font,cxColor4f color,cxFloat size);

void cxLabelTTFSetAlign(cxAny pview,cxTextAlign align);

void cxLabelTTFSetText(cxAny pview,cxString txt);

void cxLabelTTFSetShadow(cxAny pview,cxColor4f color,cxFloat radius,cxSize2f offset);

void cxLabelTTFSetStroke(cxAny pview,cxColor4f color,cxFloat width);

void cxLabelTTFUpdateText(cxAny pview);

cxLabelTTF cxLabelTTFCreate(cxString txt,cxString font,cxFloat fontsize);

CX_C_END

#endif
