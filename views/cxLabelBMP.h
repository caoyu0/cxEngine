//
//  cxLabelBMP.h
//  cxEngine
//
//  Created by xuhua on 12/9/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxLabelBMP_h
#define cxEngine_cxLabelBMP_h

#include <engine/cxBMPFont.h>
#include "cxAtlas.h"

CX_C_BEGIN

CX_DEF(cxLabelBMP, cxAtlas)
    cxBMPFont font;
    cxString txt;
    cxBool isDirty;
    cxBool isCenter;
    cxFloat size;
CX_END(cxLabelBMP, cxAtlas)

cxLabelBMP cxLabelBMPCreate(cxString font,cxString txt);

void cxLabelBMPSetFontName(cxAny pview,cxString font);

void cxLabelBMPSetText(cxAny pview,cxString txt);

void cxLabelBMPSetFontSize(cxAny pview,cxFloat size);

CX_C_END

#endif
