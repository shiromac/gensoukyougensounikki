#pragma once

#ifdef __EMSCRIPTEN__
#include "cRenderBackendBrowser.h"
#else
#include "cRenderBackendD3D9.h"
#endif
