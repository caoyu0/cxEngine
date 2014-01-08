//
//  cxBase.h
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxBase_h
#define cxEngine_cxBase_h

#include <utlist.h>
#include "cxConfig.h"
#include "cxAllocator.h"
#include "cxObject.h"

CX_C_BEGIN

typedef struct cxSignal cxSignal;

typedef struct cxSlot cxSlot;

struct cxSignal {
    cxSignal *prev,*next;
    cxSlot *slot;
    cxAny func;
    cxAny object;
};

struct cxSlot {
    cxSignal **signal;
    cxSignal *slot;
};

#define CX_SIGNAL_ALLOC(_name_) cxSignal *_name_

#define CX_SIGNAL_RELEASE(_signal_)                             \
do{                                                             \
    cxSignal *_tmp_ = NULL;                                     \
    cxSignal *_ele_ = NULL;                                     \
    DL_FOREACH_SAFE(_signal_, _ele_, _tmp_){                    \
        DL_DELETE(_signal_, _ele_);                             \
        _ele_->slot->slot = NULL;                               \
        _ele_->slot->signal = NULL;                             \
        allocator->free(_ele_);                                 \
    }                                                           \
}while(0)

#define CX_FUNC_TYPE(...) (void (*)(__VA_ARGS__))

#define CX_SLOT_OBJECT _ele_->object

#define CX_SIGNAL_FIRE(_signal_,_ft_,...)                       \
do{                                                             \
    cxSignal *_ele_ = NULL;                                     \
    DL_FOREACH(_signal_, _ele_){                                \
        (_ft_(_ele_->func))(__VA_ARGS__);                       \
    }                                                           \
}while(0)


#define CX_SLOT_ALLOC(name)      cxSlot name

#define CX_SLOT_RELEASE(_slot_)                                 \
if(_slot_.slot != NULL && _slot_.signal != NULL){               \
    DL_DELETE(*_slot_.signal,_slot_.slot);                      \
    allocator->free(_slot_.slot);                               \
    _slot_.signal = NULL;                                       \
    _slot_.slot = NULL;                                         \
}

#define CX_SLOT_CONNECT(_signal_,_object_,_slot_,_func_)        \
do{                                                             \
    CX_SLOT_RELEASE(_object_->_slot_);                          \
    cxSignal *_new_ = allocator->calloc(1,sizeof(cxSignal));    \
    _new_->func = _func_;                                       \
    _new_->object = _object_;                                   \
    _new_->slot = &_object_->_slot_;                            \
    _object_->_slot_.slot = _new_;                              \
    _object_->_slot_.signal = &_signal_;                        \
    DL_APPEND(_signal_,_new_);                                  \
}while(0)

typedef struct cxEvent cxEvent;

typedef void (*cxEventFunc)(cxEvent *event);

struct cxEvent {
    cxEvent *prev,*next;
    cxEventFunc func;
    cxAny sender;
    cxAny args;
};

#define CX_EVENT_ALLOC(name) cxEvent *name

#define CX_EVENT_APPEND(_event_,_func_,_args_)                  \
do{                                                             \
    cxEvent *_newptr_ = allocator->malloc(sizeof(cxEvent));     \
    _newptr_->func = _func_;                                    \
    _newptr_->args = NULL;                                      \
    CX_RETAIN_SWAP(_newptr_->args,_args_);                      \
    DL_APPEND(_event_, _newptr_);                               \
}while(0)

#define CX_EVENT_QUICK(_event_,_func_)                          \
CX_EVENT_APPEND(_event_,_func_,NULL)

#define CX_EVENT_PREPEND(_event_,_func_,_args_)                 \
do{                                                             \
    cxEvent *_newptr_ = allocator->malloc(sizeof(cxEvent));     \
    _newptr_->func = _func_;                                    \
    _newptr_->args = NULL;                                      \
    CX_RETAIN_SWAP(_newptr_->arg,_args_);                       \
    DL_PREPEND(_event_, _newptr_);                              \
}while(0)

#define CX_EVENT_DEL(_event_,_func_)                            \
do{                                                             \
    cxEvent *_tmp_ = NULL;                                      \
    cxEvent *_ele_ = NULL;                                      \
    DL_FOREACH_SAFE(_event_, _ele_, _tmp_){                     \
        if(_ele_->func != _func_){                              \
            continue;                                           \
        }                                                       \
        DL_DELETE(_event_, _ele_);                              \
        CX_RELEASE(_ele_->args);                                \
        allocator->free(_ele_);                                 \
    }                                                           \
}while(0)

#define CX_EVENT_RELEASE(_event_)                               \
do{                                                             \
    cxEvent *_tmp_ = NULL;                                      \
    cxEvent *_ele_ = NULL;                                      \
    DL_FOREACH_SAFE(_event_, _ele_, _tmp_){                     \
        DL_DELETE(_event_, _ele_);                              \
        CX_RELEASE(_ele_->args);                                \
        allocator->free(_ele_);                                 \
    }                                                           \
}while(0)

#define CX_EVENT_FIRE(_sender_,_event_)                         \
do{                                                             \
    cxEvent *_ele_ = NULL;                                      \
    cxEvent *_tmp_=NULL;                                        \
    DL_FOREACH_SAFE(_sender_->_event_, _ele_,_tmp_){            \
        _ele_->sender = _sender_;                               \
        _ele_->func(_ele_);                                     \
    }                                                           \
}while(0)

#define CX_LUA_EVENT_BEG(t)                                     \
CX_LUA_DEF_THIS(t);                                             \
cxConstChars eventName = luaL_checkstring(L, 2);                \
if(!lua_isfunction(L, 3)){                                      \
    luaL_error(L, "func error");                                \
    return 0;                                                   \
}                                                               \
do{

#define CX_LUA_EVENT_APPEND(en)                                 \
if(cxConstCharsEqu(eventName, #en)){                            \
    cxInt ref = lua_ref(L, true);                               \
    CX_ASSERT(ref > 0,"get ref error");                         \
    cxEventArg args = cxEventArgCreateWithRef(ref);             \
    CX_EVENT_APPEND(this->en, cxObjectLuaEventFunc, args);      \
    break;                                                      \
}

#define CX_LUA_EVENT_END(t)                                     \
}while(0);                                                      \
return 0

void cxObjectLuaEventFunc(cxEvent *event);

void cxUtilPrint(cxConstChars type,cxConstChars file,int line,cxConstChars format,va_list ap);

//调试信息输出
void cxUtilInfo(cxConstChars file, int line, cxConstChars format, ...);

//错误信息输出
void cxUtilError(cxConstChars file, int line, cxConstChars format, ...);

//警告信息输出
void cxUtilWarn(cxConstChars file, int line, cxConstChars format, ...);

//断言信息输出
void cxUtilAssert(cxConstChars file, int line, cxConstChars format, ...);

CX_C_END

#endif
