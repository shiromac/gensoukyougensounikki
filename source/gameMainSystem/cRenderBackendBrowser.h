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
	cRenderTexture() : width(0), height(0), pixels(NULL), webglTextureId(0), webglIsRenderTarget(false), webglDirty(true)
	{
	}

	~cRenderTexture()
	{
#ifdef __EMSCRIPTEN__
		if(webglTextureId != 0)
		{
			EM_ASM({
				var api = Module['ggnWebGLApi'];
				if (api) api.deleteTexture($0 | 0);
			}, webglTextureId);
		}
#endif
		delete[] pixels;
	}

	bool resize(int newWidth, int newHeight)
	{
		if(newWidth <= 0 || newHeight <= 0) return false;
		if(width == newWidth && height == newHeight && pixels != NULL) return true;

#ifdef __EMSCRIPTEN__
		if(webglTextureId != 0)
		{
			EM_ASM({
				var api = Module['ggnWebGLApi'];
				if (api) api.deleteTexture($0 | 0);
			}, webglTextureId);
			webglTextureId = 0;
		}
#endif
		delete[] pixels;
		pixels = NULL;
		width = newWidth;
		height = newHeight;
		webglDirty = true;

		pixels = new unsigned long[(size_t)width * (size_t)height];
		if(pixels == NULL) return false;
		memset(pixels, 0, (size_t)width * (size_t)height * sizeof(unsigned long));
		return true;
	}

	int width;
	int height;
	unsigned long* pixels;
	int webglTextureId;
	bool webglIsRenderTarget;
	bool webglDirty;
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
		  colorMode(0), blendFactors(0), blendOperation(0),
		  profileEnabled(false),
		  statsFastQuadCount(0), statsTriangleCount(0), statsFastQuadPixels(0), statsTrianglePixels(0),
		  statsClearMillis(0), statsDrawMillis(0)
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
		profileEnabled = chooseProfilingEnabled();
		viewportWidth = logicalWidth;
		viewportHeight = logicalHeight;
		renderTarget = NULL;
		currentTexture = NULL;
		resetStats();
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
			return 1.0;
		});
		if(scale < 0.25) scale = 0.25;
		if(scale > 1.0) scale = 1.0;
		return (float)scale;
#else
		return 1.0f;
#endif
	}

	static bool chooseProfilingEnabled()
	{
#ifdef __EMSCRIPTEN__
		return EM_ASM_INT({
			if (Module['ggnProfile']) return 1;
			if (typeof location !== 'undefined' && String(location.search || '').indexOf('profile') >= 0) return 1;
			return 0;
		}) != 0;
#else
		return false;
#endif
	}

	bool isBackBufferTarget(cRenderTexture* target) const
	{
		return target == &backBuffer;
	}

	void resetStats()
	{
		statsFastQuadCount = 0;
		statsTriangleCount = 0;
		statsFastQuadPixels = 0;
		statsTrianglePixels = 0;
		statsClearMillis = 0;
		statsDrawMillis = 0;
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
	bool profileEnabled;
	int statsFastQuadCount;
	int statsTriangleCount;
	double statsFastQuadPixels;
	double statsTrianglePixels;
	double statsClearMillis;
	double statsDrawMillis;
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

inline int cRenderClampInt(int value, int low, int high);

inline double cRenderNowMillis()
{
#ifdef __EMSCRIPTEN__
	return emscripten_get_now();
#else
	return 0;
#endif
}

inline bool cRenderWebGLEnsure()
{
#ifdef __EMSCRIPTEN__
	return EM_ASM_INT({
		if (Module['ggnForceSoftwareRender']) return 0;
		if (typeof location !== 'undefined' && String(location.search || '').indexOf('software=1') >= 0) return 0;
		if (Module['ggnWebGLApi']) return Module['ggnWebGLApi'].ok ? 1 : 0;

		var canvas = Module['canvas'] || document.getElementById('canvas');
		if (!canvas) return 0;
		var attributes = new Object();
		attributes.alpha = false;
		attributes.antialias = false;
		attributes.preserveDrawingBuffer = false;
		var gl = canvas.getContext('webgl', attributes) || canvas.getContext('experimental-webgl', attributes);
		if (!gl) {
			var failed = new Object();
			failed.ok = false;
			Module['ggnWebGLApi'] = failed;
			return 0;
		}

		function compileShader(type, source) {
			var shader = gl.createShader(type);
			gl.shaderSource(shader, source);
			gl.compileShader(shader);
			if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
				console.error(gl.getShaderInfoLog(shader));
				gl.deleteShader(shader);
				return null;
			}
			return shader;
		}

		var vertexSource =
			'attribute vec2 a_pos;' +
			'attribute vec2 a_uv;' +
			'attribute vec4 a_color;' +
			'uniform vec2 u_resolution;' +
			'varying vec2 v_uv;' +
			'varying vec4 v_color;' +
			'void main(){' +
			'vec2 zeroToOne=a_pos/u_resolution;' +
			'vec2 clip=zeroToOne*2.0-1.0;' +
			'gl_Position=vec4(clip*vec2(1.0,-1.0),0.0,1.0);' +
			'v_uv=a_uv;v_color=a_color;' +
			'}';
		var fragmentSource =
			'precision mediump float;' +
			'varying vec2 v_uv;' +
			'varying vec4 v_color;' +
			'uniform sampler2D u_texture;' +
			'uniform int u_hasTexture;' +
			'uniform int u_colorMode;' +
			'void main(){' +
			'vec4 tex=(u_hasTexture!=0)?texture2D(u_texture,v_uv):vec4(1.0);' +
			'if(u_colorMode==1){gl_FragColor=vec4(min(tex.rgb+v_color.rgb,vec3(1.0)),tex.a*v_color.a);}' +
			'else if(u_colorMode==2){gl_FragColor=vec4(v_color.rgb,tex.a*v_color.a);}' +
			'else{gl_FragColor=tex*v_color;}' +
			'}';
		var vertexShader = compileShader(gl.VERTEX_SHADER, vertexSource);
		var fragmentShader = compileShader(gl.FRAGMENT_SHADER, fragmentSource);
		if (!vertexShader || !fragmentShader) {
			var failedShader = new Object();
			failedShader.ok = false;
			Module['ggnWebGLApi'] = failedShader;
			return 0;
		}

		var program = gl.createProgram();
		gl.attachShader(program, vertexShader);
		gl.attachShader(program, fragmentShader);
		gl.linkProgram(program);
		if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
			console.error(gl.getProgramInfoLog(program));
			var failedProgram = new Object();
			failedProgram.ok = false;
			Module['ggnWebGLApi'] = failedProgram;
			return 0;
		}

		gl.useProgram(program);
		gl.disable(gl.DEPTH_TEST);
		gl.disable(gl.CULL_FACE);
		gl.pixelStorei(gl.UNPACK_PREMULTIPLY_ALPHA_WEBGL, false);
		gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, false);

		var api = new Object();
		api.ok = true;
		api.used = false;
		api.gl = gl;
		api.program = program;
		api.buffer = gl.createBuffer();
		api.textures = new Object();
		api.nextTextureId = 1;
		api.aPos = gl.getAttribLocation(program, 'a_pos');
		api.aUv = gl.getAttribLocation(program, 'a_uv');
		api.aColor = gl.getAttribLocation(program, 'a_color');
		api.uResolution = gl.getUniformLocation(program, 'u_resolution');
		api.uTexture = gl.getUniformLocation(program, 'u_texture');
		api.uHasTexture = gl.getUniformLocation(program, 'u_hasTexture');
		api.uColorMode = gl.getUniformLocation(program, 'u_colorMode');
		api.deleteTexture = function(id) {
				var object = this.textures[id | 0];
				if (!object) return;
				if (object.framebuffer) this.gl.deleteFramebuffer(object.framebuffer);
				if (object.texture) this.gl.deleteTexture(object.texture);
				delete this.textures[id | 0];
			};
		api.ensureTexture = function(id, width, height, ptr, renderTarget, dirty) {
				id = id | 0;
				width = width | 0;
				height = height | 0;
				renderTarget = !!renderTarget;
				var object = id ? this.textures[id] : null;
				if (!object) {
					id = this.nextTextureId++;
					object = new Object();
					object.texture = this.gl.createTexture();
					object.framebuffer = null;
					object.width = 0;
					object.height = 0;
					object.renderTarget = renderTarget;
					object.uploaded = false;
					this.textures[id] = object;
				}
				this.gl.bindTexture(this.gl.TEXTURE_2D, object.texture);
				this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MIN_FILTER, this.gl.NEAREST);
				this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_MAG_FILTER, this.gl.NEAREST);
				this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_S, this.gl.CLAMP_TO_EDGE);
				this.gl.texParameteri(this.gl.TEXTURE_2D, this.gl.TEXTURE_WRAP_T, this.gl.CLAMP_TO_EDGE);

				var sizeChanged = object.width !== width || object.height !== height || object.renderTarget !== renderTarget;
				object.width = width;
				object.height = height;
				object.renderTarget = renderTarget;

				if (renderTarget) {
					if (sizeChanged || !object.uploaded) {
						this.gl.texImage2D(this.gl.TEXTURE_2D, 0, this.gl.RGBA, width, height, 0, this.gl.RGBA, this.gl.UNSIGNED_BYTE, null);
						object.uploaded = true;
					}
					if (!object.framebuffer) object.framebuffer = this.gl.createFramebuffer();
					this.gl.bindFramebuffer(this.gl.FRAMEBUFFER, object.framebuffer);
					this.gl.framebufferTexture2D(this.gl.FRAMEBUFFER, this.gl.COLOR_ATTACHMENT0, this.gl.TEXTURE_2D, object.texture, 0);
					this.gl.bindFramebuffer(this.gl.FRAMEBUFFER, null);
				} else if (dirty || sizeChanged || !object.uploaded) {
					var count = width * height;
					var source = HEAPU32.subarray(ptr >>> 2, (ptr >>> 2) + count);
					var rgba = new Uint8Array(count * 4);
					for (var i = 0, j = 0; i < count; ++i, j += 4) {
						var color = source[i] >>> 0;
						rgba[j] = (color >>> 16) & 255;
						rgba[j + 1] = (color >>> 8) & 255;
						rgba[j + 2] = color & 255;
						rgba[j + 3] = (color >>> 24) & 255;
					}
					this.gl.texImage2D(this.gl.TEXTURE_2D, 0, this.gl.RGBA, width, height, 0, this.gl.RGBA, this.gl.UNSIGNED_BYTE, rgba);
					object.uploaded = true;
				}
				return id;
			};
		api.setTarget = function(targetId, width, height) {
				var target = targetId ? this.textures[targetId | 0] : null;
				if (target && target.framebuffer) {
					this.gl.bindFramebuffer(this.gl.FRAMEBUFFER, target.framebuffer);
				} else {
					var canvas = Module['canvas'] || document.getElementById('canvas');
					if (canvas.width !== width) canvas.width = width;
					if (canvas.height !== height) canvas.height = height;
					this.gl.bindFramebuffer(this.gl.FRAMEBUFFER, null);
				}
				this.gl.viewport(0, 0, width, height);
			};
		api.setBlend = function(enabled, factors, operation) {
				if (!enabled) {
					this.gl.disable(this.gl.BLEND);
					return;
				}
				this.gl.enable(this.gl.BLEND);
				this.gl.blendEquation(operation === 1 ? this.gl.FUNC_REVERSE_SUBTRACT : this.gl.FUNC_ADD);
				if (factors === 0) {
					this.gl.blendFunc(this.gl.SRC_ALPHA, this.gl.ONE);
				} else if (factors === 2) {
					this.gl.blendFunc(this.gl.ZERO, this.gl.ONE_MINUS_SRC_COLOR);
				} else if (factors === 3) {
					this.gl.blendFunc(this.gl.ONE, this.gl.ONE_MINUS_SRC_ALPHA);
				} else {
					this.gl.blendFunc(this.gl.SRC_ALPHA, this.gl.ONE_MINUS_SRC_ALPHA);
				}
			};
		api.clear = function(targetId, width, height, color) {
				this.used = true;
				this.setTarget(targetId, width, height);
				this.gl.disable(this.gl.BLEND);
				this.gl.clearColor(((color >>> 16) & 255) / 255, ((color >>> 8) & 255) / 255, (color & 255) / 255, ((color >>> 24) & 255) / 255);
				this.gl.clear(this.gl.COLOR_BUFFER_BIT);
				return 1;
			};
		api.drawStrip = function(targetId, targetWidth, targetHeight, sourceId, vertexPtr, primitiveCount, stride, colored, colorMode, alphaBlend, blendFactors, blendOperation, scaleX, scaleY) {
				this.used = true;
				var vertexCount = (primitiveCount | 0) + 2;
				if (vertexCount < 3) return 1;
				this.setTarget(targetId, targetWidth, targetHeight);
				this.setBlend(!!alphaBlend, blendFactors | 0, blendOperation | 0);
				this.gl.useProgram(this.program);

				var data = new Float32Array(vertexCount * 8);
				for (var i = 0; i < vertexCount; ++i) {
					var byteOffset = (vertexPtr >>> 0) + i * (stride | 0);
					var floatOffset = byteOffset >>> 2;
					var x = HEAPF32[floatOffset];
					var y = HEAPF32[floatOffset + 1];
					var tu;
					var tv;
					var color = 0xffffffff;
					if (colored) {
						color = HEAPU32[(byteOffset + 16) >>> 2] >>> 0;
						tu = HEAPF32[(byteOffset + 20) >>> 2];
						tv = HEAPF32[(byteOffset + 24) >>> 2];
					} else {
						tu = HEAPF32[(byteOffset + 16) >>> 2];
						tv = HEAPF32[(byteOffset + 20) >>> 2];
					}
					if (!targetId) {
						x *= scaleX;
						y *= scaleY;
					}
					var out = i * 8;
					data[out] = x;
					data[out + 1] = y;
					data[out + 2] = tu;
					data[out + 3] = tv;
					data[out + 4] = ((color >>> 16) & 255) / 255;
					data[out + 5] = ((color >>> 8) & 255) / 255;
					data[out + 6] = (color & 255) / 255;
					data[out + 7] = ((color >>> 24) & 255) / 255;
				}

				this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.buffer);
				this.gl.bufferData(this.gl.ARRAY_BUFFER, data, this.gl.STREAM_DRAW);
				this.gl.enableVertexAttribArray(this.aPos);
				this.gl.enableVertexAttribArray(this.aUv);
				this.gl.enableVertexAttribArray(this.aColor);
				this.gl.vertexAttribPointer(this.aPos, 2, this.gl.FLOAT, false, 32, 0);
				this.gl.vertexAttribPointer(this.aUv, 2, this.gl.FLOAT, false, 32, 8);
				this.gl.vertexAttribPointer(this.aColor, 4, this.gl.FLOAT, false, 32, 16);
				this.gl.uniform2f(this.uResolution, targetWidth, targetHeight);
				this.gl.uniform1i(this.uColorMode, colorMode | 0);
				this.gl.activeTexture(this.gl.TEXTURE0);
				if (sourceId) {
					var source = this.textures[sourceId | 0];
					this.gl.bindTexture(this.gl.TEXTURE_2D, source ? source.texture : null);
					this.gl.uniform1i(this.uHasTexture, source ? 1 : 0);
				} else {
					this.gl.bindTexture(this.gl.TEXTURE_2D, null);
					this.gl.uniform1i(this.uHasTexture, 0);
				}
				this.gl.uniform1i(this.uTexture, 0);
				this.gl.drawArrays(this.gl.TRIANGLE_STRIP, 0, vertexCount);
				return 1;
			};
		Module['ggnWebGLApi'] = api;
		return 1;
	}) != 0;
#else
	return false;
#endif
}

inline int cRenderEnsureWebGLTexture(cRenderTexture* texture, bool renderTarget)
{
	if(texture == NULL) return 0;
	if(renderTarget) texture->webglIsRenderTarget = true;
	if(!cRenderWebGLEnsure()) return 0;
#ifdef __EMSCRIPTEN__
	int textureId = EM_ASM_INT({
		var api = Module['ggnWebGLApi'];
		if (!api || !api.ok) return 0;
		return api.ensureTexture($0 | 0, $1 | 0, $2 | 0, $3 >>> 0, $4 | 0, $5 | 0) | 0;
	}, texture->webglTextureId, texture->width, texture->height, texture->pixels, texture->webglIsRenderTarget ? 1 : 0, texture->webglDirty ? 1 : 0);
	if(textureId != 0)
	{
		texture->webglTextureId = textureId;
		texture->webglDirty = false;
	}
	return textureId;
#else
	return 0;
#endif
}

inline bool cRenderClearTargetWebGL(cRenderDevice* device, cRenderTexture* target, cRenderColor color)
{
	if(device == NULL || target == NULL) return false;
	if(!cRenderWebGLEnsure()) return false;
	int targetId = 0;
	if(!device->isBackBufferTarget(target))
	{
		targetId = cRenderEnsureWebGLTexture(target, true);
		if(targetId == 0) return false;
	}
#ifdef __EMSCRIPTEN__
	return EM_ASM_INT({
		var api = Module['ggnWebGLApi'];
		if (!api || !api.ok) return 0;
		return api.clear($0 | 0, $1 | 0, $2 | 0, $3 >>> 0) | 0;
	}, targetId, target->width, target->height, color) != 0;
#else
	return false;
#endif
}

inline void cRenderAccumulateWebGLStats(cRenderDevice* device, UINT primitiveCount, const void* vertices, UINT stride, cRenderTexture* target)
{
	if(device == NULL || vertices == NULL || target == NULL) return;
	if(primitiveCount != 2)
	{
		device->statsTriangleCount += (int)primitiveCount;
		return;
	}

	const unsigned char* bytes = static_cast<const unsigned char*>(vertices);
	float minX = 1000000.0f;
	float minY = 1000000.0f;
	float maxX = -1000000.0f;
	float maxY = -1000000.0f;
	for(int i = 0; i < 4; ++i)
	{
		const float* vertex = reinterpret_cast<const float*>(bytes + i * stride);
		float x = vertex[0];
		float y = vertex[1];
		if(device->isBackBufferTarget(target))
		{
			x *= device->backBufferScaleX;
			y *= device->backBufferScaleY;
		}
		if(x < minX) minX = x;
		if(y < minY) minY = y;
		if(x > maxX) maxX = x;
		if(y > maxY) maxY = y;
	}
	int left = cRenderClampInt((int)floorf(minX), 0, target->width - 1);
	int right = cRenderClampInt((int)ceilf(maxX), 0, target->width - 1);
	int top = cRenderClampInt((int)floorf(minY), 0, target->height - 1);
	int bottom = cRenderClampInt((int)ceilf(maxY), 0, target->height - 1);
	device->statsFastQuadCount++;
	device->statsFastQuadPixels += (double)(right - left + 1) * (double)(bottom - top + 1);
}

inline bool cRenderDrawWebGLTriangleStrip(cRenderDevice* device, UINT primitiveCount, const void* vertices, UINT stride, bool colored)
{
	if(device == NULL || vertices == NULL || primitiveCount == 0) return false;
	if(!cRenderWebGLEnsure()) return false;
	cRenderTexture* target = device->targetTexture();
	if(target == NULL) return false;

	int targetId = 0;
	if(!device->isBackBufferTarget(target))
	{
		targetId = cRenderEnsureWebGLTexture(target, true);
		if(targetId == 0) return false;
	}

	int sourceId = 0;
	if(device->currentTexture != NULL)
	{
		sourceId = cRenderEnsureWebGLTexture(device->currentTexture, device->currentTexture->webglIsRenderTarget);
		if(sourceId == 0) return false;
		if(targetId != 0 && sourceId == targetId) return false;
	}

#ifdef __EMSCRIPTEN__
	int drawn = EM_ASM_INT({
		var api = Module['ggnWebGLApi'];
		if (!api || !api.ok) return 0;
		return api.drawStrip($0 | 0, $1 | 0, $2 | 0, $3 | 0, $4 >>> 0, $5 | 0, $6 | 0, $7 | 0, $8 | 0, $9 | 0, $10 | 0, $11 | 0, +$12, +$13) | 0;
	}, targetId, target->width, target->height, sourceId, vertices, primitiveCount, stride, colored ? 1 : 0, device->colorMode, device->alphaBlendEnabled ? 1 : 0, device->blendFactors, device->blendOperation, device->backBufferScaleX, device->backBufferScaleY);
	if(drawn != 0)
	{
		cRenderAccumulateWebGLStats(device, primitiveCount, vertices, stride, target);
		return true;
	}
#endif
	return false;
}

inline bool cRenderPresentWebGL(cRenderDevice* device)
{
	if(device == NULL) return false;
	if(!cRenderWebGLEnsure()) return false;
#ifdef __EMSCRIPTEN__
	return EM_ASM_INT({
		var api = Module['ggnWebGLApi'];
		if (!api || !api.ok) return 0;
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
			renderInfo = {};
			Module['ggnRenderInfo'] = renderInfo;
		}
		renderInfo['width'] = $0 | 0;
		renderInfo['height'] = $1 | 0;
		renderInfo['logicalWidth'] = $2 | 0;
		renderInfo['logicalHeight'] = $3 | 0;
		renderInfo['scaleX'] = +$4;
		renderInfo['scaleY'] = +$5;
		renderInfo['presentFps'] = Module['ggnPresentFps'] || 0;
		renderInfo['fastQuadCount'] = $6 | 0;
		renderInfo['triangleCount'] = $7 | 0;
		renderInfo['fastQuadPixels'] = +$8;
		renderInfo['trianglePixels'] = +$9;
		renderInfo['clearMillis'] = +$10;
		renderInfo['drawMillis'] = +$11;
		renderInfo['presentCopyMillis'] = 0;
		renderInfo['presentPutMillis'] = 0;
		renderInfo['presentMillis'] = 0;
		renderInfo['profileEnabled'] = ($12 | 0) != 0;
		renderInfo['webglEnabled'] = true;
		return 1;
	}, device->backBuffer.width, device->backBuffer.height, device->logicalWidth, device->logicalHeight, device->backBufferScaleX, device->backBufferScaleY, device->statsFastQuadCount, device->statsTriangleCount, device->statsFastQuadPixels, device->statsTrianglePixels, device->statsClearMillis, device->statsDrawMillis, device->profileEnabled ? 1 : 0) != 0;
#else
	return false;
#endif
}

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

inline unsigned long cRenderBlendColorNormalFast(unsigned long destination, unsigned long source)
{
	int srcA = cRenderColorA(source);
	if(srcA >= 255) return source;
	if(srcA <= 0) return destination;

	int invA = 255 - srcA;
	int dstA = cRenderColorA(destination);
	int outA = srcA + dstA * invA / 255;
	int outR = (cRenderColorR(source) * srcA + cRenderColorR(destination) * invA) / 255;
	int outG = (cRenderColorG(source) * srcA + cRenderColorG(destination) * invA) / 255;
	int outB = (cRenderColorB(source) * srcA + cRenderColorB(destination) * invA) / 255;
	return ((unsigned long)outA << 24) | ((unsigned long)outR << 16) | ((unsigned long)outG << 8) | (unsigned long)outB;
}

inline unsigned long cRenderSampleTextureFast(cRenderTexture* texture, float u, float v)
{
	int x = (int)floorf(u * (float)texture->width - 0.5f);
	int y = (int)floorf(v * (float)texture->height - 0.5f);
	x = cRenderClampInt(x, 0, texture->width - 1);
	y = cRenderClampInt(y, 0, texture->height - 1);
	return texture->pixels[y * texture->width + x];
}

inline int cRenderTextureCoordToFixed(float coord, int textureSize)
{
	return (int)floorf((coord * (float)textureSize - 0.5f) * 65536.0f);
}

inline int cRenderTextureStepToFixed(float step, int textureSize)
{
	return (int)(step * (float)textureSize * 65536.0f);
}

inline int cRenderFixedToTexel(int fixedCoord, int textureSize)
{
	int texel = fixedCoord >> 16;
	return cRenderClampInt(texel, 0, textureSize - 1);
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
	device->statsTriangleCount++;
	device->statsTrianglePixels += (double)(maxX - minX + 1) * (double)(maxY - minY + 1);

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

inline void cRenderScaleBackBufferVertex(cRenderDevice* device, cRenderTexture* target, cRenderDrawVertex& vertex)
{
	if(device != NULL && device->isBackBufferTarget(target))
	{
		vertex.x *= device->backBufferScaleX;
		vertex.y *= device->backBufferScaleY;
	}
}

inline bool cRenderNearlyEqual(float a, float b)
{
	return fabsf(a - b) < 0.001f;
}

inline bool cRenderDrawSoftwareAxisAlignedQuad(cRenderDevice* device, const cRenderDrawVertex& q0, const cRenderDrawVertex& q1, const cRenderDrawVertex& q2, const cRenderDrawVertex& q3)
{
	if(device == NULL) return true;
	cRenderTexture* target = device->targetTexture();
	if(target == NULL || target->pixels == NULL) return true;
	if(q0.color != q1.color || q0.color != q2.color || q0.color != q3.color) return false;

	cRenderDrawVertex v0 = q0;
	cRenderDrawVertex v1 = q1;
	cRenderDrawVertex v2 = q2;
	cRenderDrawVertex v3 = q3;
	cRenderScaleBackBufferVertex(device, target, v0);
	cRenderScaleBackBufferVertex(device, target, v1);
	cRenderScaleBackBufferVertex(device, target, v2);
	cRenderScaleBackBufferVertex(device, target, v3);

	if(!cRenderNearlyEqual(v0.y, v1.y) || !cRenderNearlyEqual(v2.y, v3.y) ||
		!cRenderNearlyEqual(v0.x, v2.x) || !cRenderNearlyEqual(v1.x, v3.x))
	{
		return false;
	}

	float left = v0.x;
	float right = v1.x;
	float top = v0.y;
	float bottom = v2.y;
	float width = right - left;
	float height = bottom - top;
	if(cRenderNearlyEqual(width, 0.0f) || cRenderNearlyEqual(height, 0.0f)) return true;

	int minX = cRenderClampInt((int)floorf(cRenderMin3(v0.x, v1.x, v2.x)), 0, target->width - 1);
	int maxX = cRenderClampInt((int)ceilf(cRenderMax3(v1.x, v2.x, v3.x)), 0, target->width - 1);
	int minY = cRenderClampInt((int)floorf(cRenderMin3(v0.y, v1.y, v2.y)), 0, target->height - 1);
	int maxY = cRenderClampInt((int)ceilf(cRenderMax3(v1.y, v2.y, v3.y)), 0, target->height - 1);
	device->statsFastQuadCount++;
	device->statsFastQuadPixels += (double)(maxX - minX + 1) * (double)(maxY - minY + 1);
	unsigned long vertexColor = v0.color;
	cRenderTexture* texture = device->currentTexture;
	bool canSampleFast = texture != NULL && texture->pixels != NULL && texture->width > 0 && texture->height > 0;
	bool textureOnly = vertexColor == 0xffffffff && device->colorMode == C_RENDER_TEXTURE_COLOR_MODULATE;
	bool noBlend = !device->alphaBlendEnabled;
	bool normalBlend = device->alphaBlendEnabled &&
		device->blendOperation == C_RENDER_BLEND_OPERATION_ADD &&
		device->blendFactors == C_RENDER_BLEND_FACTORS_NORMAL;

	if(canSampleFast && textureOnly && (noBlend || normalBlend) && width > 0.0f && height > 0.0f)
	{
		int startX = cRenderClampInt((int)ceilf(left - 0.5f), 0, target->width - 1);
		int endX = cRenderClampInt((int)floorf(right - 0.5f), 0, target->width - 1);
		int startY = cRenderClampInt((int)ceilf(top - 0.5f), 0, target->height - 1);
		int endY = cRenderClampInt((int)floorf(bottom - 0.5f), 0, target->height - 1);
		if(startX > endX || startY > endY) return true;

		float invWidth = 1.0f / width;
		float invHeight = 1.0f / height;
		const int textureWidth = texture->width;
		const int textureHeight = texture->height;
		const unsigned long* texturePixels = texture->pixels;

		for(int y = startY; y <= endY; ++y)
		{
			float ty = (((float)y + 0.5f) - top) * invHeight;
			float rowLeftU = v0.tu + (v2.tu - v0.tu) * ty;
			float rowRightU = v1.tu + (v3.tu - v1.tu) * ty;
			float rowLeftV = v0.tv + (v2.tv - v0.tv) * ty;
			float rowRightV = v1.tv + (v3.tv - v1.tv) * ty;
			float tx = (((float)startX + 0.5f) - left) * invWidth;
			float u = rowLeftU + (rowRightU - rowLeftU) * tx;
			float v = rowLeftV + (rowRightV - rowLeftV) * tx;
			float uStep = (rowRightU - rowLeftU) * invWidth;
			float vStep = (rowRightV - rowLeftV) * invWidth;
			int fixedU = cRenderTextureCoordToFixed(u, textureWidth);
			int fixedV = cRenderTextureCoordToFixed(v, textureHeight);
			int fixedUStep = cRenderTextureStepToFixed(uStep, textureWidth);
			int fixedVStep = cRenderTextureStepToFixed(vStep, textureHeight);
			unsigned long* destination = target->pixels + y * target->width + startX;

			if(noBlend)
			{
				for(int x = startX; x <= endX; ++x)
				{
					int sourceX = cRenderFixedToTexel(fixedU, textureWidth);
					int sourceY = cRenderFixedToTexel(fixedV, textureHeight);
					*destination++ = texturePixels[sourceY * textureWidth + sourceX];
					fixedU += fixedUStep;
					fixedV += fixedVStep;
				}
			}
			else
			{
				for(int x = startX; x <= endX; ++x)
				{
					int sourceX = cRenderFixedToTexel(fixedU, textureWidth);
					int sourceY = cRenderFixedToTexel(fixedV, textureHeight);
					unsigned long source = texturePixels[sourceY * textureWidth + sourceX];
					*destination = cRenderBlendColorNormalFast(*destination, source);
					++destination;
					fixedU += fixedUStep;
					fixedV += fixedVStep;
				}
			}
		}

		return true;
	}

	for(int y = minY; y <= maxY; ++y)
	{
		float ty = (((float)y + 0.5f) - top) / height;
		if(ty < -0.0001f || ty > 1.0001f) continue;
		float rowLeftU = v0.tu + (v2.tu - v0.tu) * ty;
		float rowRightU = v1.tu + (v3.tu - v1.tu) * ty;
		float rowLeftV = v0.tv + (v2.tv - v0.tv) * ty;
		float rowRightV = v1.tv + (v3.tv - v1.tv) * ty;
		unsigned long* destinationRow = target->pixels + y * target->width;

		for(int x = minX; x <= maxX; ++x)
		{
			float tx = (((float)x + 0.5f) - left) / width;
			if(tx < -0.0001f || tx > 1.0001f) continue;
			float u = rowLeftU + (rowRightU - rowLeftU) * tx;
			float v = rowLeftV + (rowRightV - rowLeftV) * tx;
			unsigned long textureColor = canSampleFast ? cRenderSampleTextureFast(texture, u, v) : cRenderSampleTexture(texture, u, v);
			if(textureOnly)
			{
				if(noBlend)
				{
					destinationRow[x] = textureColor;
				}
				else if(normalBlend)
				{
					destinationRow[x] = cRenderBlendColorNormalFast(destinationRow[x], textureColor);
				}
				else
				{
					destinationRow[x] = cRenderBlendColor(device, destinationRow[x], textureColor);
				}
			}
			else
			{
				unsigned long source = cRenderApplyTextureColor(device, textureColor, vertexColor);
				destinationRow[x] = cRenderBlendColor(device, destinationRow[x], source);
			}
		}
	}

	return true;
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
	bool created = cRenderCreateManagedTexture(device, (DWORD)width, (DWORD)height, texture);
	if(created && texture != NULL && *texture != NULL)
	{
		(*texture)->webglIsRenderTarget = true;
		(*texture)->webglDirty = false;
	}
	return created;
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

	texture->webglDirty = true;
	rect.pBits = texture->pixels;
	rect.Pitch = texture->width * sizeof(unsigned long);
	return true;
}

inline void cRenderUnlockTexture(cRenderTexture* texture)
{
	if(texture != NULL) texture->webglDirty = true;
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
	if(cRenderClearTargetWebGL(device, target, color)) return;

	for(int i = 0; i < target->width * target->height; ++i)
	{
		target->pixels[i] = color;
	}
}

inline cRenderResult cRenderPresent(cRenderDevice* device)
{
	if(device == NULL || device->backBuffer.pixels == NULL) return 1;
#ifdef __EMSCRIPTEN__
	if(cRenderPresentWebGL(device))
	{
		device->resetStats();
		return 0;
	}
	EM_ASM({
		var presentStart = performance.now();
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
		var afterCopy = performance.now();
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
		renderInfo['fastQuadCount'] = $7 | 0;
		renderInfo['triangleCount'] = $8 | 0;
		renderInfo['fastQuadPixels'] = +$9;
		renderInfo['trianglePixels'] = +$10;
		renderInfo['clearMillis'] = +$11;
		renderInfo['drawMillis'] = +$12;
		renderInfo['presentCopyMillis'] = afterCopy - presentStart;
		renderInfo['presentPutMillis'] = now - afterCopy;
		renderInfo['presentMillis'] = now - presentStart;
		renderInfo['profileEnabled'] = ($13 | 0) != 0;
	}, device->backBuffer.pixels, device->backBuffer.width, device->backBuffer.height, device->logicalWidth, device->logicalHeight, device->backBufferScaleX, device->backBufferScaleY, device->statsFastQuadCount, device->statsTriangleCount, device->statsFastQuadPixels, device->statsTrianglePixels, device->statsClearMillis, device->statsDrawMillis, device->profileEnabled ? 1 : 0);
	device->resetStats();
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
	if(cRenderDrawWebGLTriangleStrip(device, primitiveCount, vertices, sizeof(VERTEX2D), false)) return;
	if(primitiveCount == 2)
	{
		cRenderDrawVertex q0 = { vertices[0].x, vertices[0].y, vertices[0].tu, vertices[0].tv, 0xffffffff };
		cRenderDrawVertex q1 = { vertices[1].x, vertices[1].y, vertices[1].tu, vertices[1].tv, 0xffffffff };
		cRenderDrawVertex q2 = { vertices[2].x, vertices[2].y, vertices[2].tu, vertices[2].tv, 0xffffffff };
		cRenderDrawVertex q3 = { vertices[3].x, vertices[3].y, vertices[3].tu, vertices[3].tv, 0xffffffff };
		if(cRenderDrawSoftwareAxisAlignedQuad(device, q0, q1, q2, q3)) return;
	}
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
	if(cRenderDrawWebGLTriangleStrip(device, primitiveCount, vertices, sizeof(VERTEX2D_COLORED), true)) return;
	if(primitiveCount == 2)
	{
		cRenderDrawVertex q0 = { vertices[0].x, vertices[0].y, vertices[0].tu, vertices[0].tv, vertices[0].col };
		cRenderDrawVertex q1 = { vertices[1].x, vertices[1].y, vertices[1].tu, vertices[1].tv, vertices[1].col };
		cRenderDrawVertex q2 = { vertices[2].x, vertices[2].y, vertices[2].tu, vertices[2].tv, vertices[2].col };
		cRenderDrawVertex q3 = { vertices[3].x, vertices[3].y, vertices[3].tu, vertices[3].tv, vertices[3].col };
		if(cRenderDrawSoftwareAxisAlignedQuad(device, q0, q1, q2, q3)) return;
	}
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
		double startMillis = profileEnabled ? cRenderNowMillis() : 0;
		cRenderClearTarget(this, color);
		if(profileEnabled) statsClearMillis += cRenderNowMillis() - startMillis;
	}
	return 0;
}

inline int cRenderDevice::DrawPrimitiveUP(DWORD primitiveType, UINT primitiveCount, const void* vertices, UINT stride)
{
	if(primitiveType != D3DPT_TRIANGLESTRIP || vertices == NULL) return 0;
	double startMillis = profileEnabled ? cRenderNowMillis() : 0;
	if(stride == sizeof(VERTEX2D_COLORED))
	{
		cRenderDrawColoredTriangleStrip(this, primitiveCount, (const VERTEX2D_COLORED*)vertices);
	}
	else if(stride == sizeof(VERTEX2D))
	{
		cRenderDrawTriangleStrip(this, primitiveCount, (const VERTEX2D*)vertices);
	}
	if(profileEnabled) statsDrawMillis += cRenderNowMillis() - startMillis;
	return 0;
}
