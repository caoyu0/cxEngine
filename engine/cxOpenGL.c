//
//  cxOpenGL.c
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include <shaders/cxShaderColor.h>
#include <shaders/cxShaderDefault.h>
#include <shaders/cxShaderAlpha.h>
#include <shaders/cxShaderClipping.h>
#include <shaders/cxShaderMultiple.h>
#include <shaders/cxShaderTTF.h>
#include "cxUtil.h"
#include "cxOpenGL.h"

#define CX_OPENGL_LOAD_SHADER(t)                                    \
do{                                                                 \
    cxShader shader = CX_ALLOC(t);                                  \
    if(cxShaderFireInit(shader)){                                   \
        cxHashSet(this->shaders, cxHashStrKey(#t"Key"), shader);    \
    }else{                                                          \
        CX_ERROR("shader "#t" init failed");                        \
    }                                                               \
    CX_RELEASE(shader);                                             \
}while(0)

static void cxOpenGLLoadDefaultShaders()
{
    cxOpenGL this = cxOpenGLInstance();
    CX_OPENGL_LOAD_SHADER(cxShaderDefault);
    CX_OPENGL_LOAD_SHADER(cxShaderColor);
    CX_OPENGL_LOAD_SHADER(cxShaderAlpha);
    CX_OPENGL_LOAD_SHADER(cxShaderClipping);
    CX_OPENGL_LOAD_SHADER(cxShaderMultiple);
    CX_OPENGL_LOAD_SHADER(cxShaderTTF);
}

void cxOpenGLUsingShader(cxConstChars key)
{
    cxOpenGL this = cxOpenGLInstance();
    cxShader shader = cxHashGet(this->shaders, cxHashStrKey(key));
    CX_ASSERT(shader != NULL, "shader %s not exists",key);
    cxShaderUsing(shader);
}

void cxDrawLineBox(const cxBoxVec2f *box,const cxColor3f color)
{
    cxDrawLineLoop(&box->lt, 4, color);
}

void cxDrawLineLoop(const cxVec2f *vertices,int num,const cxColor3f color)
{
    cxColor3f colors[num];
    for(int i=0; i < num;i++){
        colors[i] = color;
    }
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(cxShaderColorKey);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosition|cxVertexAttribFlagColor);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 2, 0, vertices);
    cxOpenGLVertexAttribPointer(cxVertexAttribColor, 3, 0, colors);
    cxOpenGLDrawArrays(GL_LINE_LOOP, 0, num);
}

void cxDrawClippingRect(const cxVec2f pos,cxSize2f size)
{
    cxDrawSolidRect(cxRect4fv(pos.x, pos.y, size.w, size.h), cxColor4fv(0, 0, 0, 0), cxShaderDefaultKey);
}

void cxDrawPoint(const cxVec2f pos,cxColor3f color,cxFloat w)
{
    GLint lw;
    glGetIntegerv(GL_LINE_WIDTH, &lw);
    glLineWidth(w);
    cxRect4f rect = cxRect4fv(pos.x - w/2, pos.y - w/2, w, w);
    cxDrawSolidRect(rect, cxColor4fv(color.r, color.g, color.b, 1.0f), cxShaderColorKey);
    glLineWidth(lw);
}

void cxDrawSolidRect(const cxRect4f rect,const cxColor4f color,cxConstChars skey)
{
    cxBoxVec3f box;
    cxFloat wh = rect.w / 2.0f;
    cxFloat hh = rect.h / 2.0f;
    box.lb = cxVec3fv(rect.x - wh, rect.y - hh, 0);
    box.lt = cxVec3fv(rect.x - wh, rect.y + hh, 0);
    box.rb = cxVec3fv(rect.x + wh, rect.y - hh, 0);
    box.rt = cxVec3fv(rect.x + wh, rect.y + hh, 0);
    cxDrawSolidBox(&box, color, skey);
}

void cxDrawSolidBox(const cxBoxVec3f *box,const cxColor4f color,cxConstChars skey)
{
    cxColor4f colors[4] = {color,color,color,color};
    cxOpenGLSetBlendFactor(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cxOpenGLUsingShader(skey);
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosition | cxVertexAttribFlagColor);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 3, sizeof(cxVec3f), box);
    cxOpenGLVertexAttribPointer(cxVertexAttribColor, 4, sizeof(cxColor4f), colors);
    cxOpenGLDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void cxOpenGLVAODraw(GLuint vaoId,GLuint vboId[2],cxInt number,cxAny vbuffer,cxBool *dirty)
{
    if (*dirty){
        glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cxBoxPoint) * number, vbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        *dirty = false;
    }
    glBindVertexArray(vaoId);
    glDrawElements(GL_TRIANGLES, number*6, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}

void cxOpenGLVBODraw(GLuint vboId[2],cxInt number,cxAny vbuffer,cxBool *dirty)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    if (*dirty){
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cxBoxPoint) * number, vbuffer);
        *dirty = false;
    }
    cxOpenGLActiveAttribs(cxVertexAttribFlagPosColorTex);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 3, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, vertices));
    cxOpenGLVertexAttribPointer(cxVertexAttribTexcoord, 2, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, texcoords));
    cxOpenGLVertexAttribPointer(cxVertexAttribColor, 4, sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, colors));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
    glDrawElements(GL_TRIANGLES, (GLsizei)number*6, GL_UNSIGNED_SHORT, (GLvoid*)NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void cxOpenGLInitVAO(GLuint vaoId,GLuint vboId[2],cxInt number,cxAny vbuffer,cxAny ibuffer)
{
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cxBoxPoint) * number, vbuffer, GL_DYNAMIC_DRAW);
    //vertices
    cxOpenGLEnableVertexAttribArray(cxVertexAttribPosition,true);
    cxOpenGLVertexAttribPointer(cxVertexAttribPosition, 3,sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, vertices));
    //colors
    cxOpenGLEnableVertexAttribArray(cxVertexAttribColor,true);
    cxOpenGLVertexAttribPointer(cxVertexAttribColor, 4,sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, colors));
    //tex coords
    cxOpenGLEnableVertexAttribArray(cxVertexAttribTexcoord,true);
    cxOpenGLVertexAttribPointer(cxVertexAttribTexcoord, 2,sizeof(cxPoint), (GLvoid*)offsetof(cxPoint, texcoords));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cxIndices)*number, ibuffer, GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cxOpenGLInitVBO(GLuint vboId[2],cxInt number,cxAny vbuffer,cxAny ibuffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cxBoxPoint) * number, vbuffer, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cxIndices) * number, ibuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

CX_TYPE(cxOpenGL, cxObject)
{
    
}
CX_INIT(cxOpenGL, cxObject)
{
    this->clearColor = cxColor4fv(0, 0, 0, 1);
    this->blendSrc = -1;
    this->blendDst = -1;
    this->currentProgram = -1;
    this->shaders = CX_ALLOC(cxHash);
    for(cxInt i=0; i < MAX_ACTIVE_TEXTURE;i++){
        this->activeTextures[i] = -1;
    }
}
CX_FREE(cxOpenGL, cxObject)
{
    CX_RELEASE(this->shaders);
}
CX_TERM(cxOpenGL, cxObject)

cxAny cxOpenGLShader(cxConstChars key)
{
    CX_ASSERT(key != NULL, "shader key == NULL");
    cxOpenGL this = cxOpenGLInstance();
    return cxHashGet(this->shaders, cxHashStrKey(key));
}

cxAny cxOpenGLShaderByName(cxConstChars name)
{
    if(cxConstCharsEqu(name, "default")){
        return cxOpenGLShader(cxShaderDefaultKey);
    }
    if(cxConstCharsEqu(name, "color")){
        return cxOpenGLShader(cxShaderColorKey);
    }
    if(cxConstCharsEqu(name, "clipping")){
        return cxOpenGLShader(cxShaderClippingKey);
    }
    if(cxConstCharsEqu(name, "alpha")){
        return cxOpenGLShader(cxShaderAlphaKey);
    }
    if(cxConstCharsEqu(name, "multiple")){
        return cxOpenGLShader(cxShaderMultipleKey);
    }
    return cxOpenGLShader(cxShaderDefaultKey);
}

#define CX_GL_SUPPORT(t)                                \
this->support_##t = strstr(extensions,#t) > 0;          \
if(this->support_##t){CX_LOGGER("%s = YES",#t);}        \
else {CX_LOGGER("%s = NO",#t);}

void cxOpenGLCheckFeature()
{
    cxOpenGL this = cxOpenGLInstance();
    
    CX_LOGGER("GL_VENDOR:   %s", glGetString(GL_VENDOR));
    CX_LOGGER("GL_RENDERER: %s", glGetString(GL_RENDERER));
    CX_LOGGER("GL_VERSION:  %s", glGetString(GL_VERSION));
    
    cxConstChars extensions = (cxConstChars)glGetString(GL_EXTENSIONS);
    CX_ASSERT(extensions != NULL, "gl extensions get error");
    
    CX_GL_SUPPORT(GL_IMG_texture_npot);
    CX_GL_SUPPORT(GL_IMG_texture_compression_pvrtc);
    CX_GL_SUPPORT(GL_OES_compressed_ETC1_RGB8_texture);
    CX_GL_SUPPORT(GL_OES_vertex_array_object);
    CX_GL_SUPPORT(GL_EXT_discard_framebuffer);
    
#if (CX_TARGET_PLATFORM == CX_PLATFORM_ANDROID)
    this->support_GL_OES_vertex_array_object = false;
#endif
    
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->max_texture_size);
    CX_LOGGER("GL_MAX_TEXTURE_SIZE: %d",this->max_texture_size);
    
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &this->max_combined_texture_units);
    CX_LOGGER("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: %d",this->max_combined_texture_units);
    
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &this->max_vertex_texture_units);
    CX_LOGGER("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS: %d",this->max_vertex_texture_units);
    
    cxOpenGLLoadDefaultShaders();
}

void cxOpenGLSetBlendFactor(GLenum sfactor, GLenum dfactor)
{
    cxOpenGL this = cxOpenGLInstance();
    if(sfactor == this->blendSrc && dfactor == this->blendDst){
        return;
    }
    if(sfactor == GL_ONE && dfactor == GL_ZERO){
        glDisable(GL_BLEND);
    }else{
        glEnable(GL_BLEND);
        glBlendFunc(sfactor, dfactor);
        this->blendSrc = sfactor;
        this->blendDst = dfactor;
    }
}

void cxOpenGLSetDepthTest(cxBool on)
{
    if (on){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }else{
        glDisable(GL_DEPTH_TEST);
    }
}

void cxOpenGLViewport(const cxBox4f box)
{
    glViewport(box.l, box.t, box.r - box.l, box.b - box.t);
}

void cxOpenGLEnableScissor(const cxRect4f rect)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, rect.y, rect.w, rect.h);
}

void cxOpenGLSetTexParameter(GLuint type,GLuint value)
{
    glTexParameteri(GL_TEXTURE_2D, type, value);
}

void cxOpenGLSetTexParameters(const cxTextureParams params)
{
    cxOpenGLSetTexParameter(GL_TEXTURE_MIN_FILTER, params.minFilter);
    cxOpenGLSetTexParameter(GL_TEXTURE_MAG_FILTER, params.magFilter);
    cxOpenGLSetTexParameter(GL_TEXTURE_WRAP_S, params.wrapS);
    cxOpenGLSetTexParameter(GL_TEXTURE_WRAP_T, params.wrapT);
}

void cxOpenGLGenTextures(GLsizei n,GLuint *textures)
{
    glGenTextures(n, textures);
}

void cxOpenGLBindTexture(GLuint texture,cxInt idx)
{
    cxOpenGL this = cxOpenGLInstance();
    if (this->activeTextures[idx] != texture){
        this->activeTextures[idx] = texture;
        glActiveTexture(GL_TEXTURE0 + idx);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void cxOpenGLDeleteTexture(GLuint texture,cxInt idx)
{
    cxOpenGL this = cxOpenGLInstance();
    if(this->activeTextures[idx] == texture){
        this->activeTextures[idx] = -1;
    }
    glDeleteTextures(1, &texture);
}

void cxOpenGLDisableScissor()
{
    glDisable(GL_SCISSOR_TEST);
}

void cxOpenGLDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    glDrawArrays(mode, first, count);
}

void cxOpenGLVertexAttribPointer(GLuint indx, GLint size, GLsizei stride, const GLvoid* ptr)
{
    glVertexAttribPointer(indx, size, GL_FLOAT, GL_FALSE, stride, ptr);
}

void cxOpenGLEnableVertexAttribArray (GLuint index,GLboolean enable)
{
    if(enable){
        glEnableVertexAttribArray(index);
    }else{
        glDisableVertexAttribArray(index);
    }
}

void cxOpenGLActiveAttribs(cxUInt flags)
{
    cxOpenGL this = cxOpenGLInstance();
    cxBool eposition = (flags & cxVertexAttribFlagPosition) != 0;
    if(eposition != this->enableAttribPosition){
        cxOpenGLEnableVertexAttribArray(cxVertexAttribPosition,eposition);
        this->enableAttribPosition = eposition;
    }
    cxBool ecolor = (flags & cxVertexAttribFlagColor) != 0;
    if(ecolor != this->enableAttribColor){
        cxOpenGLEnableVertexAttribArray(cxVertexAttribColor,ecolor);
        this->enableAttribColor = ecolor;
    }
    cxBool etexcoords = (flags & cxVertexAttribFlagTexcoord);
    if(etexcoords != this->enableAttribTexcoords){
        cxOpenGLEnableVertexAttribArray(cxVertexAttribTexcoord,etexcoords);
        this->enableAttribTexcoords = etexcoords;
    }
}

void cxOpenGLUseProgram(GLuint program)
{
    cxOpenGL this = cxOpenGLInstance();
    if(this->currentProgram != program){
        glUseProgram(program);
        this->currentProgram = program;
    }
}

void cxOpenGLDeleteProgram(GLuint program)
{
    cxOpenGL this = cxOpenGLInstance();
    glDeleteProgram(program);
    this->currentProgram = -1;
}

void cxOpenGLSetClearColor(cxColor4f color)
{
    cxOpenGL this = cxOpenGLInstance();
    this->clearColor = color;
}

void cxOpenGLClear()
{
    cxOpenGL this = cxOpenGLInstance();
    glClearColor(this->clearColor.r, this->clearColor.g, this->clearColor.b, this->clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}





