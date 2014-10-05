//
//  cxEAGLView.m
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <engine/cxEngine.h>
#import "cxAppDelegate.h"
#import "cxEAGLView.h"
#import <regex.h>

cxEAGLView *instance = nil;

@implementation cxEAGLView

+(Class)layerClass
{
    return [CAEAGLLayer class];
}

-(void)pauseMainLoop
{
    [displayLink setPaused:YES];
    cxEnginePause();
}

-(void)resumeMainLoop
{
    [displayLink setPaused:NO];
    cxEngineResume();
}

-(void)memoryWarning
{
    cxEngineMemory();
}

-(void)initMainLoop
{
    cxEngineBegin();
}

-(void)freeMainLoop
{
    cxEngineDestroy();
}

-(void)drawFrame
{
    cxEngineDraw();
    [eaglCTX presentRenderbuffer:GL_RENDERBUFFER];
}

-(void)layoutSubviews
{
    [self resizeFromLayer:(CAEAGLLayer*)self.layer];
    cxEngineLayout(width, height);
}

-(void)startMainLoop
{
    cxEngine engine = cxEngineInstance();
    [displayLink setFrameInterval:engine->interval * 60.0f];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}
-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if(self == nil){
        CX_ERROR("init gl frame error");
        return nil;
    }
    instance = self;
    CX_ASSERT(self != nil, "init view frame failed");
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.opaque = YES;
    NSMutableDictionary *properties = [[NSMutableDictionary alloc] init];
    [properties setObject:[NSNumber numberWithBool:NO] forKey:kEAGLDrawablePropertyRetainedBacking];
    [properties setObject:kEAGLColorFormatRGBA8 forKey:kEAGLDrawablePropertyColorFormat];
    eaglLayer.drawableProperties = properties;
    [properties release];
    eaglCTX = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    CX_ASSERT(eaglCTX != nil,"alloc EAGL Context error");
    if(![EAGLContext setCurrentContext:eaglCTX]){
        CX_ERROR("set current eagl context error");
        return nil;
    }
    glGenFramebuffers(1, &frameBuffer);
    CX_ASSERT(frameBuffer > 0,"gl frame buffer create failed");
    glGenRenderbuffers(1, &renderBuffer);
    CX_ASSERT(renderBuffer > 0,"gl render buffer create failed");
    glGenRenderbuffers(1, &depthBuffer);
    CX_ASSERT(depthBuffer > 0,"gl depth buffer create failed");
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);
    [self initMainLoop];
    displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawFrame)];
    [displayLink retain];
    return self;
}

+(cxEAGLView *)glView
{
    cxAppDelegate *app = (cxAppDelegate *)[[UIApplication sharedApplication] delegate];
    return [app.glViewController getGLView];
}

-(void)resizeFromLayer:(CAEAGLLayer *)layer
{
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    [eaglCTX renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
}

- (void) dealloc
{
    [self freeMainLoop];
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteRenderbuffers(1, &renderBuffer);
    glDeleteRenderbuffers(1, &depthBuffer);
    [EAGLContext setCurrentContext:nil];
    frameBuffer = 0;
    renderBuffer = 0;
    depthBuffer = 0;
    [eaglCTX release];
    [displayLink release];
    [super dealloc];
}

-(void)dispatcherMotionEvent:(NSSet *)touches type:(cxTouchType)type event:(UIEvent *)event
{
    cxTouchPoint points[CX_MAX_TOUCH_POINT];
    cxInt num = 0;
    for (UITouch *touch in touches) {
        CGPoint point = [touch locationInView:[touch view]];
        cxVec2f pos = cxVec2fv(point.x * self.contentScaleFactor, point.y * self.contentScaleFactor);
        points[num].xy = pos;
        points[num].id = (cxLong)touch;
        num ++;
    }
    cxEngineFireTouch(type, num, points);
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeDown event:event];
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeMove event:event];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeUp event:event];
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self dispatcherMotionEvent:touches type:cxTouchTypeCancel event:event];
}
@end
