#include "types.h"
#include "rgb.h"

MMPoint MMPointMake(size_t x, size_t y)
{
	MMPoint point;
	point.x = x;
	point.y = y;
	return point;
}

MMSize MMSizeMake(size_t width, size_t height)
{
	MMSize size;
	size.width = width;
	size.height = height;
	return size;
}

MMRect MMRectMake(size_t x, size_t y, size_t width, size_t height)
{
	MMRect rect;
	rect.origin = MMPointMake(x, y);
	rect.size = MMSizeMake(width, height);
	return rect;
}

MMRGBHex hexFromMMRGB(MMRGBColor rgb)
{
	return RGB_TO_HEX(rgb.red, rgb.green, rgb.blue);
}

MMRGBColor MMRGBFromHex(MMRGBHex hex)
{
	MMRGBColor color;
	color.red = RED_FROM_HEX(hex);
	color.green = GREEN_FROM_HEX(hex);
	color.blue = BLUE_FROM_HEX(hex);
	return color;
}

int MMRGBColorSimilarToColor(MMRGBColor c1, MMRGBColor c2,
                                      float tolerance)
{
	/* Speedy case */
	if (tolerance <= 0.0f) {
		return MMRGBColorEqualToColor(c1, c2);
	} else { /* Otherwise, use a Euclidean space to determine similarity */
		uint8_t d1 = c1.red - c2.red;
		uint8_t d2 = c1.green - c2.green;
		uint8_t d3 = c1.blue - c2.blue;
		return sqrtf((d1 * d1) +
		             (d2 * d2) +
		             (d3 * d3)) <= (tolerance * 442.0f);
	}

}

int MMRGBHexSimilarToColor(MMRGBHex h1, MMRGBHex h2, float tolerance)
{
	if (tolerance <= 0.0f) {
		return h1 == h2;
	} else {
		uint8_t d1 = RED_FROM_HEX(h1) - RED_FROM_HEX(h2);
		uint8_t d2 = GREEN_FROM_HEX(h1) - GREEN_FROM_HEX(h2);
		uint8_t d3 = BLUE_FROM_HEX(h1) - BLUE_FROM_HEX(h2);
		return sqrtf((d1 * d1) +
		             (d2 * d2) +
		             (d3 * d3)) <= (tolerance * 442.0f);
	}
}
