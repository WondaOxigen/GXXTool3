// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

enum GEBufferFormat
{
	GE_FORMAT_565 = 0,
	GE_FORMAT_5551 = 1,
	GE_FORMAT_4444 = 2,
	GE_FORMAT_8888 = 3,
	GE_FORMAT_DEPTH16 = 4,  // Virtual format, just used to pass into Depal
	GE_FORMAT_INVALID = 0xFF,
};

const char* GeBufferFormatToString(GEBufferFormat fmt);
inline bool IsGeBufferFormat16BitColor(GEBufferFormat fmt) {
	return (int)fmt < 3;
}

#define GE_VTYPE_TRANSFORM (0<<23)
#define GE_VTYPE_THROUGH   (1<<23)
#define GE_VTYPE_THROUGH_MASK (1<<23)

#define GE_VTYPE_TC_NONE  (0<<0)
#define GE_VTYPE_TC_8BIT  (1<<0)
#define GE_VTYPE_TC_16BIT (2<<0)
#define GE_VTYPE_TC_FLOAT (3<<0)
#define GE_VTYPE_TC_MASK  (3<<0)
#define GE_VTYPE_TC_SHIFT 0

#define GE_VTYPE_COL_NONE (0<<2)
#define GE_VTYPE_COL_565  (4<<2)
#define GE_VTYPE_COL_5551 (5<<2)
#define GE_VTYPE_COL_4444 (6<<2)
#define GE_VTYPE_COL_8888 (7<<2)
#define GE_VTYPE_COL_MASK (7<<2)
#define GE_VTYPE_COL_SHIFT 2

#define GE_VTYPE_NRM_NONE  (0<<5)
#define GE_VTYPE_NRM_8BIT  (1<<5)
#define GE_VTYPE_NRM_16BIT (2<<5)
#define GE_VTYPE_NRM_FLOAT (3<<5)
#define GE_VTYPE_NRM_MASK  (3<<5)
#define GE_VTYPE_NRM_SHIFT 5

//#define GE_VTYPE_POSITION_NONE  (0<<5)
#define GE_VTYPE_POS_8BIT  (1<<7)
#define GE_VTYPE_POS_16BIT (2<<7)
#define GE_VTYPE_POS_FLOAT (3<<7)
#define GE_VTYPE_POS_MASK  (3<<7)
#define GE_VTYPE_POS_SHIFT 7

#define GE_VTYPE_WEIGHT_NONE  (0<<9)
#define GE_VTYPE_WEIGHT_8BIT  (1<<9)
#define GE_VTYPE_WEIGHT_16BIT (2<<9)
#define GE_VTYPE_WEIGHT_FLOAT (3<<9)
#define GE_VTYPE_WEIGHT_MASK  (3<<9)
#define GE_VTYPE_WEIGHT_SHIFT 9

#define GE_VTYPE_WEIGHTCOUNT_MASK  (7<<14)
#define GE_VTYPE_WEIGHTCOUNT_SHIFT 14

#define GE_VTYPE_MORPHCOUNT_MASK  (7<<18)
#define GE_VTYPE_MORPHCOUNT_SHIFT 18

#define GE_VTYPE_IDX_NONE  (0<<11)
#define GE_VTYPE_IDX_8BIT  (1<<11)
#define GE_VTYPE_IDX_16BIT (2<<11)
#define GE_VTYPE_IDX_32BIT (3<<11)
#define GE_VTYPE_IDX_MASK  (3<<11)
#define GE_VTYPE_IDX_SHIFT 11

#define GE_CLEARMODE_COLOR (1<<8)
#define GE_CLEARMODE_ALPHA (1<<9) //or stencil?
#define GE_CLEARMODE_Z     (1<<10)
#define GE_CLEARMODE_ALL (GE_CLEARMODE_COLOR|GE_CLEARMODE_ALPHA|GE_CLEARMODE_Z)

enum GEMatrixType {
	GE_MTX_BONE0 = 0,
	GE_MTX_BONE1,
	GE_MTX_BONE2,
	GE_MTX_BONE3,
	GE_MTX_BONE4,
	GE_MTX_BONE5,
	GE_MTX_BONE6,
	GE_MTX_BONE7,
	GE_MTX_WORLD,
	GE_MTX_VIEW,
	GE_MTX_PROJECTION,
	GE_MTX_TEXGEN,
};

enum GEComparison
{
	GE_COMP_NEVER = 0,
	GE_COMP_ALWAYS = 1,
	GE_COMP_EQUAL = 2,
	GE_COMP_NOTEQUAL = 3,
	GE_COMP_LESS = 4,
	GE_COMP_LEQUAL = 5,
	GE_COMP_GREATER = 6,
	GE_COMP_GEQUAL = 7,
};

enum GEShadeMode
{
	GE_SHADE_FLAT = 0,
	GE_SHADE_GOURAUD = 1,
};

enum GELightType
{
	GE_LIGHTTYPE_DIRECTIONAL = 0,
	GE_LIGHTTYPE_POINT = 1,
	GE_LIGHTTYPE_SPOT = 2,
	GE_LIGHTTYPE_UNKNOWN = 3,
};

enum GELightColorSetting
{
	GE_LIGHTCOLOR_AMBIENT = 0,
	GE_LIGHTCOLOR_DIFFUSE = 1,
	GE_LIGHTCOLOR_SPECULAR = 2,
};

enum GELightComputation
{
	GE_LIGHTCOMP_ONLYDIFFUSE = 0,
	GE_LIGHTCOMP_BOTH = 1,
	GE_LIGHTCOMP_ONLYPOWDIFFUSE = 2,
};

enum GETextureFormat
{
	GE_TFMT_5650 = 0,
	GE_TFMT_5551 = 1,
	GE_TFMT_4444 = 2,
	GE_TFMT_8888 = 3,
	GE_TFMT_CLUT4 = 4,
	GE_TFMT_CLUT8 = 5,
	GE_TFMT_CLUT16 = 6,
	GE_TFMT_CLUT32 = 7,
	GE_TFMT_DXT1 = 8,
	GE_TFMT_DXT3 = 9,
	GE_TFMT_DXT5 = 10,
};

inline bool IsClutFormat(GETextureFormat tfmt) {
	return tfmt == GE_TFMT_CLUT4 || tfmt == GE_TFMT_CLUT8 || tfmt == GE_TFMT_CLUT16 || tfmt == GE_TFMT_CLUT32;
}
inline bool IsDXTFormat(GETextureFormat tfmt) {
	return tfmt == GE_TFMT_DXT1 || tfmt == GE_TFMT_DXT3 || tfmt == GE_TFMT_DXT5;
}
inline bool IsTextureFormatBufferCompatible(GETextureFormat tfmt) {
	return (int)tfmt < 4;
}
inline bool IsBufferFormat16Bit(GEBufferFormat bfmt) {
	return (int)bfmt < 3;
}
inline bool IsTextureFormat16Bit(GETextureFormat tfmt) {
	return (int)tfmt < 3;
}
inline bool TextureFormatMatchesBufferFormat(GETextureFormat fmt, GEBufferFormat bfmt) {
	// First four matches perfectly.
	if ((int)fmt < 4) {
		return (int)fmt == (int)bfmt;
	}
	else {
		return false;
	}
}
// only applicable if IsTextureFormatBufferCompatible(fmt)
inline GEBufferFormat TextureFormatToBufferFormat(GETextureFormat bfmt) {
	return (GEBufferFormat)(int)bfmt;
}

enum GETexLevelMode {
	GE_TEXLEVEL_MODE_AUTO = 0,
	GE_TEXLEVEL_MODE_CONST = 1,
	GE_TEXLEVEL_MODE_SLOPE = 2,
	GE_TEXLEVEL_MODE_UNKNOWN = 3,  // Behaves like CONST.
};

enum GEMaterialColorSetting
{
	GE_MATERIALCOLOR_EMISSIVE = 0,
	GE_MATERIALCOLOR_AMBIENT = 1,
	GE_MATERIALCOLOR_DIFFUSE = 2,
	GE_MATERIALCOLOR_SPECULAR = 3,
};

enum GEBlendMode
{
	GE_BLENDMODE_MUL_AND_ADD = 0,
	GE_BLENDMODE_MUL_AND_SUBTRACT = 1,
	GE_BLENDMODE_MUL_AND_SUBTRACT_REVERSE = 2,
	GE_BLENDMODE_MIN = 3,
	GE_BLENDMODE_MAX = 4,
	GE_BLENDMODE_ABSDIFF = 5,
};

enum GEBlendSrcFactor
{
	GE_SRCBLEND_DSTCOLOR,
	GE_SRCBLEND_INVDSTCOLOR,
	GE_SRCBLEND_SRCALPHA,
	GE_SRCBLEND_INVSRCALPHA,
	GE_SRCBLEND_DSTALPHA,
	GE_SRCBLEND_INVDSTALPHA,
	GE_SRCBLEND_DOUBLESRCALPHA,
	GE_SRCBLEND_DOUBLEINVSRCALPHA,
	GE_SRCBLEND_DOUBLEDSTALPHA,
	GE_SRCBLEND_DOUBLEINVDSTALPHA,
	GE_SRCBLEND_FIXA,
};

enum GEBlendDstFactor
{
	GE_DSTBLEND_SRCCOLOR,
	GE_DSTBLEND_INVSRCCOLOR,
	GE_DSTBLEND_SRCALPHA,
	GE_DSTBLEND_INVSRCALPHA,
	GE_DSTBLEND_DSTALPHA,
	GE_DSTBLEND_INVDSTALPHA,
	GE_DSTBLEND_DOUBLESRCALPHA,
	GE_DSTBLEND_DOUBLEINVSRCALPHA,
	GE_DSTBLEND_DOUBLEDSTALPHA,
	GE_DSTBLEND_DOUBLEINVDSTALPHA,
	GE_DSTBLEND_FIXB,
};

enum GETexFunc
{
	GE_TEXFUNC_MODULATE = 0,
	GE_TEXFUNC_DECAL = 1,
	GE_TEXFUNC_BLEND = 2,
	GE_TEXFUNC_REPLACE = 3,
	GE_TEXFUNC_ADD = 4,
	GE_TEXFUNC_UNKNOWN1 = 5,
	GE_TEXFUNC_UNKNOWN2 = 6,
	GE_TEXFUNC_UNKNOWN3 = 7,
};

enum GEStencilOp
{
	GE_STENCILOP_KEEP = 0,
	GE_STENCILOP_ZERO = 1,
	GE_STENCILOP_REPLACE = 2,
	GE_STENCILOP_INVERT = 3,
	GE_STENCILOP_INCR = 4,
	GE_STENCILOP_DECR = 5,
};

enum GETextureFiltering
{
	GE_TFILT_NEAREST = 0,
	GE_TFILT_LINEAR = 1,
	GE_TFILT_NEAREST_MIPMAP_NEAREST = 4,
	GE_TFILT_LINEAR_MIPMAP_NEAREST = 5,
	GE_TFILT_NEAREST_MIPMAP_LINEAR = 6,
	GE_TFILT_LINEAR_MIPMAP_LINEAR = 7,
};

enum GETexMapMode
{
	GE_TEXMAP_TEXTURE_COORDS = 0,
	GE_TEXMAP_TEXTURE_MATRIX = 1,
	GE_TEXMAP_ENVIRONMENT_MAP = 2,
	GE_TEXMAP_UNKNOWN = 3,
};

enum GETexProjMapMode
{
	GE_PROJMAP_POSITION = 0,
	GE_PROJMAP_UV = 1,
	GE_PROJMAP_NORMALIZED_NORMAL = 2,
	GE_PROJMAP_NORMAL = 3,
};

enum GEPrimitiveType
{
	GE_PRIM_POINTS = 0,
	GE_PRIM_LINES = 1,
	GE_PRIM_LINE_STRIP = 2,
	GE_PRIM_TRIANGLES = 3,
	GE_PRIM_TRIANGLE_STRIP = 4,
	GE_PRIM_TRIANGLE_FAN = 5,
	GE_PRIM_RECTANGLES = 6,
	GE_PRIM_KEEP_PREVIOUS = 7,
	GE_PRIM_INVALID = -1,
};

enum GELogicOp
{
	GE_LOGIC_CLEAR = 0,
	GE_LOGIC_AND = 1,
	GE_LOGIC_AND_REVERSE = 2,
	GE_LOGIC_COPY = 3,
	GE_LOGIC_AND_INVERTED = 4,
	GE_LOGIC_NOOP = 5,
	GE_LOGIC_XOR = 6,
	GE_LOGIC_OR = 7,
	GE_LOGIC_NOR = 8,
	GE_LOGIC_EQUIV = 9,
	GE_LOGIC_INVERTED = 10,
	GE_LOGIC_OR_REVERSE = 11,
	GE_LOGIC_COPY_INVERTED = 12,
	GE_LOGIC_OR_INVERTED = 13,
	GE_LOGIC_NAND = 14,
	GE_LOGIC_SET = 15,
};

enum GEPatchPrimType
{
	GE_PATCHPRIM_TRIANGLES = 0,
	GE_PATCHPRIM_LINES = 1,
	GE_PATCHPRIM_POINTS = 2,
	// Treated as points.
	GE_PATCHPRIM_UNKNOWN = 3,
};

inline GEPrimitiveType PatchPrimToPrim(GEPatchPrimType type) {
	switch (type) {
	case GE_PATCHPRIM_TRIANGLES: return GE_PRIM_TRIANGLES;
	case GE_PATCHPRIM_LINES: return GE_PRIM_LINES;
	case GE_PATCHPRIM_POINTS: return GE_PRIM_POINTS;
	case GE_PATCHPRIM_UNKNOWN: default: return GE_PRIM_POINTS; // Treated as points.
	}
}

enum GEPaletteFormat
{
	GE_CMODE_16BIT_BGR5650,
	GE_CMODE_16BIT_ABGR5551,
	GE_CMODE_16BIT_ABGR4444,
	GE_CMODE_32BIT_ABGR8888,
};

const char* GEPaletteFormatToString(GEPaletteFormat pfmt);
const char* GeTextureFormatToString(GETextureFormat tfmt);
const char* GeTextureFormatToString(GETextureFormat tfmt, GEPaletteFormat pfmt);