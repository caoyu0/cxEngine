//
//  fcSpeedUp.c
//  fruit
//  加速道具
//  Created by xuhua on 3/30/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#include "fcSpeedUp.h"

CX_OBJECT_INIT(fcSpeedUp, fcSprite)
{
    this->super.type = fcSpriteTypeProperty;
}
CX_OBJECT_FREE(fcSpeedUp, fcSprite)
{

}
CX_OBJECT_TERM(fcSpeedUp, fcSprite)

void fcSpeedUpInit(cxAny this,cxAny map)
{
    fcSpriteInit(this, map);
}