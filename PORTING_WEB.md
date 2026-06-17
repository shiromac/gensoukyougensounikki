# Minimal Browser Port Plan

This document records the scope for a minimal browser port of
Gensoukyou Gensou Nikki. The goal is to preserve the existing game and move
only the platform-dependent layer needed to run it in a browser.

## Goal

- Keep the existing C++ game logic, data, scenarios, item rules, enemy logic,
  BGM selection, and intro/loop BGM model.
- Run the game in a browser through WebAssembly/Emscripten.
- Replace only the Windows/DirectX-specific platform layer required for the
  browser build.
- Avoid new gameplay features, new UI modes, rewritten rules, or redesigned
  assets.

## Non-goals

- Do not rewrite the game in JavaScript, TypeScript, Unity, Godot, Phaser, or
  another engine.
- Do not add new smartphone-only gameplay features during the initial port.
- Do not redesign menus, controls, effects, fonts, or screen layout unless a
  platform dependency makes a minimal compatibility change necessary.
- Do not change save data semantics unless required to map the save location to
  browser storage.

## Current Port Boundaries

The current Windows version has useful abstraction at the game-input, command-action, scene-lifecycle, control/window, drop/item, character, map/minimap, and dungeon-core levels:

- `source/gameMainSystem/cPlayerInput.h` exposes semantic player actions.
- `source/gameMainSystem/cInput.cpp` maps keyboard/pad/Wiimote state into
  `cPlayerInput`.
- `source/gameMainSystem/cSceneVirtual.h` and `cSceneManager.*` now express
  scene initialization, drawing, and processing in terms of `cRenderDevice`.
- `source/csTitle.*`, `source/csLoading.*`, `source/csHomeFirst.*`, and
  `source/csDungeonFirst.*` use the same scene device boundary.
- `source/cCommand.*` and the command classes in `ConfigCommands.*`,
  `menuCommand.*`, `OtherChara1.cpp`, `DungeonObjMakes.cpp`,
  `cCocktailWindow.cpp`, `ceaiShopOwner.*`, `cDropingFunc.*`,
  `cCommandDroping.*`, `cStateDroping.*`, `cMessageDroping.*`, and
  `cLuaFunctionCommand.*` now express command execution as `cRenderDevice`.
- `source/cControl.*`, `cControlLayer.*`, `cGameWindow.*`, `cSelectWindow.*`,
  `cStringWindow.*`, `cListWindow*`, `cMessageWindow.*`, `cMassageWindow.*`,
  `cNumSetWindow.*`, `cPadConfigWindow.*`, `cResultWindow.*`,
  `cHomeResultWindow.*`, and `cCocktailWindow.*` now use `cRenderDevice` for
  UI processing/drawing and `cRenderTexture` for their texture handles.
- `source/cGameObject.h`, `cDroping.*`, `cDropingFunc.*`,
  `cCommandDroping.*`, `cThing.*`, `cEquipment.*`, `cBook.*`, `cBullet.*`,
  `cBox.*`, `cDrink.*`, `cFood.*`, `cSpell.*`, `cTrap.*`, `cMoneyBag.*`,
  `cStair.*`, and `cCondition.*` now use `cRenderDevice` for their drop/item
  drawing, menu, action, and status rendering paths, with `cRenderTexture` for
  the exposed texture handles in that layer.
- `source/cCharacter.*`, `cMob.*`, `cBoss.*`, `cObjectChara.h`,
  `cOtherchara.h`, `cMap.*`, `cMiniMap.*`, `cDungeonSystem.*`,
  `DungeonDashProcess.cpp`, `cGameScreenInterface.*`,
  `cVectorOfControlLayer.*`, and `cParameterGage.*` now use `cRenderDevice`
  and `cRenderTexture` at their public rendering boundaries while preserving
  the existing dungeon flow and drawing behavior.
- `source/cAnimation*`, `AnimeChips.*`, `AnimeFunctions.cpp`, `ca*`,
  `SpellEffects.*`, `FireCommand.cpp`, `firecommand.h`,
  `source/utility/cNumField.*`, `cDrawingObject_Rod.*`,
  `cDrawingObject_belt.*`, `cDrawingObject_ring.*`, `cDatabaseOfGame.*`,
  `cSaveStore.*`, `EffectFunctions.cpp`, `Event1.cpp`, and `Box1.*` now use
  `cRenderDevice` and `cRenderTexture` for the remaining animation/effect,
  scripted command, save-init, and small drawing boundaries.

The following layers still need browser backends:

- Entry point and main loop:
  `source/gameMainSystem/Main.cpp`
- Rendering and render targets:
  `source/gameMainSystem/Main.cpp`,
  `source/gameMainSystem/System.cpp`,
  `source/cDrawingObject.cpp`,
  `source/gameMainSystem/cScreen.*`
- Texture/resource creation:
  `source/gameMainSystem/cResourseManage.*`,
  `source/TexSeBgmFilepack/CFilePack.h`
- Text rendering:
  `source/utility/SpriteText.*`
- Sound:
  `source/cSoundManager.*` now owns the game-facing boundary. The Windows path
  keeps DirectSound behind `#ifndef __EMSCRIPTEN__`, while the browser path uses
  a minimal WebAudio backend that preserves stable SE IDs and the existing
  intro/loop BGM model.
- Save persistence has a minimal browser bridge through `/save/` and IDBFS.
  It has been browser-verified for initial file creation and reload
  persistence.

## Minimal Implementation Order

1. Split platform entry/update/render from the Win32 `WinMain` path while
   keeping the current Windows build working. Done in
   `source/gameMainSystem/Main.cpp` by introducing `GameInitialize`,
   `GameSetupFrameTimer`, `GameFrame`, and `GameFinalize`, and by adding a
   minimal `__EMSCRIPTEN__` `main`/`BrowserMainLoop` path that calls the same
   lifecycle functions through `emscripten_set_main_loop`.
2. Add a browser-compatible input backend that feeds the existing
   `cPlayerInput` actions. The first pass keeps to the existing keyboard
   mapping only, so no smartphone-only controls or new UI are introduced.
   The current Windows input path reads raw platform state into `cInputState` and
   then applies it to `cPlayerInput`; the browser path now reads keydown/keyup
   state into the same structure without including DirectInput or Wiimote
   headers.
3. Introduce a renderer boundary around the existing sprite/drawing path. The
   first target is compatibility, not a new renderer design. The first pass is
   in place for `cDrawingObject.*`, `cScreen.*`, `cSceneVirtual.*`,
   `cSceneManager.*`, the title/loading/home/dungeon-first scene classes,
   `cCommand.*`, the main command action classes, the main control/window
   classes, the drop/item classes, the character/map/dungeon core drawing
   classes, and the animation/effect/scripted-command drawing classes through
   `cRenderBackend.h`.
4. Preserve the public `SpriteText` usage pattern and replace its backend with
   a browser-compatible text-to-texture implementation.
5. Split asset lookup from Direct3D texture creation so packed graphic/sound
   data can be loaded as bytes before decoding. Graphic pack loading now has a
   `CFilePackLoad::ReadFileData` byte path, and graphic asset roots go through
   `g_GraphicAssetPath`.
6. Replace DirectSound BGM/SE playback with a browser audio backend. The current
   WebAudio path is behind `cSoundManager` and preserves the existing `intro`
   then `loop` behavior without adding gameplay UI or input modes.
7. Map save files to browser persistent storage and keep the existing save data
   structure. Save filename construction goes through `g_SaveDataPath` in
   `source/cSaveStore.*`; the browser entry path mounts `/save/` with IDBFS
   before game initialization, and `cFileManager` requests a sync after
   successful saves.
8. Build with Emscripten and verify title, start-game flow, dungeon play, BGM,
   SE, text, save/load, and release asset loading.

## First Browser MVP Acceptance

- The browser build opens without adding gameplay features.
- Title screen displays correctly.
- Keyboard input can start the game.
- A dungeon can be entered and one turn can be played.
- Main graphics and window text are present.
- SE and BGM playback are present through the WebAudio backend behind
  `cSoundManager`.
- Intro/loop BGM selection state is preserved, and the loop WAV is preloaded
  while the intro WAV is playing.
- Save data writes to browser storage and can be loaded after refresh.

## Size Strategy

The current release package is large mainly because BGM is stored as
uncompressed WAV. For the browser build, BGM should be encoded separately from
the Windows package, while preserving the current file-level intro/loop split.
The initial browser port does not change music assignment or playback rules.
The current proof build still serves WAV files directly from `web-build/sound`;
compression is a size-optimization step, not a gameplay-porting requirement.

## Current Implementation Notes

- `WinMain` still owns Win32 window creation and the Windows message pump for
  the Windows build. For `__EMSCRIPTEN__`, `source/gameMainSystem/Main.cpp`
  now provides a minimal `main` that initializes the render/game lifecycle and
  schedules `GameFrame` through `emscripten_set_main_loop`.
- The game-specific lifecycle has been grouped into small functions inside
  `source/gameMainSystem/Main.cpp` and is now shared by the Windows and browser
  entry paths.
- `source/gameMainSystem/cInput.*` now separates current Windows raw input
  collection from semantic `cPlayerInput` updates through `cInputState`.
  For `__EMSCRIPTEN__`, `cinput.h` avoids the DirectInput/Wiimote headers,
  keeps a small `cInputState` buffer, and the browser path reads the existing
  keyboard mapping into that state without changing gameplay code. Gamepad and
  touch-specific controls are intentionally deferred for the first minimal pass.
- `source/gameMainSystem/cPlatformTime.h` now centralizes frame-time access.
  The Windows backend still uses `timeGetTime`, `Sleep`, and timer-period
  calls, while the Emscripten path uses `emscripten_get_now` and leaves sleep
  to the browser main loop.
- `source/cSaveStore.*` now centralizes save-root construction through
  `g_SaveDataDirectory` and `g_SaveDataPath`. The Windows path remains
  `_EXELOCATION + "savedata/"`; the browser path is `/save/`. `Main.cpp`
  mounts that path with IDBFS before initialization, and `cFileManager.cpp`
  requests `FS.syncfs(false, ...)` after successful save writes.
- `source/TexSeBgmFilepack/CFilePack.h` now exposes `ReadFileData`, separating
  pack byte lookup from texture creation. `GetTextureFile` now calls
  `cRenderCreateTextureFromMemory`, so Direct3D texture construction lives in
  the D3D9 render backend instead of the pack loader.
- `source/gameMainSystem/cResourseManage.*`, `cEnvironment.cpp`, and
  `csLoading.cpp` now use `g_GraphicAssetPath` for graphic asset paths. The
  Windows root remains `_EXELOCATION + "graphic\\"`; the browser root is
  prepared as `/assets/graphic/`.
- `source/gameMainSystem/cRenderBackend.h` is now a platform selector. The
  current Windows implementation lives in `cRenderBackendD3D9.h`, while
  `cRenderBackendBrowser.h` provides the browser-side implementation path. The
  browser file keeps the same render API shape and now owns a software
  backbuffer/render-target path plus Canvas transfer in `cRenderPresent`, still
  without adding gameplay or UI behavior.
- The D3D9 backend provides render aliases and helper calls for texture
  handles, vector/matrix/quaternion compatibility types, texture size lookup,
  managed texture creation, texture creation from packed in-memory bytes,
  texture lock/unlock, color creation, viewport setup, render targets, target
  clear, point sampling, vertex formats, alpha-blend enable, texture-stage
  color modes, blend factors, blend operations, and triangle-strip sprite
  drawing. The browser backend now has matching lightweight vector/matrix/
  quaternion types, the small vector operators needed by existing call sites,
  managed textures, render-target surfaces, clear, texture binding, state
  storage, triangle-strip software rasterization, basic alpha blending, and
  backbuffer upload to an HTML canvas.
- `source/gameMainSystem/cResourseManage.*` and
  `source/gameMainSystem/cEnvironment.*` now use `cRenderDevice`,
  `cRenderTexture`, and `cRenderSurface` at their boundaries. Direct D3D calls
  for image-file texture loading, render-target texture creation,
  texture-to-surface lookup, back-buffer lookup, and resource release now go
  through `cRenderBackendD3D9.h` helpers, with browser-side software texture and
  surface functions present in `cRenderBackendBrowser.h`.
- `source/cDrawingObject.*` now uses `cRenderDevice`, `cRenderTexture`,
  `cRenderGetTextureSize`, `cRenderSetTexture`, and
  `cRenderDrawColoredTriangleStrip`. Its sprite texture color modes and blend
  modes now go through `cRenderBackend.h` while keeping Direct3D as the active
  Windows backend.
- `source/gameMainSystem/cScreen.*` now uses `cRenderDevice`,
  `cRenderTexture`, and `cRenderSurface`, and its screen-composition path uses
  the render backend helpers for viewport setup, render target switching,
  target clear, texture binding, vertex format selection, alpha-blend enable,
  texture-stage color modes, blend factors, and triangle-strip drawing.
- `source/gameMainSystem/System.*` now uses `cRenderDevice` and `cRenderCaps`,
  and scene render setup uses the render backend helpers for target clear,
  culling, begin-scene, and end-scene.
- `source/gameMainSystem/cSceneVirtual.h` and
  `source/gameMainSystem/cSceneManager.*` now use `cRenderDevice` for scene
  lifecycle calls. The title, loading, home-first, and dungeon-first scenes now
  match that boundary, including their local texture variables where relevant.
- `source/cCommand.*` now uses `cRenderDevice` for command execution. The same
  boundary is now applied to the main command action classes and the config
  popup helpers in `ConfigCommands.*`. This keeps menu action behavior unchanged
  while removing the command layer's direct dependence on `IDirect3DDevice9`
  for Action dispatch.
- `source/cControl.h` no longer includes Direct3D headers directly and now
  uses `cRenderBackend.h`. The main UI control/window classes now use
  `cRenderDevice` for `Init`, `process`, `Draw`, text rendering, cursor/page
  drawing, and related helper calls. Their texture fields now use
  `cRenderTexture` where they previously exposed IDirect3DTexture9.
- `source/cGameObject.h` and the main drop/item classes now use
  `cRenderDevice` for item initialization, map/icon/state drawing, menu setup,
  drop actions, and status explanation drawing. Exposed texture handles in this
  layer now use `cRenderTexture`.
- The character, boss/mob, map/minimap, dungeon-system, game-screen-interface,
  control-vector, and parameter-gauge boundaries now use `cRenderDevice` and
  `cRenderTexture`.
- The animation/effect, scripted command, save initialization, number-field,
  and specialized drawing-object boundaries now use `cRenderDevice` and
  `cRenderTexture`. A direct search for `IDirect3DDevice9 *pDev`,
  `IDirect3DTexture9*`, `LPDIRECT3DDEVICE9`, and `LPDIRECT3DTEXTURE9` now
  returns no matches outside the D3D9 backend aliases.
- The packed graphic loader no longer includes Windows or D3DX headers
  directly. Its remaining browser-facing file concerns are generic C runtime
  file access and `TCHAR` path compatibility, while texture creation is now a
  render-backend responsibility.
- `source/utility/SpriteText.*` now preserves the existing public text drawing
  usage while routing its texture handle, texture creation, texture-size
  lookup, texture lock/unlock, vector size type, text context, text transform,
  glyph metrics, glyph width, and glyph bitmap lookup through
  `cRenderBackend.h`. The D3D9 backend still uses Windows GDI internally to
  preserve the current Windows rendering behavior, while the browser backend
  uses the browser Canvas text API to rasterize glyphs into the same texture
  path.
- Vector-only D3DX usages in `AnimeChips.*`, `StyleString.*`, the main
  UI/drop helper files, `EffectFunctions.h`, and
  `source/scriptprocesser/DungeonScriptFunction.h` now use `cRenderVector2`,
  `cRenderVector3`, or `cRenderVector4`.
- D3DX matrix/quaternion/math usage in
  `source/scriptprocesser/AnimationScript.cpp` and `source/EffectFunctions.cpp`
  now calls `cRender` math helpers. Lua still exposes the historical
  `D3DX...` class/function names for script compatibility, but the C++ binding
  types are `cRenderVector2`, `cRenderMatrix`, and `cRenderQuaternion`.
- Stale DirectX includes were removed from `DungeonInstance.h`,
  `DungeonMakeScript.cpp`, `DungeonStoryScript.cpp`, and `cDrawingObject_belt.h`.
  Direct D3D/D3DX headers are now concentrated in the D3D9 render backend and
  the still-Win32 `Main.cpp` entry path.
- `source/gameMainSystem/Main.cpp` still owns the Win32 window and message
  pump for Windows, but Direct3D interface creation, display-mode/capability
  lookup, present-parameter setup, device creation fallback, release, present,
  cooperative-level checks, and device reset now go through
  `cRenderBackend.h`. The browser entry path bypasses Win32 window/mutex
  setup and uses the same render-backend initialization scaffold.
- `source/utility/sVertexes.h` no longer depends on include order for
  `std::binary_function`, and its vertex color field now uses `cRenderColor`.
  Direct3D FVF constants are only exposed when the D3D headers define them,
  which lets the browser backend include the same vertex layout without pulling
  in Direct3D.
- This is intentionally not a new platform layer yet. It is the first safe
  boundary that lets a future Emscripten entry point call equivalent
  initialize/frame/finalize units without changing gameplay code.
- `source/cSoundManager.*` now keeps `sound/sound.h`, `DirectSound`, and
  `DirectSoundStream` behind `#ifndef __EMSCRIPTEN__`. The browser path keeps
  stable SE identifiers and BGM selection state and uses WebAudio for WAV
  playback from `web-build/sound`.
- `source/stdafx.h` no longer includes `windows.h` or `sound/sound.h` for
  `__EMSCRIPTEN__`, which prevents DirectSound headers from leaking back into
  browser-facing translation units through the precompiled-header path.
- `tools/build-web.ps1` now adds the Emscripten build path. It reads
  `laug_th.vcxproj`, generates `web-build\web-sources.rsp` and
  `web-build\web-build.rsp`, excludes the current Win32-only input, Wiimote,
  DirectSound, precompiled-header, and old platform entry sources, and creates a
  Web-only luabind copy with the C++11 `noexcept(false)` compatibility patch
  needed for Lua errors to reach the existing `catch(luabind::error)` paths.
  This keeps source selection tied to the existing Visual Studio project instead
  of maintaining a separate hand-written source list.
- The sound implementation intentionally stays behind the existing
  `cSoundManager` API. It preserves current BGM/SE calls and avoids any new
  gameplay UI or control mode.

## Reduced Web Compile Blockers

These platform dependencies now have narrow `__EMSCRIPTEN__` branches while the
Windows path remains unchanged:

- `source/cSaveStore.cpp` keeps the Win32 executable-version resource lookup for
  Windows, but the browser path no longer includes `Windows.h` or links
  `version.lib`; it returns the existing base version marker instead of querying
  an executable resource.
- `source/csLoading.cpp` keeps the Windows thread-handle path, but the browser
  path avoids Win32 thread headers and handle polling. It preserves the current
  loading-scene staging by running the existing loading body synchronously after
  the loading screen has drawn a few frames.
- `source/gameMainSystem/filemanage/cFileManager.cpp` keeps the Win32 code-page,
  directory, and file-time APIs for Windows. The browser path now opens files
  through a narrow UTF-8 path helper, creates parent directories with `mkdir`,
  and reads file timestamps with `stat`.
- Diagnostic `MessageBox` calls in `utility/debug.*` and
  `utility/valiableField/*` are now skipped for browser builds. `CFilePack.h`
  already guards its diagnostic `MessageBox` use.
- `source/web_compat/tchar.h` now provides an Emscripten-only `TCHAR` shim. It
  uses `char16_t` so packed file header names keep the same 2-byte storage width
  as the Windows Unicode build, and it supplies the small `_tcs*`, `_stprintf*`,
  `_tfopen_s`, text-stream, Win32-style scalar type, `min`/`max`, and
  `OutputDebugString` compatibility helpers currently needed by the selected Web
  source list.
- `source/gameMainSystem/filemanage/tstring_ph.h` keeps Windows
  `basic_ofstream<TCHAR>` / `basic_ifstream<TCHAR>` aliases, but maps
  `oftstream`/`iftstream` to the Web shim wrappers under `__EMSCRIPTEN__`.
- `source/gameMainSystem/filemanage/cFileManager.cpp` now decodes UTF-16 script
  text into `tstring`, so the browser `char16_t` `TCHAR` path does not depend
  on `wchar_t` being the same width as Windows.
- `source/gameMainSystem/filemanage/cScriptReader.cpp` now falls back from
  missing packed script data to the original script file under `__EMSCRIPTEN__`,
  allowing the browser build to use preloaded source `data` and `Language`
  assets without rebuilding the historical `Packed_*.id.cdat` files.
- `source/gameMainSystem/cRenderBackendBrowser.h` now includes the local
  `tchar.h` shim directly, so browser rendering code does not depend on
  incidental include order for `TCHAR` or `FW_REGULAR`.
- `source/gameMainSystem/cRenderBackendBrowser.h` now has a first real browser
  rendering path: software backbuffer/render-target textures, triangle-strip
  rasterization, basic Direct3D-style blend modes, texture lock/unlock for
  generated textures such as `SpriteText`, and `cRenderPresent` upload to an
  HTML canvas.

## Next Known Web Gaps

These are the next concrete gaps to handle narrowly so the port remains a
simple compatibility port rather than a gameplay rewrite.

- Browser `data` and `Language` runtime assets are now generated under
  `web-build/data` and `web-build/Language`, and text files with `.id`, `.csv`,
  or `.txt` extensions are emitted as UTF-16LE so `cFileManager` can use the
  existing Unicode decode path. The direct browser CP932 fallback remains only a
  compile bridge for any unconverted text files.
- Browser graphic assets now have a minimal raw texture path. `tools/build-web.ps1`
  converts existing PNG/JPEG/BMP files into `.ggntex` files under
  `web-build/assets/graphic`, and `cRenderLoadTextureFromFile` loads those raw
  textures from the browser preload filesystem. This avoids adding a PNG/JPEG
  decoder to the game code.
- Browser save persistence now has a minimal IDBFS bridge. A Chrome smoke test
  confirmed that `config.dat.cdat`, `player.dat.cdat`, and
  `intro_Dungeon_temp_save.dat.cdat` are written under `/save/` and are visible
  after refresh in the same browser profile.
- Browser sound packs are now expanded by `tools/build-web.ps1` into
  `web-build/sound/SE` and `web-build/sound/music`. The WebAudio backend fetches
  and decodes those WAV files on demand; the Windows DirectSound path remains
  intact. The remaining sound-specific web gap is package size, because the
  proof build still serves uncompressed WAV files.

## Verification Notes

- `.\tools\build-debug.ps1 -Configuration Release` succeeds after the current
  boundary changes and writes `ggn.exe`. The latest verified build output was
  `E:\old4\Bitbucket\source\ggn\laug_th\ggn.exe`, 7,318,528 bytes, written at
  2026-06-17 12:01:01 local time.
- The build still emits many pre-existing warnings, mainly numeric conversion
  warnings and the `CFilePack.h` `printf`/`TCHAR` warning. These are not new
  blocker errors for the minimal browser-port boundary work.
- `tools\build-web.ps1` succeeds when Emscripten is activated through
  `..\_tools\emsdk`, generating `web-build\ggn.html`,
  `web-build\ggn.js`, `web-build\ggn.wasm`, and `web-build\ggn.data`.
- Chrome headless smoke tests verified title display, `Z` to start, first
  dungeon entry, basic keyboard movement/input, readable HUD/window text,
  graphic asset loading, no JavaScript exceptions through the tested flow, and
  IDBFS save persistence after refresh.
- Chrome headless audio checks verified `Module.ggnAudio` creation, title BGM
  path `sound/music/title.wav;sound/music/title_roop.wav`, both title WAV files
  decoded into AudioBuffers, representative SE path `sound/SE/countdown.wav`
  decoded and played through the same backend, `AudioContext` state `running`,
  and no WebAudio errors.
- The only observed network error in the smoke tests is `favicon.ico` 404 from
  the generated Emscripten shell page; game assets load from the preload package
  or from `web-build/sound`.
