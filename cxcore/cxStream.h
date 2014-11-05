//
//  cxStream.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxStream_h
#define cxEngine_cxStream_h

#include "cxCore.h"
#include "cxString.h"

CX_C_BEGIN

CX_DEF(cxStream, cxObject)
    cxString path;
    CX_FIELD_DEF(cxInt Length);
    cxBool canRead;
    cxBool canWrite;
    cxBool canSeek;
    cxBool isOpen;
    CX_METHOD_DEF(cxBool, Open, cxAny);
    CX_METHOD_DEF(cxInt, Read, cxAny, cxAny, cxInt);
    CX_METHOD_DEF(cxInt, Write, cxAny, cxAny, cxInt);
    CX_METHOD_DEF(cxInt, Seek, cxAny, cxOff, cxInt);
    CX_METHOD_DEF(cxOff, Position, cxAny);
    CX_METHOD_DEF(void, Close, cxAny);
    CX_METHOD_DEF(cxString, AllBytes, cxAny);
CX_END(cxStream, cxObject)

CX_FIELD_IMP(cxStream, cxInt, Length);

void cxStreamBaseClose(cxAny pstream);

cxBool cxStreamOpen(cxAny pstream);

cxInt cxStreamRead(cxAny pstream,cxAny buffer,cxInt size);

cxInt cxStreamWrite(cxAny pstream,cxAny buffer,cxInt size);

cxInt cxStreamSeek(cxAny pstream,cxOff off,cxInt flags);

cxOff cxStreamPosition(cxAny pstream);

void cxStreamClose(cxAny pstream);

cxString cxStreamAllBytes(cxAny pstream);

CX_C_END

#endif










