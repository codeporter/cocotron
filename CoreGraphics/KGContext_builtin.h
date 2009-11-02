/*------------------------------------------------------------------------
 *
 * Derivative of the OpenVG 1.0.1 Reference Implementation
 * -------------------------------------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *-------------------------------------------------------------------*/

#import "KGBitmapContext.h"
#import "VGmath.h"
#import "KGSurface.h"

@class O2Paint;

typedef enum {
// These are winding masks, do not change value
  VG_EVEN_ODD=1,
  VG_NON_ZERO=-1
} VGFillRuleMask;

typedef struct Edge {
   O2Point v0;
   O2Point v1;
   int     direction;
// These are init/modified during AET processing, should be broken out to save memory
   struct Edge *next;
   int          isVertical;
   int          isFullCoverage;
   O2Float      vdxwl;
   O2Float      sxPre;
   O2Float      exPre;
   O2Float      bminx;
   O2Float      bmaxx;
   int          minx;
   O2Float     *samples;
   O2Float      minSample;
   O2Float      maxSample;
} Edge;

typedef void (*O2BlendSpan_RGBA8888)(O2argb8u *src,O2argb8u *dst,int length);
typedef void (*O2BlendSpan_RGBAffff)(O2argb32f *src,O2argb32f *dst,int length);
typedef void (*KGWriteCoverage_RGBA8888)(O2Surface *surface,O2Surface *mask,O2Paint *paint,int x, int y,int coverage,int length,O2BlendSpan_RGBA8888 blendFunction);

@class O2Surface,O2Context_builtin;

@interface O2Context_builtin : O2BitmapContext {

   O2Paint           *_paint;
   O2Context_builtin *_clipContext;
   
   O2BlendSpan_RGBA8888     _blend_lRGBA8888_PRE;
   O2BlendSpan_RGBAffff     _blend_lRGBAffff_PRE;
   KGWriteCoverage_RGBA8888 _writeCoverage_lRGBA8888_PRE;
   void               (*_blendFunction)();
   void               (*_writeCoverageFunction)();
   
    int _vpx,_vpy,_vpwidth,_vpheight;
    
    int    _edgeCount;
    int    _edgeCapacity;
    Edge **_edges;
    Edge **_sortCache;
    
    int *_winding;
    int *_increase;
    
    int     sampleSizeShift;
	int     numSamples;
    int     samplesWeight;
    O2Float *samplesX;
    O2Float  samplesInitialY;
    O2Float  samplesDeltaY;
    int alias;
}

-(void)setWidth:(size_t)width height:(size_t)height reallocateOnlyIfRequired:(BOOL)roir;

void KGRasterizerDealloc(O2Context_builtin *self);
void KGRasterizerSetViewport(O2Context_builtin *self,int x,int y,int vpwidth,int vpheight);
void KGRasterizerClear(O2Context_builtin *self);
void O2DContextAddEdge(O2Context_builtin *self,const O2Point v0, const O2Point v1);
void KGRasterizerSetShouldAntialias(O2Context_builtin *self,BOOL antialias,int quality);
void KGRasterizerFill(O2Context_builtin *self,int fillRule);

void KGRasterizeSetBlendMode(O2Context_builtin *self,O2BlendMode blendMode);
void KGRasterizeSetMask(O2Context_builtin *self,O2Surface* mask);
void O2DContextSetPaint(O2Context_builtin *self,O2Paint* paint);

void O2BlendSpanNormal_8888_coverage(O2argb8u *src,O2argb8u *dst,int coverage,int length);

@end
