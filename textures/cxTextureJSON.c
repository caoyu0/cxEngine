//
//  cxTextureXML.c
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <streams/cxAssetsStream.h>
#include "cxTextureJSON.h"
#include "cxTextureCache.h"

CX_METHOD_DEF(cxTextureJSON,Load,cxBool,cxStream stream)
{
    cxBool ret = false;
    CX_ASSERT(stream != NULL, "stream not set");
    cxString data = CX_CALL(stream, AllBytes, CX_M(cxString));
    if(data == NULL){
        CX_ERROR("read data failed from stream");
        return ret;
    }
    cxJson json = cxJsonCreate(data);
    if(json == NULL){
        CX_ERROR("create json error");
        return ret;
    }
    cxJson meta = cxJsonObject(json, "meta");
    if(meta == NULL){
        CX_ERROR("read json meta object error");
        return ret;
    }
    cxConstChars alphaPath = cxJsonConstChars(meta, "alpha");
    if(alphaPath != NULL){
        CX_RETAIN_SET(this->alphaTexture, cxTextureCacheLoadFile(alphaPath));
    }
    cxConstChars simagePath = cxJsonConstChars(meta, "image");
    CX_ASSERT(cxConstCharsOK(simagePath), "image miss");
    CX_RETAIN_SET(this->innerTexture, cxTextureCacheLoadFile(simagePath));
    CX_ASSERT(this->innerTexture != NULL, "get innerTexture error");
    ret = true;
    //for jpg pkm atlas texture
    this->cxTexture.size = this->innerTexture->size;
    //parse frames
    cxJson frames = cxJsonArray(json, "frames");
    CX_JSON_ARRAY_EACH_BEG(frames, item)
    cxTexCoord e = CX_ALLOC(cxTexCoord);
    cxConstChars sn = cxJsonConstChars(item, "filename");
    CX_ASSERT(sn != NULL, "filename node miss");
    e->rotated = cxJsonBool(item, "rotated", false);
    cxJson frame = cxJsonObject(item, "frame");
    CX_ASSERT(frame != NULL, "frame node miss");
    e->x = cxJsonDouble(frame, "x", 0);
    e->y = cxJsonDouble(frame, "y", 0);
    e->w = cxJsonDouble(frame, "w", 0);
    e->h = cxJsonDouble(frame, "h", 0);
    cxHashSet(this->cxTexture.keys, cxHashStrKey(sn), e);
    CX_RELEASE(e);
    CX_JSON_ARRAY_EACH_END(frames, item);
    cxConstChars shader = cxJsonConstChars(meta, "shader");
    CX_RETAIN_SET(this->cxTexture.shader, cxOpenGLShaderByName(shader));
    return ret;
}
CX_METHOD_DEF(cxTextureJSON,Bind,void)
{
    cxTextureJSON json = this;
    if(json->alphaTexture != NULL){
        cxOpenGLBindTexture(json->innerTexture->textureId, 1);
        cxOpenGLBindTexture(json->alphaTexture->textureId, 2);
    }else{
        cxOpenGLBindTexture(json->innerTexture->textureId, 0);
    }
}

CX_TYPE(cxTextureJSON, cxTexture)
{
    CX_METHOD(cxTextureJSON, Bind);
    CX_METHOD(cxTextureJSON, Load);
}
CX_INIT(cxTextureJSON, cxTexture)
{
    
}
CX_FREE(cxTextureJSON, cxTexture)
{
    CX_RELEASE(this->alphaTexture);
    CX_RELEASE(this->innerTexture);
}
CX_TERM(cxTextureJSON, cxTexture)

cxTexture cxTextureJSONLoadStream(cxStream stream)
{
    cxTexture json = CX_CREATE(cxTextureJSON);
    CX_CALL(json, Load, CX_M(cxBool,cxStream),stream);
    return json;
}



