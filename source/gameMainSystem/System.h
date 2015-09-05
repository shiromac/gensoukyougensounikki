
//=============================================================================
/** @file
    @brief		ÉVÅ[Éìä÷åW
    @author		Noriyuki Lee
    @since		2003.10.24
*/
//=============================================================================

#ifndef ___SCENE_H___
#define ___SCENE_H___


//=============================================================================
// INCLUDE
//=============================================================================
#include <d3d9.h>
#include <d3dx9.h>

#include "Main.h"


//=============================================================================
// PROTOTYPE
//=============================================================================
bool SceneInitialize( IDirect3DDevice9 *pDev );
void SceneFinalize( void );
eBootMode SceneCheck( D3DCAPS9 *pCaps );
bool SceneRender( IDirect3DDevice9 *pDev, float fFrameTime );
bool SceneReset( IDirect3DDevice9 *pDev );
bool SceneRestore( IDirect3DDevice9 *pDev );


#endif // ___SCENE_H___

