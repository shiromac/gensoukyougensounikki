#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include "../utility/sVertexes.h"

typedef IDirect3D9 cRenderInterface;
typedef IDirect3DDevice9 cRenderDevice;
typedef IDirect3DTexture9 cRenderTexture;
typedef IDirect3DSurface9 cRenderSurface;
typedef D3DCAPS9 cRenderCaps;
typedef D3DDISPLAYMODE cRenderDisplayMode;
typedef D3DPRESENT_PARAMETERS cRenderPresentParameters;
typedef D3DXVECTOR2 cRenderVector2;
typedef D3DXVECTOR3 cRenderVector3;
typedef D3DXVECTOR4 cRenderVector4;
typedef D3DXMATRIX cRenderMatrix;
typedef D3DXQUATERNION cRenderQuaternion;
typedef D3DLOCKED_RECT cRenderLockedRect;
typedef HRESULT cRenderResult;
typedef D3DFORMAT cRenderTextureFormat;

static const cRenderTextureFormat C_RENDER_TEXTURE_FORMAT_A8R8G8B8 = D3DFMT_A8R8G8B8;

struct cRenderTextContext
{
	cRenderTextContext() : hwnd(NULL), hdc(NULL), font(NULL), oldFont(NULL), size(0)
	{
	}

	HWND hwnd;
	HDC hdc;
	HFONT font;
	HFONT oldFont;
	DWORD size;
};

struct cRenderTextTransform
{
	MAT2 mat;
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
	if(context.hdc != NULL)
	{
		if(context.font != NULL)
		{
			DeleteObject(SelectObject(context.hdc, context.oldFont));
		}
		ReleaseDC(context.hwnd, context.hdc);
	}

	context.hwnd = NULL;
	context.hdc = NULL;
	context.font = NULL;
	context.oldFont = NULL;
	context.size = 0;
}

inline bool cRenderBeginText(cRenderTextContext& context, const TCHAR* face, DWORD size, int weight)
{
	cRenderEndText(context);
	context.hwnd = GetActiveWindow();
	context.hdc = GetDC(context.hwnd);
	context.size = size;

	context.font = CreateFont(
		(int)size,
		0,
		0,
		0,
		weight,
		FALSE,
		FALSE,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		face);

	if(context.font == NULL)
	{
		ReleaseDC(context.hwnd, context.hdc);
		context.hwnd = NULL;
		context.hdc = NULL;
		context.size = 0;
		return false;
	}

	context.oldFont = (HFONT)SelectObject(context.hdc, context.font);
	return true;
}

inline void cRenderBuildTextTransform(cRenderTextTransform& transform, float scaleX, float scaleY, float rotation)
{
	float fCos = (float)cos(rotation);
	float fSin = (float)sin(rotation);

	long m11 = (long)(scaleX * fCos * 65536.0f);
	long m12 = (long)(scaleY * fSin * 65536.0f);
	long m21 = (long)(-scaleX * fSin * 65536.0f);
	long m22 = (long)(scaleY * fCos * 65536.0f);

	transform.mat.eM11 = *((FIXED *)&m11);
	transform.mat.eM12 = *((FIXED *)&m12);
	transform.mat.eM21 = *((FIXED *)&m21);
	transform.mat.eM22 = *((FIXED *)&m22);
}

inline bool cRenderGetTextMetrics(cRenderTextContext& context, cRenderTextMetrics& metrics)
{
	TEXTMETRIC nativeMetrics;
	if(!GetTextMetrics(context.hdc, &nativeMetrics)) return false;

	metrics.ascent = nativeMetrics.tmAscent;
	metrics.height = nativeMetrics.tmHeight;
	metrics.externalLeading = nativeMetrics.tmExternalLeading;
	return true;
}

inline bool cRenderGetGlyphABC(cRenderTextContext& context, unsigned int code, cRenderGlyphABC& abc)
{
	ABC nativeAbc;
	if(!GetCharABCWidths(context.hdc, code, code, &nativeAbc)) return false;

	abc.a = nativeAbc.abcA;
	abc.b = nativeAbc.abcB;
	abc.c = nativeAbc.abcC;
	return true;
}

inline DWORD cRenderGetGlyphBitmap(cRenderTextContext& context, unsigned int code, bool antialias, const cRenderTextTransform& transform, cRenderGlyphMetrics& metrics, BYTE** data)
{
	GLYPHMETRICS nativeMetrics;
	ZeroMemory(&nativeMetrics, sizeof(nativeMetrics));
	*data = NULL;

	int bitmapMode = antialias ? GGO_GRAY8_BITMAP : GGO_BITMAP;
	DWORD size = GetGlyphOutline(context.hdc, code, bitmapMode, &nativeMetrics, 0, NULL, &transform.mat);
	if(size == GDI_ERROR)
	{
		return size;
	}

	*data = new BYTE[size];
	if(size != 0 && *data == NULL)
	{
		return GDI_ERROR;
	}

	GetGlyphOutline(context.hdc, code, bitmapMode, &nativeMetrics, size, *data, &transform.mat);
	metrics.blackBoxX = nativeMetrics.gmBlackBoxX;
	metrics.blackBoxY = nativeMetrics.gmBlackBoxY;
	metrics.glyphOriginY = nativeMetrics.gmptGlyphOrigin.y;
	return size;
}

inline cRenderMatrix* cRenderMatrixRotationX(cRenderMatrix* out, float angle)
{
	return D3DXMatrixRotationX(out, angle);
}

inline cRenderMatrix* cRenderMatrixRotationY(cRenderMatrix* out, float angle)
{
	return D3DXMatrixRotationY(out, angle);
}

inline cRenderMatrix* cRenderMatrixRotationZ(cRenderMatrix* out, float angle)
{
	return D3DXMatrixRotationZ(out, angle);
}

inline cRenderVector3* cRenderVec3TransformCoord(cRenderVector3* out, const cRenderVector3* vector, const cRenderMatrix* matrix)
{
	return D3DXVec3TransformCoord(out, vector, matrix);
}

inline cRenderMatrix* cRenderMatrixLookAtLH(cRenderMatrix* out, const cRenderVector3* eye, const cRenderVector3* at, const cRenderVector3* up)
{
	return D3DXMatrixLookAtLH(out, eye, at, up);
}

inline cRenderMatrix* cRenderMatrixLookAtRH(cRenderMatrix* out, const cRenderVector3* eye, const cRenderVector3* at, const cRenderVector3* up)
{
	return D3DXMatrixLookAtRH(out, eye, at, up);
}

inline cRenderMatrix* cRenderMatrixRotationYawPitchRoll(cRenderMatrix* out, float yaw, float pitch, float roll)
{
	return D3DXMatrixRotationYawPitchRoll(out, yaw, pitch, roll);
}

inline cRenderQuaternion* cRenderQuaternionRotationAxis(cRenderQuaternion* out, const cRenderVector3* axis, float angle)
{
	return D3DXQuaternionRotationAxis(out, axis, angle);
}

inline cRenderMatrix* cRenderMatrixRotationQuaternion(cRenderMatrix* out, const cRenderQuaternion* quaternion)
{
	return D3DXMatrixRotationQuaternion(out, quaternion);
}

inline cRenderVector3* cRenderVec3Normalize(cRenderVector3* out, const cRenderVector3* vector)
{
	return D3DXVec3Normalize(out, vector);
}

inline float cRenderVec3Length(const cRenderVector3* vector)
{
	return D3DXVec3Length(vector);
}

inline cRenderQuaternion* cRenderQuaternionConjugate(cRenderQuaternion* out, const cRenderQuaternion* quaternion)
{
	return D3DXQuaternionConjugate(out, quaternion);
}

inline cRenderQuaternion* cRenderQuaternionMultiply(cRenderQuaternion* out, const cRenderQuaternion* a, const cRenderQuaternion* b)
{
	return D3DXQuaternionMultiply(out, a, b);
}

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

inline void cRenderSetViewport(cRenderDevice* device, DWORD width, DWORD height)
{
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	device->SetViewport(&viewport);
}

template<class T>
inline void cRenderRelease(T*& resource)
{
	if(resource != NULL)
	{
		resource->Release();
		resource = NULL;
	}
}

inline cRenderInterface* cRenderCreateInterface()
{
	return Direct3DCreate9(D3D_SDK_VERSION);
}

inline bool cRenderSucceeded(cRenderResult result)
{
	return SUCCEEDED(result);
}

inline bool cRenderFailed(cRenderResult result)
{
	return FAILED(result);
}

inline bool cRenderIsDriverInternalError(cRenderResult result)
{
	return result == D3DERR_DRIVERINTERNALERROR;
}

inline bool cRenderIsDeviceLost(cRenderResult result)
{
	return result == D3DERR_DEVICELOST;
}

inline bool cRenderIsDeviceNotReset(cRenderResult result)
{
	return result == D3DERR_DEVICENOTRESET;
}

inline cRenderColor cRenderColorARGB(BYTE alpha, BYTE red, BYTE green, BYTE blue)
{
	return D3DCOLOR_ARGB(alpha, red, green, blue);
}

inline cRenderColor cRenderColorRGB(BYTE red, BYTE green, BYTE blue)
{
	return D3DCOLOR_XRGB(red, green, blue);
}

inline cRenderResult cRenderGetAdapterDisplayMode(cRenderInterface* renderer, cRenderDisplayMode* displayMode)
{
	return renderer->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, displayMode);
}

inline cRenderResult cRenderGetDeviceCaps(cRenderInterface* renderer, cRenderCaps* caps)
{
	return renderer->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, caps);
}

inline void cRenderInitializePresentParameters(cRenderPresentParameters* params, HWND window, bool fullscreen, const cRenderDisplayMode& displayMode, DWORD width, DWORD height)
{
	ZeroMemory(params, sizeof(cRenderPresentParameters));
	params->BackBufferCount = 1;
	params->Flags = 0;
	params->SwapEffect = D3DSWAPEFFECT_DISCARD;
	params->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	params->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	params->MultiSampleType = D3DMULTISAMPLE_NONE;
	params->MultiSampleQuality = D3DMULTISAMPLE_NONE;
	params->hDeviceWindow = window;
	params->Windowed = !fullscreen;
	params->BackBufferWidth = width;
	params->BackBufferHeight = height;
	params->BackBufferFormat = displayMode.Format;
	params->EnableAutoDepthStencil = FALSE;
	params->AutoDepthStencilFormat = D3DFMT_D16;
}

inline cRenderResult cRenderCreateDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, D3DDEVTYPE deviceType, DWORD vertexProcessing, cRenderDevice** device)
{
	return renderer->CreateDevice(D3DADAPTER_DEFAULT, deviceType, window, vertexProcessing, params, device);
}

inline bool cRenderCreateReferenceSoftwareDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	return cRenderSucceeded(cRenderCreateDevice(renderer, window, params, D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING, device));
}

inline bool cRenderCreateHalSoftwareDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	return cRenderSucceeded(cRenderCreateDevice(renderer, window, params, D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, device));
}

inline bool cRenderCreateHalHardwareDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	return cRenderSucceeded(cRenderCreateDevice(renderer, window, params, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING, device));
}

inline bool cRenderCreateHalMixedDevice(cRenderInterface* renderer, HWND window, cRenderPresentParameters* params, cRenderDevice** device)
{
	return cRenderSucceeded(cRenderCreateDevice(renderer, window, params, D3DDEVTYPE_HAL, D3DCREATE_MIXED_VERTEXPROCESSING, device));
}

inline bool cRenderGetTextureSize(cRenderTexture* texture, int& width, int& height)
{
	if(texture == NULL) return false;

	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	width = desc.Width;
	height = desc.Height;
	return true;
}

inline bool cRenderCreateManagedTexture(cRenderDevice* device, DWORD width, DWORD height, cRenderTexture** texture)
{
	return cRenderSucceeded(D3DXCreateTexture(device, width, height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, texture));
}

inline bool cRenderLoadTextureFromFile(cRenderDevice* device, const void* filePath, cRenderTexture** texture)
{
	const TCHAR* path = static_cast<const TCHAR*>(filePath);
	return cRenderSucceeded(D3DXCreateTextureFromFileEx(
		device,
		path,
		0, 0, 0, 0,
		D3DFMT_A8B8G8R8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		0,
		NULL,
		NULL,
		texture));
}

inline bool cRenderCreateTextureFromMemory(cRenderDevice* device, const BYTE* data, UINT dataSize, cRenderTextureFormat format, DWORD colorkey, cRenderTexture** texture)
{
	if(device == NULL || data == NULL || dataSize == 0 || texture == NULL) return false;

	return cRenderSucceeded(D3DXCreateTextureFromFileInMemoryEx(
		device,
		data,
		dataSize,
		0, 0, 0, 0,
		format,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		colorkey,
		NULL,
		NULL,
		texture));
}
inline bool cRenderCreateRenderTargetTexture(cRenderDevice* device, int width, int height, cRenderTexture** texture)
{
	return cRenderSucceeded(device->CreateTexture(width, height, 0, D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, texture, NULL));
}

inline bool cRenderGetSurfaceFromTexture(cRenderTexture* texture, cRenderSurface** surface)
{
	if(texture == NULL) return false;

	return cRenderSucceeded(texture->GetSurfaceLevel(0, surface));
}

inline bool cRenderGetBackBuffer(cRenderDevice* device, cRenderSurface** surface)
{
	if(device == NULL) return false;

	return cRenderSucceeded(device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, surface));
}

inline bool cRenderLockTexture(cRenderTexture* texture, cRenderLockedRect& rect)
{
	if(texture == NULL) return false;

	return cRenderSucceeded(texture->LockRect(0, &rect, NULL, 0));
}

inline void cRenderUnlockTexture(cRenderTexture* texture)
{
	if(texture != NULL)
		texture->UnlockRect(0);
}

inline void cRenderSetRenderTarget(cRenderDevice* device, DWORD index, cRenderSurface* surface)
{
	device->SetRenderTarget(index, surface);
}

inline void cRenderClearTarget(cRenderDevice* device, cRenderColor color)
{
	device->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
}

inline cRenderResult cRenderPresent(cRenderDevice* device)
{
	return device->Present(NULL, NULL, NULL, NULL);
}

inline cRenderResult cRenderTestCooperativeLevel(cRenderDevice* device)
{
	return device->TestCooperativeLevel();
}

inline cRenderResult cRenderResetDevice(cRenderDevice* device, cRenderPresentParameters* params)
{
	return device->Reset(params);
}

inline bool cRenderBeginScene(cRenderDevice* device)
{
	return SUCCEEDED(device->BeginScene());
}

inline void cRenderEndScene(cRenderDevice* device)
{
	device->EndScene();
}

inline void cRenderSetCullNone(cRenderDevice* device)
{
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

inline void cRenderSetPointSampler(cRenderDevice* device, DWORD sampler)
{
	device->SetSamplerState(sampler, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	device->SetSamplerState(sampler, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	device->SetSamplerState(sampler, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	device->SetSamplerState(sampler, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	device->SetSamplerState(sampler, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
}

inline void cRenderSetTexture(cRenderDevice* device, DWORD stage, cRenderTexture* texture)
{
	device->SetTexture(stage, texture);
}

inline void cRenderSetTexturedVertexFormat(cRenderDevice* device)
{
	device->SetFVF(VERTEX2D::FVF);
}

inline void cRenderSetColoredVertexFormat(cRenderDevice* device)
{
	device->SetFVF(VERTEX2D_COLORED::FVF);
}

inline void cRenderSetTextureCoordIndex(cRenderDevice* device, DWORD stage, DWORD coordIndex)
{
	device->SetTextureStageState(stage, D3DTSS_TEXCOORDINDEX, coordIndex);
}

inline void cRenderSetTextureColorMode(cRenderDevice* device, DWORD stage, cRenderTextureColorMode mode)
{
	DWORD colorOp = D3DTOP_MODULATE;
	if(mode == C_RENDER_TEXTURE_COLOR_ADD)
		colorOp = D3DTOP_ADD;
	else if(mode == C_RENDER_TEXTURE_COLOR_FILL)
		colorOp = D3DTOP_SELECTARG2;

	device->SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(stage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(stage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(stage, D3DTSS_COLOROP, colorOp);
	device->SetTextureStageState(stage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(stage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
}

inline void cRenderSetTextureAlphaCurrent(cRenderDevice* device, DWORD stage)
{
	device->SetTextureStageState(stage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(stage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(stage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

inline void cRenderSetAlphaBlendEnabled(cRenderDevice* device, bool enabled)
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, enabled ? TRUE : FALSE);
}

inline void cRenderSetBlendFactors(cRenderDevice* device, cRenderBlendFactors factors)
{
	DWORD destBlend = D3DBLEND_INVSRCALPHA;
	DWORD srcBlend = D3DBLEND_SRCALPHA;

	if(factors == C_RENDER_BLEND_FACTORS_ADDITION)
	{
		destBlend = D3DBLEND_ONE;
		srcBlend = D3DBLEND_SRCALPHA;
	}
	else if(factors == C_RENDER_BLEND_FACTORS_ANTIADDITION)
	{
		destBlend = D3DBLEND_INVSRCCOLOR;
		srcBlend = D3DBLEND_ZERO;
	}
	else if(factors == C_RENDER_BLEND_FACTORS_HALFADDITION)
	{
		destBlend = D3DBLEND_INVSRCALPHA;
		srcBlend = D3DBLEND_ONE;
	}

	device->SetRenderState(D3DRS_DESTBLEND, destBlend);
	device->SetRenderState(D3DRS_SRCBLEND, srcBlend);
}

inline void cRenderSetBlendOperation(cRenderDevice* device, cRenderBlendOperation operation)
{
	DWORD blendOp = D3DBLENDOP_ADD;
	if(operation == C_RENDER_BLEND_OPERATION_REVERSE_SUBTRACT)
		blendOp = D3DBLENDOP_REVSUBTRACT;

	device->SetRenderState(D3DRS_BLENDOP, blendOp);
}

inline void cRenderDrawTriangleStrip(cRenderDevice* device, UINT primitiveCount, const VERTEX2D* vertices)
{
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, primitiveCount, vertices, sizeof(VERTEX2D));
}

inline void cRenderDrawColoredTriangleStrip(cRenderDevice* device, UINT primitiveCount, const VERTEX2D_COLORED* vertices)
{
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, primitiveCount, vertices, sizeof(VERTEX2D_COLORED));
}
