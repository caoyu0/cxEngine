//
//  cxValue.c
//  cxCore
//
//  Created by xuhua on 10/5/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxEngine.h"
#include "cxValue.h"

cxBool cxValueFloatEqu(cxAny pobj)
{
    CX_ASSERT_THIS(pobj, cxValue);
    cxFloat v1 = *(cxFloat *)this->oldVar;
    cxFloat v2 = *(cxFloat *)this->newVar;
    return cxFloatEqu(v1, v2);
}

cxBool cxValueVec2fEqu(cxAny pobj)
{
    CX_ASSERT_THIS(pobj, cxValue);
    cxVec2f v1 = *(cxVec2f *)this->oldVar;
    cxVec2f v2 = *(cxVec2f *)this->newVar;
    return cxVec2fEqu(v1, v2);
}

cxBool cxValueSize2fEqu(cxAny pobj)
{
    CX_ASSERT_THIS(pobj, cxValue);
    cxSize2f v1 = *(cxSize2f *)this->oldVar;
    cxSize2f v2 = *(cxSize2f *)this->newVar;
    return cxSize2fEqu(v1, v2);
}

cxBool cxValueBinaryEqu(cxAny pobj)
{
    CX_ASSERT_THIS(pobj, cxValue);
    return memcmp(this->newVar, this->oldVar, this->size) == 0;
}

static void cxValueUpdate(cxAny pobj,cxFloat dt)
{
    CX_ASSERT_THIS(pobj, cxValue);
    if(CX_METHOD_GET(false, this->ValueEqu, this)){
        return;
    }
    CX_EVENT_FIRE(this, onChanged);
    memcpy(this->oldVar, this->newVar, this->size);
}

CX_OBJECT_TYPE(cxValue, cxObject)
{
    
}
CX_OBJECT_INIT(cxValue, cxObject)
{
    cxEngine engine = cxEngineInstance();
    CX_LIN(cxEngine, engine, onUpdate, this, cxValueUpdate);
    CX_SET(cxValue, this, ValueEqu, cxValueBinaryEqu);
}
CX_OBJECT_FREE(cxValue, cxObject)
{
    CX_SLOT_RELEASE(this->onUpdate);
    allocator->free(this->oldVar);
    allocator->free(this->newVar);
    CX_EVENT_RELEASE(this->onChanged);
}
CX_OBJECT_TERM(cxValue, cxObject)

cxValue cxValueAllocImp(cxInt size)
{
    CX_ASSERT(size > 0, "size error");
    cxValue this = CX_ALLOC(cxValue);
    this->size = size;
    this->newVar = allocator->malloc(size);
    this->oldVar = allocator->malloc(size);
    return this;
}

cxValue cxValueCreateImp(cxInt size)
{
    return CX_AUTO(cxValueAllocImp(size));
}