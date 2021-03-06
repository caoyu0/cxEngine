//
//  cxIconv.h
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxIconv_h
#define cxEngine_cxIconv_h

#include <iconv.h>
#include <cxcore/cxBase.h>

CX_C_BEGIN

CX_DEF(cxIconvItem, cxObject)
    iconv_t iconvptr;
    cxString from;
    cxString to;
CX_END(cxIconvItem, cxObject)

cxIconvItem cxIconvItemCreate(cxConstChars from,cxConstChars to);

CX_DEF(cxIconv, cxObject)
    cxHash caches;
CX_END(cxIconv, cxObject)

cxString cxIconvConvertUTF8ToUTF16LE(const cxString string);

cxString cxIconvConvert(const cxString string,cxConstChars from,cxConstChars to);

CX_C_END

#endif
