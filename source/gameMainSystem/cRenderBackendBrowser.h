#pragma once

#include <stddef.h>
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include <tchar.h>
#include <math.h>
#include <string.h>

#ifndef D3DFVF_XYZRHW
#define D3DFVF_XYZRHW 0x00000400
#endif
#ifndef D3DFVF_DIFFUSE
#define D3DFVF_DIFFUSE 0x00000040
#endif
#ifndef D3DFVF_TEX1
#define D3DFVF_TEX1 0x00000100
#endif
#ifndef D3DFVF_TEX2
#define D3DFVF_TEX2 0x00000200
#endif

#ifndef D3DPT_TRIANGLESTRIP
#define D3DPT_TRIANGLESTRIP 5
#endif
#ifndef D3DCLEAR_TARGET
#define D3DCLEAR_TARGET 0x00000001
#endif
#ifndef D3DRS_ALPHABLENDENABLE
#define D3DRS_ALPHABLENDENABLE 27
#endif
#ifndef D3DTSS_COLOROP
#define D3DTSS_COLOROP 1
#endif
#ifndef D3DTSS_COLORARG1
#define D3DTSS_COLORARG1 2
#endif
#ifndef D3DTSS_COLORARG2
#define D3DTSS_COLORARG2 3
#endif
#ifndef D3DTSS_ALPHAOP
#define D3DTSS_ALPHAOP 4
#endif
#ifndef D3DTSS_ALPHAARG1
#define D3DTSS_ALPHAARG1 5
#endif
#ifndef D3DTSS_ALPHAARG2
#define D3DTSS_ALPHAARG2 6
#endif
#ifndef D3DTSS_TEXCOORDINDEX
#define D3DTSS_TEXCOORDINDEX 11
#endif
#ifndef D3DTOP_SELECTARG2
#define D3DTOP_SELECTARG2 3
#endif
#ifndef D3DTOP_MODULATE
#define D3DTOP_MODULATE 4
#endif
#ifndef D3DTOP_ADD
#define D3DTOP_ADD 7
#endif
#ifndef D3DTA_DIFFUSE
#define D3DTA_DIFFUSE 0
#endif
#ifndef D3DTA_CURRENT
#define D3DTA_CURRENT 1
#endif
#ifndef D3DTA_TEXTURE
#define D3DTA_TEXTURE 2
#endif
#ifndef D3DCOLOR_ARGB
#define D3DCOLOR_ARGB(a, r, g, b) ((((unsigned long)(a) & 0xff) << 24) | (((unsigned long)(r) & 0xff) << 16) | (((unsigned long)(g) & 0xff) << 8) | ((unsigned long)(b) & 0xff))
#endif
#ifndef D3DCOLOR_XRGB
#define D3DCOLOR_XRGB(r, g, b) D3DCOLOR_ARGB(255, r, g, b)
#endif

#include "../utility/sVertexes.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef FW_REGULAR
#define FW_REGULAR 400
#endif

typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef int BOOL;
typedef void* HWND;

struct cRenderInterface
{
};

struct cRenderTexture
{
	cRenderTexture() : width(0), height(0), pixels(NULL)
	{
	}

	~cRenderTexture()
	{
		delete[] pixels;
	}

	bool resize(int newWidth, int newHeight)
	{
		if(newWidth <= 0 || newHeight <= 0) return false;
		if(width == newWidth && height == newHeight && pixels != NULL) return true;

		delete[] pixels;
		pixels = NULL;
		width = newWidth;
		height = newHeight;

		pixels = new unsigned long[(size_t)width * (size_t)height];
		if(pixels == NULL) return false;
		memset(pixels, 0, (size_t)width * (size_t)height * sizeof(unsigned long));
		return true;
	}

	int width;
	int height;
	unsigned long* pixels;
};

struct cRenderSurface
{
	cRenderSurface() : texture(NULL)
	{
	}

	explicit cRenderSurface(cRenderTexture* targetTexture) : texture(targetTexture)
	{
	}

	cRenderTexture* texture;
};

struct cRenderCaps
{
};

struct cRenderDisplayMode
{
	cRenderDisplayMode() : Format(0)
	{
	}

	int Format;
};

struct cRenderPresentParameters
{
	cRenderPresentParameters() : BackBufferWidth(800), BackBufferHeight(600)
	{
	}

	DWORD BackBufferWidth;
	DWORD BackBufferHeight;
};

struct cRenderDevice
{
	cRenderDevice()
		: width(800), height(600), logicalWidth(800), logicalHeight(600),
		  viewportWidth(800), viewportHeight(600), backBufferScaleX(1.0f), backBufferScaleY(1.0f),
		  renderTarget(NULL), currentTexture(NULL), alphaBlendEnabled(false),
		  colorMode(0), blendFactors(0), blendOperation(0)
	{
		backBuffer.resize(width, height);
	}

	bool initialize(int newWidth, int newHeight)
	{
		logicalWidth = newWidth;
		logicalHeight = newHeight;
		float scale = chooseBackBufferScale();
		width = (int)((float)newWidth * scale + 0.5f);
		height = (int)((float)newHeight * scale + 0.5f);
		if(width < 1) width = 1;
		if(height < 1) height = 1;
		if(width > newWidth) width = newWidth;
		if(height > newHeight) height = newHeight;
		backBufferScaleX = (float)width / (float)logicalWidth;
		backBufferScaleY = (float)height / (float)logicalHeight;
		viewportWidth = logicalWidth;
		viewportHeight = logicalHeight;
		renderTarget = NULL;
		currentTexture = NULL;
		return backBuffer.resize(width, height);
	}

	cRenderTexture* targetTexture()
	{
		if(renderTarget != NULL && renderTarget->texture != NULL) return renderTarget->texture;
		return &backBuffer;
	}

	static float chooseBackBufferScale()
	{
#ifdef __EMSCRIPTEN__
		double scale = EM_ASM_DOUBLE({
			function clamp(value) {
				if (!isFinite(value) || value <= 0) return 1.0;
				return Math.max(0.25, Math.min(1.0, value));
			}
			if (typeof Module['ggnRenderScale'] !== 'undefined') {
				var moduleScale = parseFloat(Module['ggnRenderScale']);
				if (isFinite(moduleScale) && moduleScale > 0) return clamp(moduleScale);
			}
			var paramScale = NaN;
			try {
				paramScale = parseFloat(new URLSearchParams(location.search).get('renderScale'));
			} catch (e) {
			}
			if (isFinite(paramScale) && paramScale > 0) return clamp(paramScale);
			var coarse = false;
			try {
				coarse = !!(window.matchMedia && window.matchMedia('(pointer: coarse)').matches);
			} catch (e) {
			}
			var layoutShortSide = Math.min(window.innerWidth || 800, window.innerHeight || 600);
			var screenShortSide = layoutShortSide;
			try {
				if (window.screen && screen.width > 0 && screen.height > 0) {
					screenShortSide = Math.min(screen.width, screen.height);
				}
			} catch (e) {
			}
			var shortSide = Math.min(layoutShortSide, screenShortSide);
			if (shortSide <= 520) return 0.375;
			if (coarse || shortSide <= 700) return 0.5;
			return 1.0;
		});
		if(scale < 0.25) scale = 0.25;
		if(scale > 1.0) scale = 1.0;
		return (float)scale;
#else
		return 1.0f;
#endif
	}

	bool isBackBufferTarget(cRenderTexture* target) const
	{
		return target == &backBuffer;
	}

	int SetTexture(DWORD stage, cRenderTexture* texture);
	int SetFVF(DWORD fvf);
	int SetTextureStageState(DWORD stage, DWORD state, DWORD value);
	int SetRenderState(DWORD state, DWORD value);
	int Clear(DWORD count, const void* rects, DWORD flags, cRenderColor color, float z, DWORD stencil);
	int DrawPrimitiveUP(DWORD primitiveType, UINT primitiveCount, const void* vertices, UINT stride);

	int width;
	int height;
	int logicalWidth;
	int logicalHeight;
	int viewportWidth;
	int viewportHeight;
	float backBufferScaleX;
	float backBufferScaleY;
	cRenderTexture backBuffer;
	cRenderSurface* renderTarget;
	cRenderTexture* currentTexture;
	bool alphaBlendEnabled;
	int colorMode;
	int blendFactors;
	int blendOperation;
};

struct cRenderVector2
{
	cRenderVector2() : x(0.0f), y(0.0f)
	{
	}

	cRenderVector2(float inX, float inY) : x(inX), y(inY)
	{
	}

	float x;
	float y;
};

struct cRenderVector3
{
	cRenderVector3() : x(0.0f), y(0.0f), z(0.0f)
	{
	}

	cRenderVector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ)
	{
	}

	float x;
	float y;
	float z;
};

struct cRenderVector4
{
	cRenderVector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{
	}

	cRenderVector4(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW)
	{
	}

	float x;
	float y;
	float z;
	float w;
};

struct cRenderMatrix
{
	float m[4][4];
};

struct cRenderQuaternion
{
	cRenderQuaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	cRenderQuaternion(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW)
	{
	}

	float x;
	float y;
	float z;
	float w;
};
struct cRenderTextContext
{
	cRenderTextContext() : size(16), weight(FW_REGULAR), active(false)
	{
	}

	DWORD size;
	int weight;
	bool active;
};

struct cRenderTextTransform
{
	cRenderTextTransform() : scaleX(1.0f), scaleY(1.0f), rotation(0.0f)
	{
	}

	float scaleX;
	float scaleY;
	float rotation;
};

struct cRenderTextMetrics
{
	long ascent;
	long height;
	long externalLeading;
};

struct cRenderGlyphMetrics
{
	DWORD blackBoxX;
	DWORD blackBoxY;
	long glyphOriginY;
};

struct cRenderGlyphABC
{
	int a;
	int b;
	int c;
};

inline void cRenderEndText(cRenderTextContext& context)
{
	context.active = false;
}

inline bool cRenderBeginText(cRenderTextContext& context, const TCHAR* face, DWORD size, int weight)
{
	(void)face;
	context.size = size == 0 ? 16 : size;
	context.weight = weight;
	context.active = true;
	return true;
}

inline void cRenderBuildTextTransform(cRenderTextTransform& transform, float scaleX, float scaleY, float rotation)
{
	transform.scaleX = scaleX;
	transform.scaleY = scaleY;
	transform.rotation = rotation;
}

inline bool cRenderGetTextMetrics(cRenderTextContext& context, cRenderTextMetrics& metrics)
{
	long size = (long)context.size;
	metrics.ascent = size;
	metrics.height = size;
	metrics.externalLeading = 0;
	return true;
}

inline int cRenderApproxGlyphWidth(cRenderTextContext& context, unsigned int code)
{
#ifdef __EMSCRIPTEN__
	int measuredWidth = EM_ASM_INT({
		var code = $0 >>> 0;
		var size = $1 | 0;
		var weight = $2 | 0;
		if (size <= 0) size = 16;
		var canvas = Module['ggnGlyphMeasureCanvas'];
		if (!canvas) {
			canvas = document.createElement('canvas');
			Module['ggnGlyphMeasureCanvas'] = canvas;
		}
		var ctx = canvas.getContext('2d', { willReadFrequently: true });
		var ch = '?';
		try { ch = String.fromCodePoint(code); } catch (e) {}
		ctx.font = (weight >= 700 ? 'bold ' : '') + size + 'px Meiryo, "Yu Gothic", sans-serif';
		var width = Math.ceil(ctx.measureText(ch).width);
		return width > 0 ? width : 0;
	}, code, (int)context.size, context.weight);
	if(measuredWidth > 0) return measuredWidth;
#endif
	if(code <= 0x7f)
	{
		int width = (int)context.size / 2;
		return width > 0 ? width : 1;
	}

	return (int)context.size;
}

inline bool cRenderGetGlyphABC(cRenderTextContext& context, unsigned int code, cRenderGlyphABC& abc)
{
	abc.a = 0;
	abc.b = cRenderApproxGlyphWidth(context, code);
	abc.c = 0;
	return true;
}

inline bool cRenderBrowserGlyphPixel(unsigned int code, int x, int y, int width, int height)
{
	if(x == 0 || y == 0 || x == width - 1 || y == height - 1) return true;
	return ((x * 13 + y * 7 + (int)code) % 17) < 3;
}

inline DWORD cRenderGetGlyphBitmap(cRenderTextContext& context, unsigned int code, bool antialias, const cRenderTextTransform& transform, cRenderGlyphMetrics& metrics, BYTE** data)
{
	*data = NULL;
	int baseWidth = cRenderApproxGlyphWidth(context, code);
	int width = (int)(baseWidth * transform.scaleX);
	int height = (int)(context.size * transform.scaleY);
	if(width <= 0) width = 1;
	if(height <= 0) height = 1;

	metrics.blackBoxX = (DWORD)width;
	metrics.blackBoxY = (DWORD)height;
	metrics.glyphOriginY = height;

	if(antialias)
	{
		int pitch = (width + 3) & ~0x03;
		DWORD size = (DWORD)(pitch * height);
		*data = new BYTE[size];
		if(*data == NULL) return 0;
		memset(*data, 0, size);
#ifdef __EMSCRIPTEN__
		EM_ASM({
			var code = $0 >>> 0;
			var width = $1 | 0;
			var height = $2 | 0;
			var out = $3 >>> 0;
			var pitch = $4 | 0;
			var weight = $5 | 0;
			var canvas = Module['ggnGlyphCanvas'];
			if (!canvas) {
				canvas = document.createElement('canvas');
				Module['ggnGlyphCanvas'] = canvas;
			}
			canvas.width = width;
			canvas.height = height;
			var ctx = canvas.getContext('2d', { willReadFrequently: true });
			ctx.clearRect(0, 0, width, height);
			ctx.fillStyle = '#fff';
			ctx.textBaseline = 'top';
			ctx.font = (weight >= 700 ? 'bold ' : '') + height + 'px Meiryo, "Yu Gothic", sans-serif';
			var ch = '?';
			try { ch = String.fromCodePoint(code); } catch (e) {}
			ctx.fillText(ch, 0, 0);
			var pixels = ctx.getImageData(0, 0, width, height).data;
			for (var y = 0; y < height; ++y) {
				for (var x = 0; x < width; ++x) {
					var alpha = pixels[(y * width + x) * 4 + 3];
					HEAPU8[out + y * pitch + x] = Math.min(64, Math.round(alpha * 64 / 255));
				}
			}
		}, code, width, height, *data, pitch, context.weight);
		return size;
#endif
		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				(*data)[y * pitch + x] = cRenderBrowserGlyphPixel(code, x, y, width, height) ? 64 : 0;
			}
		}
		return size;
	}

	int pitch = ((width + 0x1f) & ~0x1f) / 8;
	DWORD size = (DWORD)(pitch * height);
	*data = new BYTE[size];
	if(*data == NULL) return 0;
	memset(*data, 0, size);
#ifdef __EMSCRIPTEN__
	EM_ASM({
		var code = $0 >>> 0;
		var width = $1 | 0;
		var height = $2 | 0;
		var out = $3 >>> 0;
		var pitch = $4 | 0;
		var weight = $5 | 0;
		var canvas = Module['ggnGlyphCanvas'];
		if (!canvas) {
			canvas = document.createElement('canvas');
			Module['ggnGlyphCanvas'] = canvas;
		}
		canvas.width = width;
		canvas.height = height;
		var ctx = canvas.getContext('2d', { willReadFrequently: true });
		ctx.clearRect(0, 0, width, height);
		ctx.fillStyle = '#fff';
		ctx.textBaseline = 'top';
		ctx.font = (weight >= 700 ? 'bold ' : '') + height + 'px Meiryo, "Yu Gothic", sans-serif';
		var ch = '?';
		try { ch = String.fromCodePoint(code); } catch (e) {}
		ctx.fillText(ch, 0, 0);
		var pixels = ctx.getImageData(0, 0, width, height).data;
		for (var y = 0; y < height; ++y) {
			for (var x = 0; x < width; ++x) {
				var alpha = pixels[(y * width + x) * 4 + 3];
				if (alpha > 32) HEAPU8[out + y * pitch + (x >> 3)] |= 0x80 >> (x & 7);
			}
		}
	}, code, width, height, *data, pitch, context.weight);
	return size;
#endif
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			if(cRenderBrowserGlyphPixel(code, x, y, width, height))
			{
				(*data)[y * pitch + x / 8] |= (BYTE)(0x80 >> (x % 8));
			}
		}
	}
	return size;
}
inline void cRenderMatrixIdentity(cRenderMatrix* out)
{
	for(int row = 0; row < 4; ++row)
	{
		for(int column = 0; column < 4; ++column)
		{
			out->m[row][column] = row == column ? 1.0f : 0.0f;
		}
	}
}

inline cRenderVector3 cRenderVec3Cross(const cRenderVector3& a, const cRenderVector3& b)
{
	return cRenderVector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}

inline float cRenderVec3Dot(const cRenderVector3& a, const cRenderVector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float cRenderVec3Length(const cRenderVector3* vector)
{
	return sqrtf(cRenderVec3Dot(*vector, *vector));
}

inline cRenderVector3* cRenderVec3Normalize(cRenderVector3* out, const cRenderVector3* vector)
{
	float length = cRenderVec3Length(vector);
	if(length <= 0.0f)
	{
		out->x = 0.0f;
		out->y = 0.0f;
		out->z = 0.0f;
		return out;
	}

	out->x = vector->x / length;
	out->y = vector->y / length;
	out->z = vector->z / length;
	return out;
}

inline cRenderMatrix* cRenderMatrixRotationX(cRenderMatrix* out, float angle)
{
	cRenderMatrixIdentity(out);
	float c = cosf(angle);
	float s = sinf(angle);
	out->m[1][1] = c;
	out->m[1][2] = s;
	out->m[2][1] = -s;
	out->m[2][2] = c;
	return out;
}

inline cRenderMatrix* cRenderMatrixRotationY(cRenderMatrix* out, float angle)
{
	cRenderMatrixIdentity(out);
	float c = cosf(angle);
	float s = sinf(angle);
	out->m[0][0] = c;
	out->m[0][2] = -s;
	out->m[2][0] = s;
	out->m[2][2] = c;
	return out;
}

inline cRenderMatrix* cRenderMatrixRotationZ(cRenderMatrix* out, float angle)
{
	cRenderMatrixIdentity(out);
	float c = cosf(angle);
	float s = sinf(angle);
	out->m[0][0] = c;
	out->m[0][1] = s;
	out->m[1][0] = -s;
	out->m[1][1] = c;
	return out;
}

inline cRenderMatrix operator*(const cRenderMatrix& a, const cRenderMatrix& b)
{
	cRenderMatrix result;
	for(int row = 0; row < 4; ++row)
	{
		for(int column = 0; column < 4; ++column)
		{
			result.m[row][column] = 0.0f;
			for(int k = 0; k < 4; ++k)
			{
				result.m[row][column] += a.m[row][k] * b.m[k][column];
			}
		}
	}
	return result;
}

inline cRenderMatrix operator+(const cRenderMatrix& a, const cRenderMatrix& b)
{
	cRenderMatrix result;
	for(int row = 0; row < 4; ++row)
		for(int column = 0; column < 4; ++column)
			result.m[row][column] = a.m[row][column] + b.m[row][column];
	return result;
}

inline cRenderMatrix operator-(const cRenderMatrix& a, const cRenderMatrix& b)
{
	cRenderMatrix result;
	for(int row = 0; row < 4; ++row)
		for(int column = 0; column < 4; ++column)
			result.m[row][column] = a.m[row][column] - b.m[row][column];
	return result;
}

inline cRenderMatrix operator*(const cRenderMatrix& a, double b)
{
	cRenderMatrix result;
	for(int row = 0; row < 4; ++row)
		for(int column = 0; column < 4; ++column)
			result.m[row][column] = (float)(a.m[row][column] * b);
	return result;
}

inline cRenderMatrix operator*(double a, const cRenderMatrix& b)
{
	return b * a;
}

inline cRenderMatrix operator/(const cRenderMatrix& a, double b)
{
	return a * (1.0 / b);
}

inline bool operator==(const cRenderMatrix& a, const cRenderMatrix& b)
{
	for(int row = 0; row < 4; ++row)
		for(int column = 0; column < 4; ++column)
			if(a.m[row][column] != b.m[row][column]) return false;
	return true;
}

inline bool operator!=(const cRenderMatrix& a, const cRenderMatrix& b)
{
	return !(a == b);
}

inline cRenderVector3* cRenderVec3TransformCoord(cRenderVector3* out, const cRenderVector3* vector, const cRenderMatrix* matrix)
{
	float x = vector->x * matrix->m[0][0] + vector->y * matrix->m[1][0] + vector->z * matrix->m[2][0] + matrix->m[3][0];
	float y = vector->x * matrix->m[0][1] + vector->y * matrix->m[1][1] + vector->z * matrix->m[2][1] + matrix->m[3][1];
	float z = vector->x * matrix->m[0][2] + vector->y * matrix->m[1][2] + vector->z * matrix->m[2][2] + matrix->m[3][2];
	float w = vector->x * matrix->m[0][3] + vector->y * matrix->m[1][3] + vector->z * matrix->m[2][3] + matrix->m[3][3];
	if(w != 0.0f)
	{
		x /= w;
		y /= w;
		z /= w;
	}
	out->x = x;
	out->y = y;
	out->z = z;
	return out;
}

inline cRenderMatrix* cRenderMatrixLookAtLH(cRenderMatrix* out, const cRenderVector3* eye, const cRenderVector3* at, const cRenderVector3* up)
{
	cRenderVector3 zaxis;
	cRenderVector3 look(at->x - eye->x, at->y - eye->y, at->z - eye->z);
	cRenderVec3Normalize(&zaxis, &look);
	cRenderVector3 xaxis;
	cRenderVector3 cross = cRenderVec3Cross(*up, zaxis);
	cRenderVec3Normalize(&xaxis, &cross);
	cRenderVector3 yaxis = cRenderVec3Cross(zaxis, xaxis);

	out->m[0][0] = xaxis.x; out->m[0][1] = yaxis.x; out->m[0][2] = zaxis.x; out->m[0][3] = 0.0f;
	out->m[1][0] = xaxis.y; out->m[1][1] = yaxis.y; out->m[1][2] = zaxis.y; out->m[1][3] = 0.0f;
	out->m[2][0] = xaxis.z; out->m[2][1] = yaxis.z; out->m[2][2] = zaxis.z; out->m[2][3] = 0.0f;
	out->m[3][0] = -cRenderVec3Dot(xaxis, *eye); out->m[3][1] = -cRenderVec3Dot(yaxis, *eye); out->m[3][2] = -cRenderVec3Dot(zaxis, *eye); out->m[3][3] = 1.0f;
	return out;
}

inline cRenderMatrix* cRenderMatrixLookAtRH(cRenderMatrix* out, const cRenderVector3* eye, const cRenderVector3* at, const cRenderVector3* up)
{
	cRenderVector3 zaxis;
	cRenderVector3 look(eye->x - at->x, eye->y - at->y, eye->z - at->z);
	cRenderVec3Normalize(&zaxis, &look);
	cRenderVector3 xaxis;
	cRenderVector3 cross = cRenderVec3Cross(*up, zaxis);
	cRenderVec3Normalize(&xaxis, &cross);
	cRenderVector3 yaxis = cRenderVec3Cross(zaxis, xaxis);

	out->m[0][0] = xaxis.x; out->m[0][1] = yaxis.x; out->m[0][2] = zaxis.x; out->m[0][3] = 0.0f;
	out->m[1][0] = xaxis.y; out->m[1][1] = yaxis.y; out->m[1][2] = zaxis.y; out->m[1][3] = 0.0f;
	out->m[2][0] = xaxis.z; out->m[2][1] = yaxis.z; out->m[2][2] = zaxis.z; out->m[2][3] = 0.0f;
	out->m[3][0] = -cRenderVec3Dot(xaxis, *eye); out->m[3][1] = -cRenderVec3Dot(yaxis, *eye); out->m[3][2] = -cRenderVec3Dot(zaxis, *eye); out->m[3][3] = 1.0f;
	return out;
}

inline cRenderMatrix* cRenderMatrixRotationYawPitchRoll(cRenderMatrix* out, float yaw, float pitch, float roll)
{
	cRenderMatrix yawMatrix;
	cRenderMatrix pitchMatrix;
	cRenderMatrix rollMatrix;
	cRenderMatrixRotationY(&yawMatrix, yaw);
	cRenderMatrixRotationX(&pitchMatrix, pitch);
	cRenderMatrixRotationZ(&rollMatrix, roll);
	*out = rollMatrix * pitchMatrix * yawMatrix;
	return out;
}

inline cRenderQuaternion* cRenderQuaternionRotationAxis(cRenderQuaternion* out, const cRenderVector3* axis, float angle)
{
	cRenderVector3 normalized;
	cRenderVec3Normalize(&normalized, axis);
	float halfAngle = angle * 0.5f;
	float s = sinf(halfAngle);
	out->x = normalized.x * s;
	out->y = normalized.y * s;
	out->z = normalized.z * s;
	out->w = cosf(halfAngle);
	return out;
}

inline cRenderQuaternion* cRenderQuaternionConjugate(cRenderQuaternion* out, const cRenderQuaternion* quaternion)
{
	out->x = -quaternion->x;
	out->y = -quaternion->y;
	out->z = -quaternion->z;
	out->w = quaternion->w;
	return out;
}

inline cRenderQuaternion* cRenderQuaternionMultiply(cRenderQuaternion* out, const cRenderQuaternion* a, const cRenderQuaternion* b)
{
	cRenderQuaternion result;
	result.x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
	result.y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
	result.z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
	result.w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
	*out = result;
	return out;
}

inline cRenderQuaternion operator*(const cRenderQuaternion& a, const cRenderQuaternion& b)
{
	cRenderQuaternion result;
	cRenderQuaternionMultiply(&result, &a, &b);
	return result;
}

inline cRenderQuaternion operator+(const cRenderQuaternion& a, const cRenderQuaternion& b)
{
	return cRenderQuaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline cRenderQuaternion operator-(const cRenderQuaternion& a, const cRenderQuaternion& b)
{
	return cRenderQuaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline cRenderQuaternion operator*(const cRenderQuaternion& a, double b)
{
	return cRenderQuaternion((float)(a.x * b), (float)(a.y * b), (float)(a.z * b), (float)(a.w * b));
}

inline cRenderQuaternion operator*(double a, const cRenderQuaternion& b)
{
	return b * a;
}

inline cRenderQuaternion operator/(const cRenderQuaternion& a, double b)
{
	return a * (1.0 / b);
}

inline bool operator==(const cRenderQuaternion& a, const cRenderQuaternion& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool operator!=(const cRenderQuaternion& a, const cRenderQuaternion& b)
{
	return !(a == b);
}

inline cRenderMatrix* cRenderMatrixRotationQuaternion(cRenderMatrix* out, const cRenderQuaternion* quaternion)
{
	float x2 = quaternion->x + quaternion->x;
	float y2 = quaternion->y + quaternion->y;
	float z2 = quaternion->z + quaternion->z;
	float xx = quaternion->x * x2;
	float yy = quaternion->y * y2;
	float zz = quaternion->z * z2;
	float xy = quaternion->x * y2;
	float xz = quaternion->x * z2;
	float yz = quaternion->y * z2;
	float wx = quaternion->w * x2;
	float wy = quaternion->w * y2;
	float wz = quaternion->w * z2;

	out->m[0][0] = 1.0f - yy - zz;
	out->m[0][1] = xy + wz;
	out->m[0][2] = xz - wy;
	out->m[0][3] = 0.0f;
	out->m[1][0] = xy - wz;
	out->m[1][1] = 1.0f - xx - zz;
	out->m[1][2] = yz + wx;
	out->m[1][3] = 0.0f;
	out->m[2][0] = xz + wy;
	out->m[2][1] = yz - wx;
	out->m[2][2] = 1.0f - xx - yy;
	out->m[2][3] = 0.0f;
	out->m[3][0] = 0.0f;
	out->m[3][1] = 0.0f;
	out->m[3][2] = 0.0f;
	out->m[3][3] = 1.0f;
	return out;
}

inline cRenderVector2 operator+(const cRenderVector2& a, const cRenderVector2& b)
{
	return cRenderVector2(a.x + b.x, a.y + b.y);
}

inline cRenderVector2 operator-(const cRenderVector2& a, const cRenderVector2& b)
{
	return cRenderVector2(a.x - b.x, a.y - b.y);
}

inline cRenderVector2 operator*(double a, const cRenderVector2& b)
{
	return cRenderVector2((float)(a * b.x), (float)(a * b.y));
}

inline cRenderVector2 operator*(const cRenderVector2& a, double b)
{
	return b * a;
}

inline cRenderVector2 operator/(const cRenderVector2& a, double b)
{
	return cRenderVector2((float)(a.x / b), (float)(a.y / b));
}

inline bool operator==(const cRenderVector2& a, const cRenderVector2& b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const cRenderVector2& a, const cRenderVector2& b)
{
	return !(a == b);
}

inline cRenderVector3 operator+(const cRenderVector3& a, const cRenderVector3& b)
{
	return cRenderVector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline cRenderVector3& operator+=(cRenderVector3& a, const cRenderVector3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

inline cRenderVector3 operator-(const cRenderVector3& a, const cRenderVector3& b)
{
	return cRenderVector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline cRenderVector3 operator-(const cRenderVector3& a)
{
	return cRenderVector3(-a.x, -a.y, -a.z);
}

inline cRenderVector3 operator*(double a, const cRenderVector3& b)
{
	return cRenderVector3((float)(a * b.x), (float)(a * b.y), (float)(a * b.z));
}

inline cRenderVector3 operator*(const cRenderVector3& a, double b)
{
	return b * a;
}

inline cRenderVector3& operator*=(cRenderVector3& a, double b)
{
	a.x = (float)(a.x * b);
	a.y = (float)(a.y * b);
	a.z = (float)(a.z * b);
	return a;
}

inline cRenderVector3 operator/(const cRenderVector3& a, double b)
{
	return cRenderVector3((float)(a.x / b), (float)(a.y / b), (float)(a.z / b));
}

inline bool operator==(const cRenderVector3& a, const cRenderVector3& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(const cRenderVector3& a, const cRenderVector3& b)
{
	return !(a == b);
}

inline cRenderVector4 operator+(const cRenderVector4& a, const cRenderVector4& b)
{
	return cRenderVector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline cRenderVector4 operator-(const cRenderVector4& a, const cRenderVector4& b)
{
	return cRenderVector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline cRenderVector4 operator*(double a, const cRenderVector4& b)
{
	return cRenderVector4((float)(a * b.x), (float)(a * b.y), (float)(a * b.z), (float)(a * b.w));
}

inline cRenderVector4 operator*(const cRenderVector4& a, double b)
{
	return b * a;
}

inline cRenderVector4 operator/(const cRenderVector4& a, double b)
{
	return cRenderVector4((float)(a.x / b), (float)(a.y / b), (float)(a.z / b), (float)(a.w / b));
}

inline bool operator==(const cRenderVector4& a, const cRenderVector4& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool operator!=(const cRenderVector4& a, const cRenderVector4& b)
{
	return !(a == b);
}

struct cRenderLockedRect
{
	cRenderLockedRect() : pBits(NULL), Pitch(0)
	{
	}

	void* pBits;
	int Pitch;
};

typedef int cRenderResult;
typedef int cRenderTextureFormat;

static const cRenderTextureFormat C_RENDER_TEXTURE_FORMAT_A8R8G8B8 = 0;

enum cRenderTextureColorMode
{
	C_RENDER_TEXTURE_COLOR_MODULATE,
	C_RENDER_TEXTURE_COLOR_ADD,
	C_RENDER_TEXTURE_COLOR_FILL,
};

enum cRenderBlendFactors
{
	C_RENDER_BLEND_FACTORS_ADDITION,
	C_RENDER_BLEND_FACTORS_NORMAL,
	C_RENDER_BLEND_FACTORS_ANTIADDITION,
	C_RENDER_BLEND_FACTORS_HALFADDITION,
};

enum cRenderBlendOperation
{
	C_RENDER_BLEND_OPERATION_ADD,
	C_RENDER_BLEND_OPERATION_REVERSE_SUBTRACT,
};

inline int cRenderClampInt(int value, int low, int high)
{
	if(value < low) return low;
	if(value > high) return high;
	return value;
}

inline float cRenderMin3(float a, float b, float c)
{
	float value = a < b ? a : b;
	return value < c ? value : c;
}

inline float cRenderMax3(float a, float b, float c)
{
	float value = a > b ? a : b;
	return value > c ? value : c;
}

inline unsigned char cRenderColorA(unsigned long color)
{
	return (unsigned char)((color >> 24) & 0xff);
}

inline unsigned char cRenderColorR(unsigned long color)
{
	return (unsigned char)((color >> 16) & 0xff);
}

inline unsigned char cRenderColorG(unsigned long color)
{
	return (unsigned char)((color >> 8) & 0xff);
}

inline unsigned char cRenderColorB(unsigned long color)
{
	return (unsigned char)(color & 0xff);
}

inline unsigned long cRenderPackColor(int alpha, int red, int green, int blue)
{
	alpha = cRenderClampInt(alpha, 0, 255);
	red = cRenderClampInt(red, 0, 255);
	green = cRenderClampInt(green, 0, 255);
	blue = cRenderClampInt(blue, 0, 255);
	return ((unsigned long)alpha << 24) | ((unsigned long)red << 16) | ((unsigned long)green << 8) | (unsigned long)blue;
}

inline unsigned long cRenderInterpolateColor(unsigned long c0, unsigned long c1, unsigned long c2, float w0, float w1, float w2)
{
	int alpha = (int)(cRenderColorA(c0) * w0 + cRenderColorA(c1) * w1 + cRenderColorA(c2) * w2 + 0.5f);
	int red = (int)(cRenderColorR(c0) * w0 + cRenderColorR(c1) * w1 + cRenderColorR(c2) * w2 + 0.5f);
	int green = (int)(cRenderColorG(c0) * w0 + cRenderColorG(c1) * w1 + cRenderColorG(c2) * w2 + 0.5f);
	int blue = (int)(cRenderColorB(c0) * w0 + cRenderColorB(c1) * w1 + cRenderColorB(c2) * w2 + 0.5f);
	return cRenderPackColor(alpha, red, green, blue);
}

inline unsigned long cRenderSampleTexture(cRenderTexture* texture, float u, float v)
{
	if(texture == NULL || texture->pixels == NULL || texture->width <= 0 || texture->height <= 0)
	{
		return 0xffffffff;
	}

	// cDrawingObject already nudges UVs by almost half a texel for D3D9.
	// Match that point-sampling convention when software-rasterizing enlarged tiles.
	int x = (int)floorf(u * (float)texture->width - 0.5f);
	int y = (int)floorf(v * (float)texture->height - 0.5f);
	x = cRenderClampInt(x, 0, texture->width - 1);
	y = cRenderClampInt(y, 0, texture->height - 1);
	return texture->pixels[y * texture->width + x];
}

inline unsigned long cRenderApplyTextureColor(cRenderDevice* device, unsigned long textureColor, unsigned long vertexColor)
{
	int alpha = cRenderColorA(textureColor) * cRenderColorA(vertexColor) / 255;
	int red;
	int green;
	int blue;

	if(device != NULL && device->colorMode == C_RENDER_TEXTURE_COLOR_ADD)
	{
		red = cRenderColorR(textureColor) + cRenderColorR(vertexColor);
		green = cRenderColorG(textureColor) + cRenderColorG(vertexColor);
		blue = cRenderColorB(textureColor) + cRenderColorB(vertexColor);
	}
	else if(device != NULL && device->colorMode == C_RENDER_TEXTURE_COLOR_FILL)
	{
		red = cRenderColorR(vertexColor);
		green = cRenderColorG(vertexColor);
		blue = cRenderColorB(vertexColor);
	}
	else
	{
		red = cRenderColorR(textureColor) * cRenderColorR(vertexColor) / 255;
		green = cRenderColorG(textureColor) * cRenderColorG(vertexColor) / 255;
		blue = cRenderColorB(textureColor) * cRenderColorB(vertexColor) / 255;
	}

	return cRenderPackColor(alpha, red, green, blue);
}

inline unsigned long cRenderBlendColor(cRenderDevice* device, unsigned long destination, unsigned long source)
{
	if(device == NULL || !device->alphaBlendEnabled)
	{
		return source;
	}

	int srcA = cRenderColorA(source);
	int srcR = cRenderColorR(source);
	int srcG = cRenderColorG(source);
	int srcB = cRenderColorB(source);
	int dstA = cRenderColorA(destination);
	int dstR = cRenderColorR(destination);
	int dstG = cRenderColorG(destination);
	int dstB = cRenderColorB(destination);
	int outA = 255;
	int outR = srcR;
	int outG = srcG;
	int outB = srcB;

	if(device->blendOperation == C_RENDER_BLEND_OPERATION_REVERSE_SUBTRACT)
	{
		outA = dstA;
		outR = dstR - (srcR * srcA / 255);
		outG = dstG - (srcG * srcA / 255);
		outB = dstB - (srcB * srcA / 255);
		return cRenderPackColor(outA, outR, outG, outB);
	}

	if(device->blendFactors == C_RENDER_BLEND_FACTORS_ADDITION)
	{
		outA = dstA > srcA ? dstA : srcA;
		outR = dstR + srcR * srcA / 255;
		outG = dstG + srcG * srcA / 255;
		outB = dstB + srcB * srcA / 255;
	}
	else if(device->blendFactors == C_RENDER_BLEND_FACTORS_ANTIADDITION)
	{
		outA = dstA;
		outR = dstR * (255 - srcR) / 255;
		outG = dstG * (255 - srcG) / 255;
		outB = dstB * (255 - srcB) / 255;
	}
	else if(device->blendFactors == C_RENDER_BLEND_FACTORS_HALFADDITION)
	{
		outA = dstA > srcA ? dstA : srcA;
		outR = srcR + dstR * (255 - srcA) / 255;
		outG = srcG + dstG * (255 - srcA) / 255;
		outB = srcB + dstB * (255 - srcA) / 255;
	}
	else
	{
		outA = srcA + dstA * (255 - srcA) / 255;
		outR = (srcR * srcA + dstR * (255 - srcA)) / 255;
		outG = (srcG * srcA + dstG * (255 - srcA)) / 255;
		outB = (srcB * srcA + dstB * (255 - srcA)) / 255;
	}

	return cRenderPackColor(outA, outR, outG, outB);
}

struct cRenderDrawVertex
{
	float x;
	float y;
	float tu;
	float tv;
	unsigned long color;
};

inline float cRenderEdge(float ax, float ay, float bx, float by, float px, float py)
{
	return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

inline void cRenderDrawSoftwareTriangle(cRenderDevice* device, const cRenderDrawVertex& v0, const cRenderDrawVertex& v1, const cRenderDrawVertex& v2)
{
	if(device == NULL) return;
	cRenderTexture* target = device->targetTexture();
	if(target == NULL || target->pixels == NULL) return;

	cRenderDrawVertex sv0 = v0;
	cRenderDrawVertex sv1 = v1;
	cRenderDrawVertex sv2 = v2;
	if(device->isBackBufferTarget(target))
	{
		sv0.x *= device->backBufferScaleX;
		sv0.y *= device->backBufferScaleY;
		sv1.x *= device->backBufferScaleX;
		sv1.y *= device->backBufferScaleY;
		sv2.x *= device->backBufferScaleX;
		sv2.y *= device->backBufferScaleY;
	}

	float area = cRenderEdge(sv0.x, sv0.y, sv1.x, sv1.y, sv2.x, sv2.y);
	if(area > -0.0001f && area < 0.0001f) return;

	int minX = cRenderClampInt((int)floorf(cRenderMin3(sv0.x, sv1.x, sv2.x)), 0, target->width - 1);
	int maxX = cRenderClampInt((int)ceilf(cRenderMax3(sv0.x, sv1.x, sv2.x)), 0, target->width - 1);
	int minY = cRenderClampInt((int)floorf(cRenderMin3(sv0.y, sv1.y, sv2.y)), 0, target->height - 1);
	int maxY = cRenderClampInt((int)ceilf(cRenderMax3(sv0.y, sv1.y, sv2.y)), 0, target->height - 1);

	for(int y = minY; y <= maxY; ++y)
	{
		for(int x = minX; x <= maxX; ++x)
		{
			float px = (float)x + 0.5f;
			float py = (float)y + 0.5f;
			float w0 = cRenderEdge(sv1.x, sv1.y, sv2.x, sv2.y, px, py) / area;
			float w1 = cRenderEdge(sv2.x, sv2.y, sv0.x, sv0.y, px, py) / area;
			float w2 = cRenderEdge(sv0.x, sv0.y, sv1.x, sv1.y, px, py) / area;
			if(w0 < -0.0001f || w1 < -0.0001f || w2 < -0.0001f) continue;

			float u = sv0.tu * w0 + sv1.tu * w1 + sv2.tu * w2;
			float v = sv0.tv * w0 + sv1.tv * w1 + sv2.tv * w2;
			unsigned long vertexColor = cRenderInterpolateColor(sv0.color, sv1.color, sv2.color, w0, w1, w2);
			unsigned long textureColor = cRenderSampleTexture(device->currentTexture, u, v);
			unsigned long source = cRenderApplyTextureColor(device, textureColor, vertexColor);
			unsigned long& destination = target->pixels[y * target->width + x];
			destination = cRenderBlendColor(device, destination, source);
		}
	}
}

inline void cRenderSetViewport(cRenderDevice* device, DWORD width, DWORD height)
{
	if(device == NULL) return;
	device->viewportWidth = (int)width;
	device->viewportHeight = (int)height;
}

template<class T>
inline void cRenderRelease(T*& resource)
{
	if(resource != NULL)
	{
		delete resource;
		resource = NULL;
	}
}

inline cRenderInterface* cRenderCreateInterface()
{
	return new cRenderInterface();
}

inline bool cRenderSucceeded(cRenderResult result)
{
	return result == 0;
}

inline bool cRenderFailed(cRenderResult result)
{
	return !cRenderSucceeded(result);
}

inline bool cRenderIsDriverInternalError(cRenderResult result)
{
	(void)result;
	return false;
}

inline bool cRenderIsDeviceLost(cRenderResult result)
{
	(void)result;
	return false;
}

inline bool cRenderIsDeviceNotReset(cRenderResult result)
{
	(void)result;
	return false;
}

inline cRenderColor cRenderColorARGB(BYTE alpha, BYTE red, BYTE green, BYTE blue)
{
	return ((cRenderColor)((((alpha)&0xff)<<24)|(((red)&0xff)<<16)|(((green)&0xff)<<8)|((blue)&0xff)));
}

inline cRenderColor cRenderColorRGB(BYTE red, BYTE green, BYTE blue)
{
	return cRenderColorARGB(255, red, green, blue);
}

inline cRenderResult cRenderGetAdapterDisplayMode(cRenderInterface* renderer, cRenderDisplayMode* displayMode)
{
	(void)renderer;
	if(displayMode != NULL)
		displayMode->Format = 0;
	return 0;
}

inline cRenderResult cRenderGetDeviceCaps(cRenderInterface* renderer, cRenderCaps* caps)
{
	(void)renderer;
	(void)caps;
	return 0;
}

inline void cRenderInitializePresentParameters(cRenderPresentParameters* params, HWND window, bool fullscreen, const cRenderDisplayMode& displayMode, DWORD width, DWORD height)
{
	(void)window;
	(void)fullscreen;
	(void)displayMode;
	if(params == NULL) return;
	params->BackBufferWidth = width;
	params->BackBufferHeight = height;
}

inline bool cRenderCreateBrowserDevice(cRenderPresentParameters* params, cRenderDevice** device)
{
	if(device == NULL) return false;
	*device = new cRenderDevice();
	if(*device == NULL) return false;

	int width = params != NULL ? (int)params->BackBufferWidth : 800;
	int height = params != NULL ? (int)params->BackBufferHeight : 600;
	return (*device)->initialize(width, height);
}

inline bool cRenderCreateReferenceSoftwareDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	(void)renderer;
	(void)window;
	return cRenderCreateBrowserDevice(params, device);
}

inline bool cRenderCreateHalSoftwareDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	return cRenderCreateReferenceSoftwareDevice(renderer, window, params, device);
}

inline bool cRenderCreateHalHardwareDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	return cRenderCreateReferenceSoftwareDevice(renderer, window, params, device);
}

inline bool cRenderCreateHalMixedDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	return cRenderCreateReferenceSoftwareDevice(renderer, window, params, device);
}

inline bool cRenderGetTextureSize(cRenderTexture* texture, int& width, int& height)
{
	if(texture == NULL) return false;

	width = texture->width;
	height = texture->height;
	return true;
}

inline bool cRenderCreateManagedTexture(cRenderDevice* device, DWORD width, DWORD height, cRenderTexture** texture)
{
	(void)device;
	if(texture == NULL) return false;
	*texture = new cRenderTexture();
	if(*texture == NULL) return false;

	if(!(*texture)->resize((int)width, (int)height))
	{
		delete *texture;
		*texture = NULL;
		return false;
	}
	return true;
}

inline UINT cRenderReadLittleEndian32(const BYTE* data)
{
	return ((UINT)data[0]) | ((UINT)data[1] << 8) | ((UINT)data[2] << 16) | ((UINT)data[3] << 24);
}

inline bool cRenderRawTextureMagicMatches(const BYTE* data, UINT dataSize)
{
	static const BYTE magic[8] = { 'G', 'G', 'N', 'T', 'E', 'X', '1', 0 };
	return data != NULL && dataSize >= 16 && memcmp(data, magic, sizeof(magic)) == 0;
}

inline void cRenderApplyColorKey(cRenderTexture* texture, DWORD colorkey)
{
	if(texture == NULL || texture->pixels == NULL || colorkey == 0) return;

	unsigned long key = colorkey & 0x00ffffffUL;
	int count = texture->width * texture->height;
	for(int i = 0; i < count; ++i)
	{
		if((texture->pixels[i] & 0x00ffffffUL) == key)
		{
			texture->pixels[i] &= 0x00ffffffUL;
		}
	}
}

inline bool cRenderCreateTextureFromRawMemory(const BYTE* data, UINT dataSize, DWORD colorkey, cRenderTexture** texture)
{
	if(texture == NULL) return false;
	*texture = NULL;
	if(!cRenderRawTextureMagicMatches(data, dataSize)) return false;

	UINT width = cRenderReadLittleEndian32(data + 8);
	UINT height = cRenderReadLittleEndian32(data + 12);
	if(width == 0 || height == 0) return false;

	size_t pixelCount = (size_t)width * (size_t)height;
	if(width > 16384 || height > 16384 || pixelCount > (((size_t)-1) / 4)) return false;
	size_t pixelBytes = pixelCount * 4;
	if((size_t)dataSize < 16 + pixelBytes) return false;

	cRenderTexture* rawTexture = new cRenderTexture();
	if(rawTexture == NULL) return false;
	if(!rawTexture->resize((int)width, (int)height))
	{
		delete rawTexture;
		return false;
	}

	memcpy(rawTexture->pixels, data + 16, pixelBytes);
	cRenderApplyColorKey(rawTexture, colorkey);
	*texture = rawTexture;
	return true;
}

inline bool cRenderLoadRawTextureFromFile(const char* path, DWORD colorkey, cRenderTexture** texture)
{
	if(texture == NULL) return false;
	*texture = NULL;
	if(path == NULL) return false;

	FILE* file = fopen(path, "rb");
	if(file == NULL) return false;

	BYTE header[16];
	size_t headerRead = fread(header, 1, sizeof(header), file);
	if(headerRead != sizeof(header) || !cRenderRawTextureMagicMatches(header, sizeof(header)))
	{
		fclose(file);
		return false;
	}

	UINT width = cRenderReadLittleEndian32(header + 8);
	UINT height = cRenderReadLittleEndian32(header + 12);
	if(width == 0 || height == 0 || width > 16384 || height > 16384)
	{
		fclose(file);
		return false;
	}

	cRenderTexture* rawTexture = new cRenderTexture();
	if(rawTexture == NULL)
	{
		fclose(file);
		return false;
	}
	if(!rawTexture->resize((int)width, (int)height))
	{
		delete rawTexture;
		fclose(file);
		return false;
	}

	size_t pixelCount = (size_t)width * (size_t)height;
	size_t readCount = fread(rawTexture->pixels, sizeof(unsigned long), pixelCount, file);
	fclose(file);
	if(readCount != pixelCount)
	{
		delete rawTexture;
		return false;
	}

	cRenderApplyColorKey(rawTexture, colorkey);
	*texture = rawTexture;
	return true;
}

inline bool cRenderLoadTextureFromFile(cRenderDevice* device, const void* filePath, cRenderTexture** texture)
{
	(void)device;
	if(texture != NULL) *texture = NULL;
	if(filePath == NULL || texture == NULL) return false;

	const TCHAR* path = static_cast<const TCHAR*>(filePath);
	std::string rawPath = ggn_tchar_to_path(path) + ".ggntex";
	return cRenderLoadRawTextureFromFile(rawPath.c_str(), 0, texture);
}

inline bool cRenderCreateTextureFromMemory(cRenderDevice* device, const BYTE* data, UINT dataSize, cRenderTextureFormat format, DWORD colorkey, cRenderTexture** texture)
{
	(void)device;
	(void)format;
	return cRenderCreateTextureFromRawMemory(data, dataSize, colorkey, texture);
}
inline bool cRenderCreateRenderTargetTexture(cRenderDevice* device, int width, int height, cRenderTexture** texture)
{
	return cRenderCreateManagedTexture(device, (DWORD)width, (DWORD)height, texture);
}

inline bool cRenderGetSurfaceFromTexture(cRenderTexture* texture, cRenderSurface** surface)
{
	if(texture == NULL || surface == NULL) return false;

	*surface = new cRenderSurface(texture);
	return *surface != NULL;
}

inline bool cRenderGetBackBuffer(cRenderDevice* device, cRenderSurface** surface)
{
	if(device == NULL || surface == NULL) return false;

	*surface = new cRenderSurface(&device->backBuffer);
	return *surface != NULL;
}

inline bool cRenderLockTexture(cRenderTexture* texture, cRenderLockedRect& rect)
{
	if(texture == NULL) return false;

	if(texture->pixels == NULL && !texture->resize(texture->width, texture->height)) return false;

	if(texture->pixels == NULL) return false;

	rect.pBits = texture->pixels;
	rect.Pitch = texture->width * sizeof(unsigned long);
	return true;
}

inline void cRenderUnlockTexture(cRenderTexture* texture)
{
	(void)texture;
}

inline void cRenderSetRenderTarget(cRenderDevice* device, DWORD index, cRenderSurface* surface)
{
	(void)index;
	if(device == NULL) return;
	device->renderTarget = surface;
}

inline void cRenderClearTarget(cRenderDevice* device, cRenderColor color)
{
	if(device == NULL) return;
	cRenderTexture* target = device->targetTexture();
	if(target == NULL || target->pixels == NULL) return;

	for(int i = 0; i < target->width * target->height; ++i)
	{
		target->pixels[i] = color;
	}
}

inline cRenderResult cRenderPresent(cRenderDevice* device)
{
	if(device == NULL || device->backBuffer.pixels == NULL) return 1;
#ifdef __EMSCRIPTEN__
	EM_ASM({
		var ptr = $0 >>> 0;
		var width = $1 | 0;
		var height = $2 | 0;
		var canvas = Module['canvas'] || document.getElementById('canvas');
		if (!canvas) {
			canvas = document.createElement('canvas');
			canvas.id = 'canvas';
			document.body.appendChild(canvas);
			Module['canvas'] = canvas;
		}
		if (canvas.width !== width) canvas.width = width;
		if (canvas.height !== height) canvas.height = height;
		var context = Module['ggnCanvasContext'];
		if (!context || Module['ggnCanvas'] !== canvas) {
			context = canvas.getContext('2d');
			Module['ggnCanvasContext'] = context;
			Module['ggnCanvas'] = canvas;
			context.imageSmoothingEnabled = false;
		}
		var image = Module['ggnImageData'];
		if (!image || image.width !== width || image.height !== height) {
			image = context.createImageData(width, height);
			Module['ggnImageData'] = image;
		}
		var pixels = HEAPU32.subarray(ptr >>> 2, (ptr >>> 2) + width * height);
		var output = image.data;
		var output32 = Module['ggnImageData32'];
		if (!output32 || output32.buffer !== output.buffer) {
			output32 = new Uint32Array(output.buffer);
			Module['ggnImageData32'] = output32;
		}
		for (var i = 0; i < pixels.length; ++i) {
			var color = pixels[i] >>> 0;
			output32[i] = (color & 0xff000000) | ((color & 0x000000ff) << 16) | (color & 0x0000ff00) | ((color & 0x00ff0000) >>> 16);
		}
		context.putImageData(image, 0, 0);
		var now = performance.now();
		Module['ggnPresentCount'] = (Module['ggnPresentCount'] || 0) + 1;
		if (!Module['ggnPresentFpsStart']) {
			Module['ggnPresentFpsStart'] = now;
			Module['ggnPresentFpsFrames'] = 0;
		}
		Module['ggnPresentFpsFrames'] = (Module['ggnPresentFpsFrames'] || 0) + 1;
		var elapsed = now - Module['ggnPresentFpsStart'];
		if (elapsed >= 1000) {
			Module['ggnPresentFps'] = Module['ggnPresentFpsFrames'] * 1000 / elapsed;
			Module['ggnPresentFpsStart'] = now;
			Module['ggnPresentFpsFrames'] = 0;
		}
		var renderInfo = Module['ggnRenderInfo'];
		if (!renderInfo) {
			renderInfo = new Object();
			Module['ggnRenderInfo'] = renderInfo;
		}
		renderInfo['width'] = width;
		renderInfo['height'] = height;
		renderInfo['logicalWidth'] = $3 | 0;
		renderInfo['logicalHeight'] = $4 | 0;
		renderInfo['scaleX'] = +$5;
		renderInfo['scaleY'] = +$6;
		renderInfo['presentFps'] = Module['ggnPresentFps'] || 0;
	}, device->backBuffer.pixels, device->backBuffer.width, device->backBuffer.height, device->logicalWidth, device->logicalHeight, device->backBufferScaleX, device->backBufferScaleY);
#endif
	return 0;
}

inline cRenderResult cRenderTestCooperativeLevel(cRenderDevice* device)
{
	(void)device;
	return 0;
}

inline cRenderResult cRenderResetDevice(cRenderDevice* device, cRenderPresentParameters* params)
{
	if(device == NULL) return 1;
	int width = params != NULL ? (int)params->BackBufferWidth : device->width;
	int height = params != NULL ? (int)params->BackBufferHeight : device->height;
	return device->initialize(width, height) ? 0 : 1;
}

inline bool cRenderBeginScene(cRenderDevice* device)
{
	(void)device;
	return true;
}

inline void cRenderEndScene(cRenderDevice* device)
{
	(void)device;
}

inline void cRenderSetCullNone(cRenderDevice* device)
{
	(void)device;
}

inline void cRenderSetPointSampler(cRenderDevice* device, DWORD sampler)
{
	(void)device;
	(void)sampler;
}

inline void cRenderSetTexture(cRenderDevice* device, DWORD stage, cRenderTexture* texture)
{
	(void)stage;
	if(device != NULL) device->currentTexture = texture;
}

inline void cRenderSetTexturedVertexFormat(cRenderDevice* device)
{
	(void)device;
}

inline void cRenderSetColoredVertexFormat(cRenderDevice* device)
{
	(void)device;
}

inline void cRenderSetTextureCoordIndex(cRenderDevice* device, DWORD stage, DWORD coordIndex)
{
	(void)device;
	(void)stage;
	(void)coordIndex;
}

inline void cRenderSetTextureColorMode(cRenderDevice* device, DWORD stage, cRenderTextureColorMode mode)
{
	(void)stage;
	if(device != NULL) device->colorMode = mode;
}

inline void cRenderSetTextureAlphaCurrent(cRenderDevice* device, DWORD stage)
{
	(void)device;
	(void)stage;
}

inline void cRenderSetAlphaBlendEnabled(cRenderDevice* device, bool enabled)
{
	if(device != NULL) device->alphaBlendEnabled = enabled;
}

inline void cRenderSetBlendFactors(cRenderDevice* device, cRenderBlendFactors factors)
{
	if(device != NULL) device->blendFactors = factors;
}

inline void cRenderSetBlendOperation(cRenderDevice* device, cRenderBlendOperation operation)
{
	if(device != NULL) device->blendOperation = operation;
}

inline void cRenderDrawTriangleStrip(cRenderDevice* device, UINT primitiveCount, const VERTEX2D* vertices)
{
	if(device == NULL || vertices == NULL || primitiveCount == 0) return;
	for(UINT i = 0; i < primitiveCount; ++i)
	{
		cRenderDrawVertex v0 = { vertices[i].x, vertices[i].y, vertices[i].tu, vertices[i].tv, 0xffffffff };
		cRenderDrawVertex v1 = { vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].tu, vertices[i + 1].tv, 0xffffffff };
		cRenderDrawVertex v2 = { vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].tu, vertices[i + 2].tv, 0xffffffff };
		if((i & 1) != 0)
		{
			cRenderDrawSoftwareTriangle(device, v1, v0, v2);
		}
		else
		{
			cRenderDrawSoftwareTriangle(device, v0, v1, v2);
		}
	}
}

inline void cRenderDrawColoredTriangleStrip(cRenderDevice* device, UINT primitiveCount, const VERTEX2D_COLORED* vertices)
{
	if(device == NULL || vertices == NULL || primitiveCount == 0) return;
	for(UINT i = 0; i < primitiveCount; ++i)
	{
		cRenderDrawVertex v0 = { vertices[i].x, vertices[i].y, vertices[i].tu, vertices[i].tv, vertices[i].col };
		cRenderDrawVertex v1 = { vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].tu, vertices[i + 1].tv, vertices[i + 1].col };
		cRenderDrawVertex v2 = { vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].tu, vertices[i + 2].tv, vertices[i + 2].col };
		if((i & 1) != 0)
		{
			cRenderDrawSoftwareTriangle(device, v1, v0, v2);
		}
		else
		{
			cRenderDrawSoftwareTriangle(device, v0, v1, v2);
		}
	}
}

inline int cRenderDevice::SetTexture(DWORD stage, cRenderTexture* texture)
{
	cRenderSetTexture(this, stage, texture);
	return 0;
}

inline int cRenderDevice::SetFVF(DWORD fvf)
{
	(void)fvf;
	return 0;
}

inline int cRenderDevice::SetTextureStageState(DWORD stage, DWORD state, DWORD value)
{
	(void)stage;
	if(state == D3DTSS_COLOROP)
	{
		if(value == D3DTOP_ADD)
		{
			colorMode = C_RENDER_TEXTURE_COLOR_ADD;
		}
		else if(value == D3DTOP_SELECTARG2)
		{
			colorMode = C_RENDER_TEXTURE_COLOR_FILL;
		}
		else
		{
			colorMode = C_RENDER_TEXTURE_COLOR_MODULATE;
		}
	}
	return 0;
}

inline int cRenderDevice::SetRenderState(DWORD state, DWORD value)
{
	if(state == D3DRS_ALPHABLENDENABLE)
	{
		alphaBlendEnabled = value != FALSE;
	}
	return 0;
}

inline int cRenderDevice::Clear(DWORD count, const void* rects, DWORD flags, cRenderColor color, float z, DWORD stencil)
{
	(void)count;
	(void)rects;
	(void)z;
	(void)stencil;
	if((flags & D3DCLEAR_TARGET) != 0)
	{
		cRenderClearTarget(this, color);
	}
	return 0;
}

inline int cRenderDevice::DrawPrimitiveUP(DWORD primitiveType, UINT primitiveCount, const void* vertices, UINT stride)
{
	if(primitiveType != D3DPT_TRIANGLESTRIP || vertices == NULL) return 0;
	if(stride == sizeof(VERTEX2D_COLORED))
	{
		cRenderDrawColoredTriangleStrip(this, primitiveCount, (const VERTEX2D_COLORED*)vertices);
	}
	else if(stride == sizeof(VERTEX2D))
	{
		cRenderDrawTriangleStrip(this, primitiveCount, (const VERTEX2D*)vertices);
	}
	return 0;
}
