
#pragma once

#include <functional>

#ifndef C_RENDER_COLOR_DEFINED
typedef unsigned long cRenderColor;
#define C_RENDER_COLOR_DEFINED
#endif
//=============================================================================
// STRUCT
//=============================================================================
struct VERTEX2D
{
	float x, y, z, w;
	float tu, tv;

	// ’¸“_‚e‚u‚e
#ifdef D3DFVF_XYZRHW
	enum { FVF = D3DFVF_XYZRHW | D3DFVF_TEX1 };
#endif
};

struct VERTEX2D_COLORED
{
    float x, y, z, w;
	cRenderColor col;
    float tu, tv;
 
    // ’¸“_‚e‚u‚e
#ifdef D3DFVF_XYZRHW
    enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
#endif
};

struct VERTEX2D_DOUBLETEX_COLORED
{
    float x, y, z, w;
	cRenderColor col;
    float tu, tv;
	float tu2, tv2;
    // ’¸“_‚e‚u‚e
#ifdef D3DFVF_XYZRHW
    enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2 };
#endif
};


template<class T>
struct ptr_less : std::binary_function<T*,T*,bool> {
  bool operator()(const T* x, const T* y) const {
    return *x < *y;
  }
};


#define SOUND_MUSIC_STAGE_INTRO 0
#define SOUND_MUSIC_STAGE_LOOP 1
#define SOUND_MUSIC_BOSS_INTRO 2
#define SOUND_MUSIC_BOSS_LOOP 3

#define SOUND_MUSIC_PRACTICE_LOOP 4



#define SOUND_EFFECT_SHOT 100
#define SOUND_EFFECT_SLASH_STRONG 101
#define SOUND_EFFECT_SLASH_WEEK 102
#define SOUND_EFFECT_THRUST 103
#define SOUND_EFFECT_GRAVITY 104
#define SOUND_EFFECT_ENEMYATTACK 105
#define SOUND_EFFECT_DAMAGE 106
#define SOUND_EFFECT_TIMEALERT 107
#define SOUND_EFFECT_LOCKON 108
#define SOUND_EFFECT_BONUS 109

