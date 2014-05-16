//
//  cxUtil.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <zlib.h>
#include <evhttp.h>
#include <sys/time.h>
#include <ctype.h>
#include <streams/cxAssetsStream.h>
#include <streams/cxFileStream.h>
#include <engine/cxEngine.h>
#include "cxBase.h"
#include "cxUtil.h"

cxBool cxCopyFile(cxConstChars file,cxCopyFileFunc func,cxAny udata)
{
    cxStream src = cxAssetsStreamCreate(file);
    cxStream dst = cxFileStreamCreate(file);
    if(!cxStreamOpen(src)){
        return false;
    }
    if(!cxStreamOpen(dst)){
        return false;
    }
    cxChar buffer[4096];
    cxProgress p = {0};
    p.current = 0;
    p.total = src->length;
    while (true) {
        cxInt rbytes = cxStreamRead(src, buffer, 4096);
        if(rbytes <= 0){
            break;
        }
        p.current += rbytes;
        cxInt wbytes = cxStreamWrite(dst, buffer, rbytes);
        CX_ASSERT(wbytes == rbytes, "rw error");
        if(func != NULL){
            func(file,&p,udata);
        }
        CX_UNUSED_PARAM(wbytes);
    }
    return true;
}

cxUInt cxBinaryToUInt(const cxChar *bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = strlen(bs);
    cxUInt ret = 0;
    for(cxInt i=0; i < len; i++){
        CX_ASSERT(bs[i] == '0' || bs[i] == '1', "str error");
        if(bs[i] != '1'){
            continue;
        }
        ret |= (1 << (len - i - 1));
    }
    return ret;
}

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c)
{
    for(cxInt i=0; i < strlen(sp);i++){
        if(sp[i] == c){
            return true;
        }
    }
    return  false;
}

cxUInt cxHexToUInt(const cxChar *bs)
{
    CX_ASSERT(bs != NULL, "args error");
    cxInt len = strlen(bs);
    cxUInt ret = 0;
    for(cxInt i=0; i < len; i++){
        cxChar v = tolower(bs[i]);
        cxUInt tmp = 0;
        if(v >= '0' && v <= '9'){
            tmp = v - '0';
        }else if(v >= 'a' && v <= 'f'){
            tmp = v - 'a' + 0xa;
        }else{
            CX_ASSERT(false, "str error");
        }
        ret |= (tmp << ((len - i - 1)) * 4);
    }
    return ret;
}

cxAudioFileType cxAudioGetType(cxConstChars file)
{
    cxConstChars ext = strrchr(file, '.');
    CX_RETURN(ext == NULL,cxAudioFileTypeNone);
    if(cxConstCharsEqu(ext, ".mp3")){
        return cxAudioFileTypeMP3;
    }
    if(cxConstCharsEqu(ext, ".wav")){
        return cxAudioFileTypeWAV;
    }
    return cxAudioFileTypeNone;
}

cxString cxCompressed(cxString data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStringLength(data);
    uLongf size = compressBound(datasiz);
    char *cv = allocator->malloc(size);
    if(compress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), datasiz) == Z_OK){
        return cxStringAttach(cv, size);
    }
    allocator->free(cv);
    return NULL;
}

cxString cxDecompress(cxString data)
{
    CX_ASSERT(data != NULL, "args error");
    cxInt datasiz = cxStringLength(data);
    uLongf size = datasiz * 4;
    char *cv = allocator->malloc(size);
    int ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), (uLongf)datasiz);
    while(ret == Z_BUF_ERROR){
        size = (size << 1);
        cv = allocator->realloc(cv,size);
        ret = uncompress((Bytef *)cv, &size, (Bytef *)cxStringBody(data), (uLongf)datasiz);
    }
    if(ret == Z_OK){
        return cxStringAttach(cv, size);
    }
    allocator->free(cv);
    return NULL;
}

cxInt cxRand(cxInt min,cxInt max)
{
    cxInt x = rand();
    return (min + x % (max + 1 - min));
}

void cxSetRandSeed()
{
    srand((unsigned)clock());
}

cxDouble cxTimestamp()
{
    struct timeval val = {0};
    gettimeofday(&val, NULL);
    return val.tv_sec + (cxDouble)val.tv_usec/(cxDouble)1000000.0;
}