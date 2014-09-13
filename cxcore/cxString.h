//
//  cxString.h
//  cxCore
//  可以存放二进制数据
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxCore_cxString_h
#define cxCore_cxString_h

#include <utstring.h>
#include "cxCore.h"
#include "cxArray.h"

CX_C_BEGIN

CX_OBJECT_DEF(cxString, cxObject)
    cxBool nocopy;
    UT_string strptr;
CX_OBJECT_END(cxString, cxObject)

#define cxStringNumber(v,t)         cxStringData(&(t){v},sizeof(t))

#define cxConstCharsEqu(s1,s2)      ((s1) != NULL && (s2) != NULL && strcmp(s1,s2) == 0)

#define cxConstCharsHas(str,sub)    (str != NULL && sub != NULL && strstr(str,sub))

#define cxConstCaseCharsEqu(s1,s2)  ((s1) != NULL && (s2) != NULL && strcasecmp(s1,s2) == 0)

#define cxConstCharsOK(s)           ((s) != NULL && strlen(s) > 0)

#define cxStringOK(s)               ((s) != NULL && cxStringLength(s) > 0)

#define CX_CONST_STRING(f,...)      cxStringBody(cxStringCreate(f,##__VA_ARGS__))

#define UTF8(f,...)  cxStringCreate(f,##__VA_ARGS__)

cxBool cxConstCharsIsNumber(cxConstChars s);

cxString cxStringAttachMem(cxChars d,cxInt l);

cxString cxStringAttachMap(cxAny d,cxInt l);

cxBool cxStringToBool(cxString str,cxBool dv);

cxInt cxStringToInt(cxString str,cxInt dv);

cxDouble cxStringToDouble(cxString str,cxDouble dv);

cxLong cxStringToLong(cxString str,cxLong dv);

void cxStringClean(cxString string);

cxString cxMD5(cxString v);

cxBool cxStringEqu(cxString s1,cxString s2);

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

cxInt cxStringLength(cxString string);

cxArray cxStringSplit(cxString string,cxConstChars sp);

cxConstChars cxStringBody(cxString string);

cxString cxStringAllocChars(cxConstChars str);

void cxStringReplace(cxString string,cxChar s1,cxChar s2);

cxString cxStringAttachChars(cxChars str);

cxString cxStringConstChars(cxConstChars str);

void cxStringConcat(cxString string,cxString str);

void cxStringFormat(cxString string,cxConstChars format,...);

cxString cxStringCreate(cxConstChars format,...);

void cxStringAppend(cxString string,cxConstChars d,cxInt l);

cxString cxStringBinary(cxAny d,cxInt l);

CX_C_END

#endif
