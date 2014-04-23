//
//  main.c
//  cxFramework
//
//  Created by xuhua on 4/23/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include <stdio.h>
#include "core/cxFramework.h"

CX_OBJECT_DEF(Test, cxObject)

CX_OBJECT_END(Test)

CX_OBJECT_INIT(Test, cxObject)
{
    
}
CX_OBJECT_FREE(Test, cxObject)
{
    
}
CX_OBJECT_TERM(Test, cxObject)

int main(int argc, const char * argv[])
{
    //
    cxFrameworkInit();
    //
    Test t = CX_ALLOC(Test);
    CX_RELEASE(t);
    //
    cxFrameworkFree();
    //
    return 0;
}
