
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
#include "Main.h"
#include "cRenderBackend.h"


//=============================================================================
// PROTOTYPE
//=============================================================================
bool SceneInitialize( cRenderDevice *pDev );
void SceneFinalize( void );
eBootMode SceneCheck( cRenderCaps *pCaps );
bool SceneRender( cRenderDevice *pDev, float fFrameTime );
bool SceneReset( cRenderDevice *pDev );
bool SceneRestore( cRenderDevice *pDev );


#endif // ___SCENE_H___

