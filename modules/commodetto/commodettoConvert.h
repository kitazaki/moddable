/*
 * Copyright (c) 2016-2017  Moddable Tech, Inc.
 *
 *   This file is part of the Moddable SDK Runtime.
 * 
 *   The Moddable SDK Runtime is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   The Moddable SDK Runtime is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with the Moddable SDK Runtime.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef __commodetto_convert_h__
#define __commodetto_convert_h__

#include <stdint.h>

#include "commodettoBitmap.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*CommodettoConverter)(uint32_t pixelCount, void *src, void *dst, void *clut);

uint8_t CommodettoPixelsConvert(uint32_t pixelCount,
								void *srcPixels, CommodettoBitmapFormat srcFormat,
								void *dstPixels, CommodettoBitmapFormat dstFormat);

CommodettoConverter CommodettoPixelsConverterGet(CommodettoBitmapFormat srcFormat, CommodettoBitmapFormat dstFormat);

#ifdef __cplusplus
}
#endif

#endif
