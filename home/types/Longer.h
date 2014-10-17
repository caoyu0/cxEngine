//
//  Longer.h
//  Home
//  远程地面攻击者，例如弓箭手
//  Created by xuhua on 9/27/14.
//  Copyright (c) 2014 xuhua. All rights reserved.
//

#ifndef Home_Longer_h
#define Home_Longer_h

#include "Attack.h"

CX_C_BEGIN

CX_DEF(Longer, Attack)
CX_END(Longer, Attack)

void LongerInit(cxAny pview, cxAny pmap,cxVec2i idx);

Longer LongerCreate(cxAny pmap,cxVec2i idx);

FindRuleResult LongerFindRule(cxAny pview,const NodeCombined *type);

ActionResult LongerAttackAction(cxAny pattacker,cxAny ptarget);

CX_C_END

#endif
