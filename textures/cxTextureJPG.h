//
//  cxTextureJPG.h
//  cxEngine
//
//  Created by xuhua on 11/10/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTextureJPG_h
#define cxEngine_cxTextureJPG_h

#include <engine/cxTexture.h>
#include <cxcore/cxStream.h>

CX_C_BEGIN

CX_DEF(cxTextureJPG, cxTexture)
    //
CX_END(cxTextureJPG, cxTexture)

cxTexture cxTextureJPGLoadStream(cxStream stream);

CX_C_END

#endif
