# Building and smoke testing

The maintained build target for this public branch is `Debug|Win32`.
`Release|Win32` is historical and still references release-only packaging/library assumptions that are not part of the public archive.

Current public distribution version: `1.0.5.3` (`2026-06-20`).

## Prerequisites

Install these on the build machine:

- Visual Studio 2022 with **Desktop development with C++**
  - MSVC v143 x86/x64 build tools
  - Windows SDK
- Complete Boost headers (`BoostRoot` must contain `boost\shared_ptr.hpp`, `boost\smart_ptr\shared_ptr.hpp`, and `boost\mpl\bool.hpp`). Boost 1.46.1 is known to work with the bundled luabind 0.9.1 code.
- D3DX9 headers/libs
  - Either DirectX SDK (June 2010), or
  - NuGet package `Microsoft.DXSDK.D3DX` restored by the build script
- Lua 5.1 and luabind 0.9.1 next to this repository:
  - `../lua5.1`
  - `../luabind-0.9.1`

This local archive already has `../lua5.1` and `../luabind-0.9.1`; they are external dependencies, not part of the public branch.


## Known released runtime

The historical released runtime checked during archive preparation identifies as `幻想郷幻想日記` / `ggn.exe` version `1.0.4.0`, with release notes dated `2014-12-21`.

That release used packed runtime data: `data/Packed_*.id.cdat`, `graphic/graphicpack`, `sound/data1`, `sound/data2`, and `Language/Default`. The maintained public build target is still `Debug|Win32`, which runs from `Debug\` and reads the source tree through `..\data`, `..\graphic`, `..\Language`, and `..\sound`. Full `Release_Archive` packaging is historical and is not reconstructed by the current script.

## Build

From the repository root, pass a complete Boost root explicitly or set `BOOST_ROOT`:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\build-debug.ps1 -BoostRoot C:\path\to\boost_1_46_1 -RestoreNuGetDeps
```

`-RestoreNuGetDeps` can restore `Microsoft.DXSDK.D3DX` for D3DX9. If DirectX SDK is installed somewhere custom, pass it explicitly:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\build-debug.ps1 -BoostRoot C:\path\to\boost_1_46_1 -DirectXSdkDir "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
```

On the recovered archive layout used during maintenance, the script also checks `..\..\..\..\libs\boost\boost_1_46_1` relative to this repository.

The expected output is:

```text
Debug\ggn_d.exe
```

For a distributable executable that reads runtime assets from the executable directory, build `Release|Win32`:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\build-debug.ps1 -Configuration Release -RestoreNuGetDeps
```

The expected release output is:

```text
ggn.exe
```

## Browser/Web build scaffold

The minimal browser port is prepared as an Emscripten/WebAssembly build scaffold. It intentionally reuses the existing C++ sources and does not add gameplay or UI features.

Install and activate Emscripten so `em++` is available on `PATH`, then run:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\build-web.ps1
```

To verify source selection without Emscripten installed:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\build-web.ps1 -GenerateOnly
```

To run the browser touch-control layout and audio lifecycle unit tests:

```powershell
node .\tools\test-web-touch-layout.js
node .\tools\test-web-audio-lifecycle.js
```

The script reads `laug_th.vcxproj`, resolves the existing Lua 5.1, luabind 0.9.1, and Boost 1.46.1-style dependency roots, prepends `source\web_compat` for browser-only compatibility headers, excludes the Win32-only input, Wiimote, DirectSound, precompiled-header, and old platform entry sources, and writes response files under `web-build\`. That directory is generated output and is ignored by Git.

For browser runtime assets, the script also converts existing `graphic\` PNG/JPEG/BMP files into simple raw texture files under `web-build\assets\graphic\*.ggntex`, copies lightweight non-image graphic metadata such as `graphicpass.id`, skips the large Windows `graphicpack`, converts `data\` and `Language\` text assets to UTF-16LE where appropriate, copies Lua/binary assets as-is, extracts `sound\data1` and `sound\data2` into `web-build\sound\SE\*.wav` and `web-build\sound\music\*.wav`, adds `/assets`, `/data`, and `/Language` preload arguments for Emscripten, and links `-lidbfs.js` for the minimal `/save/` persistent filesystem bridge. Sound files are served next to the generated page instead of being embedded into `ggn.data`.

Current status: the WebAssembly build succeeds in the recovered archive layout when Emscripten is activated through the local `..\_tools\emsdk` checkout. The generated runtime is:

```text
web-build\ggn.html
web-build\ggn.js
web-build\ggn.wasm
web-build\ggn.data
web-build\sound\
```

The browser build has been smoke-tested in Chrome with the existing keyboard controls. Verified so far: title display, `Z` to start the game, entering the first dungeon, basic movement/input without JavaScript exceptions, readable text rendering, graphic asset loading, and `/save/` IDBFS persistence across refresh. The browser sound path now uses a minimal WebAudio backend behind the existing `cSoundManager` API. The latest headless Chrome check verified that the title BGM intro/loop WAV files and a representative SE WAV are fetched and decoded into AudioBuffers with the AudioContext running.

## Runtime assets

The public branch does not include the large released sound packs. To run the built debug executable locally, copy these files from a released runtime into this repository:

```text
sound\data1
sound\data2
```

For the recovered maintenance machine, these were copied from:

```text
E:\2025D\games\ggn\幻想郷幻想日記\sound\data1
E:\2025D\games\ggn\幻想郷幻想日記\sound\data2
```

Those files are ignored by Git and should not be committed. Debug startup also uses existing packed graphics/script data by default, so running the game does not rewrite `graphic\graphicpack` or generate `Packed_*.id.cdat` files unless repacking macros are enabled intentionally.

## Smoke test

After building:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\run-smoke.ps1
```

The smoke test starts an executable, waits a few seconds, and treats an immediate exit or non-responding window as a failure. By default it stops the process after the check. Use `-KeepRunning` to leave the game window open.

Debug builds run from `Debug\` and use `..\data`, `..\graphic`, `..\Language`, and `..\sound` through the existing `_EXELOCATION` setting.

You can also smoke-test an existing copied runtime without changing this repository:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\run-smoke.ps1 -ExePath C:\path\to\ggn.exe
```

Run this against a disposable copy if you do not want the executable to update `savedata` or `log` files in that runtime folder.

## Release package

After a successful Release build, create a binary package by pointing the package script at a released runtime that contains the packed `data`, `graphic`, `Language`, and `sound` assets:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\package-release.ps1 -RuntimeRoot "E:\path\to\released-runtime"
```

The package is written under `Release_Archive\ggn-1.0.5.3\` and `Release_Archive\ggn-1.0.5.3.zip`. These outputs are ignored by Git.
