//
//  cxEngine.h
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEngine_h
#define cxEngine_cxEngine_h

#include <views/cxWindow.h>
#include <cxcore/cxBase.h>
#include <textures/cxTextureCache.h>
#include "cxInput.h"
#include "cxUtil.h"
#include "cxBMPFont.h"
#include "cxGroup.h"
#include "cxIconv.h"
#include "cxPlayer.h"
#include <net/cxLooper.h>

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_STRING_KEY_DEF(cxEngineNoticPause);
CX_STRING_KEY_DEF(cxEngineNoticResume);
CX_STRING_KEY_DEF(cxEngineNoticMemory);
CX_STRING_KEY_DEF(cxEngineNoticRecvJson);

CX_DEF(cxEngine, cxObject)
    CX_FIELD_DEF(cxSize2f WinSize);     //screen size
    CX_FIELD_DEF(cxSize2f DesSize);     //design size
    CX_FIELD_DEF(cxVec2f Scale);        //dessize / winsize
    CX_FIELD_DEF(cxBool IsDebug);       //debug border
    CX_FIELD_DEF(cxFloat Delta);        //time per frame
    CX_FIELD_DEF(cxString Lang);        //zh en
    CX_FIELD_DEF(cxString Country);     //CN TW
    CX_FIELD_DEF(cxString Localized);   //current lang file path
    CX_FIELD_DEF(cxFloat Interval);     //
    CX_FIELD_DEF(cxWindow Window);      //main window
    CX_FIELD_DEF(cxJson Config);
    CX_FIELD_DEF(cxUInt Version);       //version number
    cxBool isInit;
    cxBool isTouch;
    cxBool isPause;
    cxHash files;
    cxHash assets;
    cxHash items;           //touch items
    cxTouchItems points;    //all touch points
    cxKey key;
    cxHash groups;          //action groups
    cxCurve curve;              //cxCurve instance
    cxOpenGL opengl;            //cxOpenGL instance
    cxIconv iconv;              //cxIconv instance
    cxPlayer player;            //cxPlayer instance
    cxTextureCache textures;    //cxTextureCache instance
    cxLooper looper;
CX_END(cxEngine, cxObject)

extern cxEngine engine;

CX_INLINE cxEngine cxEngineInstance()
{
    return engine;
}

CX_INLINE cxLooper cxEngineLooper()
{
    return engine->looper;
}

CX_INLINE cxJson cxEngineGetConfig()
{
    return engine->Config;
}

CX_INLINE cxUInt cxEngineVersion()
{
    return engine->Version;
}

CX_INLINE cxTextureCache cxTextureCacheInstance()
{
    return engine->textures;
}

CX_INLINE cxPlayer cxPlayerInstance()
{
    return engine->player;
}

CX_INLINE cxIconv cxIconvInstance()
{
    return engine->iconv;
}

CX_INLINE cxWindow cxEngineGetWindow()
{
    return engine->Window;
}

CX_INLINE cxOpenGL cxOpenGLInstance()
{
    return engine->opengl;
}

CX_INLINE cxCurve cxCurveInstance()
{
    return engine->curve;
}

CX_INLINE void cxEngineSetInterval(cxFloat interval)
{
    engine->Interval = interval;
}

CX_INLINE cxString cxEngineGetLocalized()
{
    return engine->Localized;
}

CX_INLINE cxString cxEngineGetCountry()
{
    return engine->Country;
}

CX_INLINE void cxEngineSetCountry(cxString country)
{
    CX_RETAIN_SWAP(engine->Country, country);
}

CX_INLINE cxString cxEngineGetLang()
{
    return engine->Lang;
}

CX_INLINE void cxEngineSetLang(cxString lng)
{
    CX_RETAIN_SWAP(engine->Lang, lng);
}

CX_INLINE cxFloat cxEngineGetDelta()
{
    return engine->Delta;
}

CX_INLINE void cxEngineSetScale(cxVec2f v)
{
    engine->Scale = v;
}

CX_INLINE cxVec2f cxEngineGetScale()
{
    return engine->Scale;
}

CX_INLINE cxBool cxEngineGetIsDebug()
{
    return engine->IsDebug;
}

CX_INLINE void cxEngineSetIsDebug(cxBool v)
{
#ifdef NDEBUG
    engine->IsDebug = false;
#else
    engine->IsDebug = v;
#endif
}

//use init method
CX_INLINE void cxEngineSetDesSize(cxSize2f size)
{
    engine->DesSize = size;
}
CX_INLINE cxSize2f cxEngineGetDesSize()
{
    return engine->DesSize;
}

CX_INLINE cxSize2f cxEngineGetWinSize()
{
    return engine->WinSize;
}

void cxEngineClear();

//type init this = cxEngine type
CX_EXTERN void cxEngineType(cxType this);

//engine init
CX_EXTERN void cxEngineInit(cxEngine engine);

//engine start draw
CX_EXTERN void cxEngineMain(cxEngine engine);

//engine free
CX_EXTERN void cxEngineFree(cxEngine engine);

json_t *cxEngineLocalizeder(cxConstChars key);

cxString cxLocalizedString(cxConstChars key);

cxJson cxEngineJsonReader(cxConstChars src);

cxAny cxEngineCreateObject(cxConstChars src);

cxString cxEngineAssetsData(cxConstChars file);

cxJson cxEngineLoadJson(cxConstChars file);

cxBMPFont cxEngineLoadBMPFont(cxConstChars file);

cxAny cxEngineTimer(cxFloat freq,cxInt repeat);

cxVec2f cxEngineTouchToWindow(cxVec2f pos);

cxVec2f cxEngineWindowToTouch(cxVec2f pos);

void cxEngineEnableTouch(cxBool enable);

cxBool cxEngineFireTouch(cxTouchType type,cxInt num,cxTouchPoint *points);

cxBool cxEngineFireKey(cxKeyType type,cxInt code);

void cxEngineSendJson(cxString json);

void cxEngineRecvJson(cxString json);

//初始化数据
void cxEngineStartup();

void cxEngineTerminate();

void cxEngineExit();

void cxEnginePause();

void cxEngineResume();

void cxEngineMemory();

void cxEngineDraw(cxFloat dt);

void cxEngineLayout(cxInt width,cxInt height);

void cxEngineDestroy();

CX_C_END

#endif
