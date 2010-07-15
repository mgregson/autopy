#pragma once
#ifndef TYPES_H
#define TYPES_H

#include "os.h"
#include "keywords.h" /* For H_INLINE definition */
#include <stddef.h>

/* Some generic, cross-platform types. */

struct _MMPoint {
	size_t x;
	size_t y;
};

typedef struct _MMPoint MMPoint;

struct _MMSize {
	size_t width;
	size_t height;
};

typedef struct _MMSize MMSize;

struct _MMRect {
	MMPoint origin;
	MMSize size;
};

typedef struct _MMRect MMRect;

MMPoint MMPointMake(size_t x, size_t y);

MMSize MMSizeMake(size_t width, size_t height);

MMRect MMRectMake(size_t x, size_t y, size_t width, size_t height);

#define MMPointZero MMPointMake(0, 0)

#if defined(IS_MACOSX)

#define CGPointFromMMPoint(p) CGPointMake((CGFloat)(p).x, (CGFloat)(p).y)
#define MMPointFromCGPoint(p) MMPointMake((size_t)(p).x, (size_t)(p).y)

#elif defined(IS_WINDOWS)

#define MMPointFromPOINT(p) MMPointMake((size_t)p.x, (size_t)p.y)

#endif

#endif /* TYPES_H */
