//
//  cxTextureTXT.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxIconv.h>
#include <engine/cxUtil.h>
#include "cxTextureTXT.h"


static cxBool cxTextureTXTLoad(cxAny texture,cxStream stream)
{
    cxTextureTXT this = texture;
    cxConstChars font = this->font == NULL ? NULL : cxStringBody(this->font);
    cxConstChars text = cxStringBody(this->string);
    cxString data = cxCreateTXTTextureData(text, font,this->attr.size, this->attr.align, this->attr.viewSize.w,this->attr.viewSize.h);
    CX_RETURN(data == NULL, false);
    cxInt bufsiz = cxStringLength(data);
    cxAny buffer = (cxAny)cxStringBody(data);
    cxSize2i *psize = (cxSize2i *)(buffer + bufsiz - sizeof(cxSize2i));
    cxAny tdata = buffer + sizeof(cxSize2i);
    cxTextureTXTMakeTexture(this, tdata, psize->w, psize->h);
    return true;
}

//bind alpha texture data
void cxTextureTXTMakeTexture(cxTextureTXT texture,cxAny buffer,cxInt width,cxInt height)
{
    GLint unpack = 0;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
    cxOpenGLGenTextures(1, &texture->cxTexture.textureId);
    cxOpenGLBindTexture(texture->cxTexture.textureId, 0);
    cxOpenGLSetTexParameters(texture->cxTexture.texParam);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
    cxOpenGLBindTexture(0, 0);
    texture->cxTexture.hasAlpha = true;
    texture->cxTexture.size = cxSize2fv(width, height);
}

static void cxTextureTXTBind(cxAny this)
{
    cxTextureTXT txt = this;
    cxOpenGLBindTexture(txt->cxTexture.textureId, 0);
}

CX_TYPE(cxTextureTXT, cxTexture)
{
    
}
CX_INIT(cxTextureTXT, cxTexture)
{
    CX_SET(cxTexture, this, Bind, cxTextureTXTBind);
    CX_SET(cxTexture, this, Load, cxTextureTXTLoad);
}
CX_FREE(cxTextureTXT, cxTexture)
{
    CX_RELEASE(this->string);
    CX_RELEASE(this->font);
}
CX_TERM(cxTextureTXT, cxTexture)


