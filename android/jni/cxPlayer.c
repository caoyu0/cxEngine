//
//  cxPlayer.c
//  cxEngine
//
//  Created by xuhua on 10/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxPlayer.h>
#include <engine/cxEngine.h>
#include <cxcore/cxBase.h>
#include "cxAndroid.h"

CX_TYPE(cxBuffer, cxObject)
{
}
CX_INIT(cxBuffer, cxObject)
{
}
CX_FREE(cxBuffer, cxObject)
{
}
CX_TERM(cxBuffer, cxObject)

CX_TYPE(cxTrack, cxObject)
{
}
CX_INIT(cxTrack, cxObject)
{
    
}
CX_FREE(cxTrack, cxObject)
{
    CX_RELEASE(this->file);
}
CX_TERM(cxTrack, cxObject)

CX_TYPE(cxPlayer, cxObject)
{
}
CX_INIT(cxPlayer, cxObject)
{
    this->tracks = CX_ALLOC(cxHash);
}
CX_FREE(cxPlayer, cxObject)
{
    CX_RELEASE(this->tracks);
}
CX_TERM(cxPlayer, cxObject)

static cxPlayer instance = NULL;

JniMethodInfo pauseEffect   = {.isGet=false};
JniMethodInfo resumeEffect  = {.isGet=false};
JniMethodInfo stopEffect    = {.isGet=false};
JniMethodInfo playEffect    = {.isGet=false};
JniMethodInfo playMusic     = {.isGet=false};
JniMethodInfo stopMusic     = {.isGet=false};
JniMethodInfo pauseMusic    = {.isGet=false};
JniMethodInfo resumeMusic   = {.isGet=false};

cxAny cxBufferCreate(cxString data,cxInt format,cxInt freq)
{
    CX_ASSERT(false, "cxBufferCreate not use at android");
    return NULL;
}

void cxPauseEffect(cxAny track)
{
    cxTrack this = track;
    CX_GET_METHOD(pauseEffect,"cxEnginePauseEffect","(I)V");
    (*javaENV)->CallStaticVoidMethod(M(pauseEffect), this->soundId);
}

void cxResumeEffect(cxAny track)
{
    cxTrack this = track;
    CX_GET_METHOD(resumeEffect,"cxEngineResumeEffect","(I)V");
    (*javaENV)->CallStaticVoidMethod(M(resumeEffect), this->soundId);
}

void cxStopEffect(cxAny track)
{
    cxTrack this = track;
    CX_GET_METHOD(stopEffect,"cxEngineStopEffect","(I)V");
    (*javaENV)->CallStaticVoidMethod(M(stopEffect), this->soundId);
}

cxAny cxPlayBuffer(cxAny buffer,cxBool loop)
{
    CX_ASSERT(false, "cxPlayBuffer not use at android");
    return NULL;
}

cxAny cxPlayEffect(cxConstChars file,cxBool loop)
{
    cxPlayer this = cxPlayerInstance();
    CX_GET_METHOD(playEffect,"cxEnginePlayEffect","(Ljava/lang/String;Z)I");
    jstring path = (*javaENV)->NewStringUTF(javaENV,file);
    cxInt soundId = (*javaENV)->CallStaticIntMethod(M(playEffect), path, loop);
    (*javaENV)->DeleteLocalRef(javaENV,path);
    if(soundId <= 0){
        CX_ERROR("play file failed %s",file);
        return NULL;
    }
    cxTrack track = cxHashGet(this->tracks, cxHashStrKey(file));
    //add or replace
    if(track == NULL || track->soundId != soundId){
        track = CX_ALLOC(cxTrack);
        CX_RETAIN_SET(track->file, cxStringConstChars(file));
        track->soundId = soundId;
        cxHashSet(this->tracks, cxHashStrKey(file), track);
        CX_RELEASE(track);
    }
    return track;
}

void cxPlayMusic(cxConstChars file,cxBool loop){
    CX_GET_METHOD(playMusic,"cxEnginePlayMusic","(Ljava/lang/String;Z)V");
    jstring path = (*javaENV)->NewStringUTF(javaENV,file);
    (*javaENV)->CallStaticVoidMethod(M(playMusic), path, loop);
    (*javaENV)->DeleteLocalRef(javaENV,path);
}

void cxStopMusic()
{
    CX_GET_METHOD(stopMusic,"cxEngineStopMusic","()V");
    (*javaENV)->CallStaticVoidMethod(M(stopMusic));
}

void cxPauseMusic()
{
    CX_GET_METHOD(pauseMusic,"cxEnginePauseMusic","()V");
    (*javaENV)->CallStaticVoidMethod(M(pauseMusic));
}

void cxResumeMusic()
{
    CX_GET_METHOD(resumeMusic,"cxEngineResumeMusic","()V");
    (*javaENV)->CallStaticVoidMethod(M(resumeMusic));
}






