//
//  cxIconv.c
//  cxEngine
//
//  Created by xuhua on 10/20/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxIconv.h"
#include "cxEngine.h"

CX_TYPE(cxIconvItem, cxObject)
{
    
}
CX_INIT(cxIconvItem, cxObject)
{
    
}
CX_FREE(cxIconvItem, cxObject)
{
    if(this->iconvptr != NULL){
        iconv_close(this->iconvptr);
    }
    CX_RELEASE(this->from);
    CX_RELEASE(this->to);
}
CX_TERM(cxIconvItem, cxObject)

cxIconvItem cxIconvItemCreate(cxConstChars from,cxConstChars to)
{
    cxIconvItem this = CX_CREATE(cxIconvItem);
    CX_RETAIN_SET(this->from, cxStringConstChars(from));
    CX_RETAIN_SET(this->to, cxStringConstChars(to));
    this->iconvptr = iconv_open(to,from);
    if(this->iconvptr == NULL){
        CX_ERROR("iconv open error form(%s) -> to(%s)",from, to);
        return NULL;
    }
    return this;
}

static cxString cxIconvItemConvert(cxIconvItem this,const cxString string)
{
    CX_RETURN(string == NULL, NULL);
    cxInt insiz = cxStringLength(string);
    cxInt allocsize = insiz * 4 + 1;
    cxInt outsiz = allocsize;
    cxInt outlen = 0;
    cxChars inptr = (cxChars)cxStringBody(string);
    cxChars outptr = allocator->malloc(outsiz);
    cxChars buffer = outptr;
    cxInt ret = (cxInt)iconv(this->iconvptr, &inptr, (size_t *)&insiz, &outptr, (size_t *)&outsiz);
    if(ret == 0){
        outlen = allocsize - outsiz;
        outptr[outlen] = '\0';
        return cxStringAttachMem(buffer, outlen);
    }
    CX_ERROR("convert failed");
    allocator->free(buffer);
    return NULL;
}

CX_TYPE(cxIconv, cxObject)
{
    
}
CX_INIT(cxIconv, cxObject)
{
    this->caches = CX_ALLOC(cxHash);
}
CX_FREE(cxIconv, cxObject)
{
    CX_RELEASE(this->caches);
}
CX_TERM(cxIconv, cxObject)

cxString cxIconvConvertUTF8ToUTF16LE(const cxString string)
{
    return cxIconvConvert(string, "UTF-8", "UTF-16LE");
}

cxString cxIconvConvert(const cxString string,cxConstChars from,cxConstChars to)
{
    cxIconv this = cxIconvInstance();
    cxConstChars key = cxConstString("%s->%s",from,to);
    cxIconvItem item = cxHashGet(this->caches, cxHashStrKey(key));
    if(item == NULL){
        item = cxIconvItemCreate(from,to);
        CX_ASSERT(item != NULL, "create iconv %s error",key);
        cxHashSet(this->caches, cxHashStrKey(key), item);
    }
    return cxIconvItemConvert(item,string);
}



