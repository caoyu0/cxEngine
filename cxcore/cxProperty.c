//
//  cxProperty.c
//  cxCoreIOS
//
//  Created by xuhua on 5/15/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "cxType.h"
#include "cxProperty.h"

CX_OBJECT_TYPE(cxProperty, cxObject)
{}
CX_OBJECT_INIT(cxProperty, cxObject)
{}
CX_OBJECT_FREE(cxProperty, cxObject)
{}
CX_OBJECT_TERM(cxProperty, cxObject)

void cxObjectSetter(cxAny object,cxConstChars key,cxAny value)
{
    CX_ASSERT(key != NULL && object != NULL && value != NULL, "args error");
    cxProperty p = cxObjectProperty(object, key);
    if(p != NULL && p->setter != NULL){
        p->setter(object,value);
    }
}

cxBool cxObjectGetter(cxAny object,cxConstChars key,cxAny *value)
{
    CX_ASSERT(key != NULL && object != NULL, "args error");
    cxProperty p = cxObjectProperty(object, key);
    if(p != NULL && p->setter != NULL){
        *value = p->getter(object);
        return true;
    }
    return false;
}
