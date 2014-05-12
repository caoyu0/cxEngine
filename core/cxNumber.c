//
//  cxNumber.c
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxNumber.h"

cxBool cxNumberIsType(cxAny this,cxNumberType type)
{
    cxNumber number = this;
    return number != NULL && number->type == type;
}

CX_OBJECT_INIT(cxNumber, cxObject)
{
    
}
CX_OBJECT_FREE(cxNumber, cxObject)
{
    
}
CX_OBJECT_TERM(cxNumber, cxObject)

cxBool cxNumberToBool(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeBool, "type error");
    return this->value.bv;
}

cxVec2i cxNumberToVec2i(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeVec2i, "type error");
    return this->value.vec2i;
}

cxVec2f cxNumberToVec2f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeVec2f, "type error");
    return this->value.vec2f;
}

cxTex2f cxNumberToTex2f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeTex2f, "type error");
    return this->value.tex2f;
}

cxSize2f cxNumberToSize2f(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeSize2f, "type error");
    return this->value.size2f;
}

cxSize2i cxNumberToSize2i(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeSize2i, "type error");
    return this->value.size2i;
}

cxInt cxNumberToInt(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt, "type error");
    return this->value.vi;
}

cxInt8 cxNumberToInt8(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt8, "type error");
    return this->value.i8;
}

cxInt16 cxNumberToInt16(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt16, "type error");
    return this->value.i16;
}

cxInt32 cxNumberToInt32(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt32, "type error");
    return this->value.i32;
}

cxInt64 cxNumberToInt64(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeInt64, "type error");
    return this->value.i64;
}

cxUInt cxNumberToUInt(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt, "type error");
    return this->value.vu;
}

cxUInt8 cxNumberToUInt8(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt8, "type error");
    return this->value.u8;
}

cxUInt16 cxNumberToUInt16(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt16, "type error");
    return this->value.u16;
}

cxUInt32 cxNumberToUInt32(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt32, "type error");
    return this->value.u32;
}

cxUInt64 cxNumberToUInt64(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeUInt64, "type error");
    return this->value.u64;
}

cxFloat cxNumberToFloat(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeFloat, "type error");
    return this->value.vf;
}

cxDouble cxNumberToDouble(cxNumber this)
{
    CX_ASSERT(this->type == cxNumberTypeDouble, "type error");
    return this->value.vd;
}

cxNumber cxNumberBool(cxBool v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeBool;
    rv->value.bv = v;
    return rv;
}

cxNumber cxNumberVec2f(cxVec2f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeVec2f;
    rv->value.vec2f = v;
    return rv;
}

cxNumber cxNumberTex2f(cxTex2f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeTex2f;
    rv->value.tex2f = v;
    return rv;
}

cxNumber cxNumberVec2i(cxVec2i v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeVec2i;
    rv->value.vec2i = v;
    return rv;
}

cxNumber cxNumberSize2f(cxSize2f v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeSize2f;
    rv->value.size2f = v;
    return rv;
}

cxNumber cxNumberSize2i(cxSize2i v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeSize2i;
    rv->value.size2i = v;
    return rv;
}

cxNumber cxNumberInt(cxInt v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt;
    rv->value.vi = v;
    return rv;
}

cxNumber cxNumberInt8(cxInt8 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt8;
    rv->value.i8 = v;
    return rv;
}

cxNumber cxNumberInt16(cxInt16 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt16;
    rv->value.i16 = v;
    return rv;
}

cxNumber cxNumberInt32(cxInt32 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt32;
    rv->value.i32 = v;
    return rv;
}

cxNumber cxNumberInt64(cxInt64 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeInt64;
    rv->value.i64 = v;
    return rv;
}

cxNumber cxNumberUInt(cxUInt v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt;
    rv->value.vu = v;
    return rv;
}

cxNumber cxNumberUInt8(cxUInt8 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt8;
    rv->value.u8 = v;
    return rv;
}

cxNumber cxNumberUInt16(cxUInt16 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt16;
    rv->value.u16 = v;
    return rv;
}

cxNumber cxNumberUInt32(cxUInt32 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt32;
    rv->value.u32 = v;
    return rv;
}

cxNumber cxNumberUInt64(cxUInt64 v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeUInt64;
    rv->value.u64 = v;
    return rv;
}

cxNumber cxNumberFloat(cxFloat v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeFloat;
    rv->value.vf = v;
    return rv;
}

cxNumber cxNumberDouble(cxDouble v)
{
    cxNumber rv = CX_CREATE(cxNumber);
    rv->type = cxNumberTypeDouble;
    rv->value.vd = v;
    return rv;
}

