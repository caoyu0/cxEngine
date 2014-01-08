//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <core/cxEngine.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineDataSet("items.xml");
    cxEngineLuaRunChars("require('main')");
    cxEngineLuaRunChars("init()");
}

void cxEngineMain(cxEngine engine)
{
    cxEngineLuaRunChars("main()");
}

void cxEngineFree(cxEngine engine)
{
    cxEngineLuaRunChars("free()");
}
















