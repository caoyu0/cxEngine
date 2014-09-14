//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <engine/cxEngine.h>

void cxEngineType(cxEngine engine)
{
    
}

#include <views/cxSpine.h>

void cxEngineInit(cxEngine engine)
{
    cxEngineSetDesignSize(cxSize2fv(640, 960));
    
}

void cxEngineMain(cxEngine engine)
{
    cxLoader loader = cxLoaderCreate("cxSpine.json");
    cxWindowPushView(loader->object);
}

void cxEngineFree(cxEngine engine)
{
    
}
