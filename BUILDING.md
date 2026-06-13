# Building and smoke testing

The maintained build target for this public branch is `Debug|Win32`.
`Release|Win32` is historical and still references release-only packaging/library assumptions that are not part of the public archive.

## Prerequisites

Install these on the build machine:

- Visual Studio 2022 with **Desktop development with C++**
  - MSVC v143 x86/x64 build tools
  - Windows SDK
- Boost headers (`BoostRoot` must contain `boost\shared_ptr.hpp`)
- D3DX9 headers/libs
  - Either DirectX SDK (June 2010), or
  - NuGet package `Microsoft.DXSDK.D3DX` restored by the build script
- Lua 5.1 and luabind 0.9.1 next to this repository:
  - `../lua5.1`
  - `../luabind-0.9.1`

This local archive already has `../lua5.1` and `../luabind-0.9.1`; they are external dependencies, not part of the public branch.

## Build

From the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\build-debug.ps1 -RestoreNuGetDeps
```

If Boost or DirectX SDK are installed somewhere custom, pass them explicitly:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\build-debug.ps1 -BoostRoot C:\path\to\boost_1_xx_x -DirectXSdkDir "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
```

The expected output is:

```text
Debug\ggn_d.exe
```

## Smoke test

After building:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\run-smoke.ps1
```

The smoke test starts `Debug\ggn_d.exe`, waits a few seconds, and treats an immediate exit as a failure. By default it stops the process after the check. Use `-KeepRunning` to leave the game window open.

Debug builds run from `Debug\` and use `..\data`, `..\graphic`, `..\Language`, and `..\sound` through the existing `_EXELOCATION` setting.
