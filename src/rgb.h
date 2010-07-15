#pragma once
#ifndef RGB_H
#define RGB_H

#include <stdlib.h> /* For abs() */
#include <math.h>
#include "keywords.h" /* For H_INLINE */

#if defined(_MSC_VER)
	#include "ms_stdint.h"
#else
	#include <stdint.h>
#endif

/* RGB colors in MMBitmaps are stored as BGR for convenience in converting
 * to/from certain formats (mainly OpenGL).
 *
 * It is best not to rely on the order (simply use rgb.{blue,green,red} to
 * access values), but some situations (e.g., glReadPixels) require one to
 * do so. In that case, check to make sure to use MMRGB_IS_BGR for future
 * compatibility. */

/* #define MMRGB_IS_BGR (offsetof(MMRGBColor, red) > offsetof(MMRGBColor, blue)) */
#define MMRGB_IS_BGR 1

struct _MMRGBColor {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

typedef struct _MMRGBColor MMRGBColor;

/* MMRGBHex is a hexadecimal color value, akin to HTML's, in the form 0xRRGGBB
 * where RR is the red value expressed as hexadecimal, GG is the green value,
 * and BB is the blue value. */
typedef uint32_t MMRGBHex;

#define MMRGBHEX_MIN 0x000000
#define MMRGBHEX_MAX 0xFFFFFF

/* Converts rgb color to hexadecimal value.
 * |red|, |green|, and |blue| should each be of the type |uint8_t|, where the
 * range is 0 - 255. */
#define RGB_TO_HEX(red, green, blue) (((red) << 16) | ((green) << 8) | (blue))

/* Convenience wrapper for MMRGBColors. */
MMRGBHex hexFromMMRGB(MMRGBColor rgb);

#define RED_FROM_HEX(hex) ((hex >> 16) & 0xFF)
#define GREEN_FROM_HEX(hex) ((hex >> 8) & 0xFF)
#define BLUE_FROM_HEX(hex) (hex & 0xFF)

/* Converts hexadecimal color to MMRGBColor. */
MMRGBColor MMRGBFromHex(MMRGBHex hex);

/* Check absolute equality of two RGB colors. */
#define MMRGBColorEqualToColor(c1, c2) ((c1).red == (c2).red && \
                                        (c1).blue == (c2).blue && \
                                        (c1).green == (c2).green)

/* Returns whether two colors are similar within the given range, |tolerance|.
 * Tolerance can be in the range 0.0f - 1.0f, where 0 denotes the exact
 * color and 1 denotes any color. */
int MMRGBColorSimilarToColor(MMRGBColor c1, MMRGBColor c2,
							 float tolerance);

/* Identical to MMRGBColorSimilarToColor, only for hex values. */
int MMRGBHexSimilarToColor(MMRGBHex h1, MMRGBHex h2, float tolerance);

#endif /* RGB_H */
