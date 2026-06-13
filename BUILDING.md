# Building and smoke testing

The maintained build target for this public branch is `Debug|Win32`.
`Release|Win32` is historical and still references release-only packaging/library assumptions that are not part of the public archive.

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

The released runtime checked during archive preparation identifies as `幻想郷幻想日記` / `ggn.exe` version `1.0.4.0`, with release notes dated `2014-12-21`.

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

## Smoke test

After building:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\run-smoke.ps1
```

The smoke test starts an executable, waits a few seconds, and treats an immediate exit as a failure. By default it stops the process after the check. Use `-KeepRunning` to leave the game window open.

Debug builds run from `Debug\` and use `..\data`, `..\graphic`, `..\Language`, and `..\sound` through the existing `_EXELOCATION` setting.

You can also smoke-test an existing copied runtime without changing this repository:

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\run-smoke.ps1 -ExePath C:\path\to\ggn.exe
```

Run this against a disposable copy if you do not want the executable to update `savedata` or `log` files in that runtime folder.
