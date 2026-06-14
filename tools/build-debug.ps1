param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",
    [string]$Platform = "Win32",
    [string]$ThirdPartyRoot,
    [string]$BoostRoot,
    [string]$DirectXSdkDir,
    [switch]$RestoreNuGetDeps
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot

function Fail($message) {
    throw "[build-debug] $message"
}

function Find-VsInstall {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path -LiteralPath $vswhere)) { return $null }
    $path = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($path)) { return $null }
    return $path.Trim()
}

function Find-NuGet {
    $cmd = Get-Command nuget.exe -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    $cmd = Get-Command nuget -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    $tempNuget = Join-Path $env:TEMP "nuget.exe"
    if (Test-Path -LiteralPath $tempNuget) { return $tempNuget }
    return $null
}

function Restore-Package([string]$packageId) {
    $nuget = Find-NuGet
    if (-not $nuget) { Fail "nuget.exe was not found; cannot restore $packageId." }
    $packages = Join-Path $root "packages"
    & $nuget install $packageId -OutputDirectory $packages -NonInteractive | Write-Host
    if ($LASTEXITCODE -ne 0) { Fail "NuGet package restore failed: $packageId" }
}

function Find-FileDir([string]$base, [string]$fileName, [string]$mustContain) {
    if (-not (Test-Path -LiteralPath $base)) { return $null }
    $hits = Get-ChildItem -LiteralPath $base -Recurse -File -Filter $fileName -ErrorAction SilentlyContinue
    if ($mustContain) { $hits = $hits | Where-Object { $_.FullName -like "*$mustContain*" } }
    $hit = $hits | Select-Object -First 1
    if ($hit) { return $hit.DirectoryName }
    return $null
}

function Test-BoostRoot([string]$path) {
    if ([string]::IsNullOrWhiteSpace($path) -or -not (Test-Path -LiteralPath $path)) { return $false }
    $required = @(
        "boost\shared_ptr.hpp",
        "boost\smart_ptr\shared_ptr.hpp",
        "boost\mpl\bool.hpp"
    )
    foreach ($file in $required) {
        if (-not (Test-Path -LiteralPath (Join-Path $path $file))) { return $false }
    }
    return $true
}

function Find-BoostRoot([string]$base) {
    if (-not (Test-Path -LiteralPath $base)) { return $null }
    if (Test-BoostRoot $base) { return (Resolve-Path -LiteralPath $base).Path }
    $versions = Get-ChildItem -LiteralPath $base -Recurse -File -Filter version.hpp -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -match '[\\/]boost[\\/]version\.hpp$' }
    foreach ($version in $versions) {
        $candidate = Split-Path -Parent $version.DirectoryName
        if (Test-BoostRoot $candidate) { return $candidate }
    }
    return $null
}

$vs = Find-VsInstall
if (-not $vs) {
    Fail "Visual Studio C++ tools were not found. Install Desktop development with C++, MSVC v143 x86/x64 build tools, and Windows SDK."
}
$msbuild = Join-Path $vs "MSBuild\Current\Bin\MSBuild.exe"
$cppTargets = Join-Path $vs "MSBuild\Microsoft\VC\v170\Microsoft.Cpp.Default.props"
if (-not (Test-Path -LiteralPath $msbuild)) { Fail "MSBuild.exe was not found: $msbuild" }
if (-not (Test-Path -LiteralPath $cppTargets)) { Fail "C++ MSBuild targets were not found: $cppTargets" }

if (-not $ThirdPartyRoot) { $ThirdPartyRoot = (Resolve-Path -LiteralPath (Join-Path $root "..")).Path }
$luaRoot = Join-Path $ThirdPartyRoot "lua5.1"
$luabindRoot = Join-Path $ThirdPartyRoot "luabind-0.9.1"
if (-not (Test-Path -LiteralPath (Join-Path $luaRoot "include\lua.h"))) { Fail "Lua 5.1 headers were not found: $luaRoot" }
if (-not (Test-Path -LiteralPath (Join-Path $luabindRoot "luabind\luabind.hpp"))) { Fail "luabind 0.9.1 headers were not found: $luabindRoot" }

function Ensure-LuabindMsvcNoexceptPatch($luabindRootPath) {
    $header = Join-Path $luabindRootPath "luabind\detail\call_function.hpp"
    if (-not (Test-Path -LiteralPath $header)) { Fail "luabind call_function.hpp was not found: $header" }

    $text = Get-Content -LiteralPath $header -Raw
    if ($text -match "~proxy_function_void_caller\(\)\s+noexcept\(false\)") {
        return
    }

    $old = @"
				~proxy_function_void_caller()
				{
"@
    $new = @"
#if defined(_MSC_VER) && _MSC_VER >= 1900
				~proxy_function_void_caller() noexcept(false)
#else
				~proxy_function_void_caller()
#endif
				{
"@
    if (-not $text.Contains($old)) {
        Fail "luabind call_function.hpp did not match the expected 0.9.1 layout. Apply the MSVC noexcept compatibility patch manually: $header"
    }

    [System.IO.File]::WriteAllText($header, $text.Replace($old, $new), [System.Text.Encoding]::ASCII)
    Write-Host "Applied luabind 0.9.1 MSVC noexcept compatibility patch: $header"
}

Ensure-LuabindMsvcNoexceptPatch $luabindRoot

if ($BoostRoot -and -not (Test-BoostRoot $BoostRoot)) {
    Fail "BoostRoot is incomplete: $BoostRoot"
}
if (-not $BoostRoot) {
    $boostSearchRoots = @()
    if ($env:BOOST_ROOT) { $boostSearchRoots += $env:BOOST_ROOT }
    $boostSearchRoots += Join-Path $root "packages"
    $boostSearchRoots += Join-Path $ThirdPartyRoot "boost"
    $boostSearchRoots += Join-Path $ThirdPartyRoot "boost_1_46_1"
    $boostSearchRoots += Join-Path $root "..\..\..\..\libs\boost\boost_1_46_1"

    foreach ($candidate in $boostSearchRoots) {
        $found = Find-BoostRoot $candidate
        if ($found) {
            $BoostRoot = $found
            break
        }
    }
}
if (-not $BoostRoot -and $RestoreNuGetDeps) {
    Restore-Package "boost"
    $BoostRoot = Find-BoostRoot (Join-Path $root "packages")
}
if (-not (Test-BoostRoot $BoostRoot)) {
    Fail "Boost headers were not found. Pass -BoostRoot <path>, set BOOST_ROOT, or install a complete Boost header tree."
}

$d3dxInclude = $null
$d3dxLib = $null
if ($DirectXSdkDir) {
    $d3dxInclude = Join-Path $DirectXSdkDir "Include"
    $d3dxLib = Join-Path $DirectXSdkDir "Lib\x86"
} elseif ($env:DXSDK_DIR) {
    $d3dxInclude = Join-Path $env:DXSDK_DIR "Include"
    $d3dxLib = Join-Path $env:DXSDK_DIR "Lib\x86"
} else {
    $defaultDx = "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
    if (Test-Path -LiteralPath $defaultDx) {
        $d3dxInclude = Join-Path $defaultDx "Include"
        $d3dxLib = Join-Path $defaultDx "Lib\x86"
    }
}
if (-not $d3dxInclude -or -not (Test-Path -LiteralPath (Join-Path $d3dxInclude "d3dx9.h")) -or -not (Test-Path -LiteralPath (Join-Path $d3dxLib "d3dx9.lib"))) {
    $packages = Join-Path $root "packages"
    $packageD3dxInclude = Find-FileDir $packages "d3dx9.h" $null
    $packageD3dxLib = Find-FileDir $packages "d3dx9.lib" "x86"
    if ($packageD3dxInclude -and $packageD3dxLib) {
        $d3dxInclude = $packageD3dxInclude
        $d3dxLib = $packageD3dxLib
    }
}
if (-not $d3dxInclude -or -not (Test-Path -LiteralPath (Join-Path $d3dxInclude "d3dx9.h")) -or -not (Test-Path -LiteralPath (Join-Path $d3dxLib "d3dx9.lib"))) {
    if ($RestoreNuGetDeps) {
        Restore-Package "Microsoft.DXSDK.D3DX"
        $packages = Join-Path $root "packages"
        $d3dxInclude = Find-FileDir $packages "d3dx9.h" $null
        $d3dxLib = Find-FileDir $packages "d3dx9.lib" "x86"
    }
}
if (-not $d3dxInclude -or -not (Test-Path -LiteralPath (Join-Path $d3dxInclude "d3dx9.h")) -or -not (Test-Path -LiteralPath (Join-Path $d3dxLib "d3dx9.lib"))) {
    Fail "D3DX9 headers/libs were not found. Install DirectX SDK (June 2010), pass -DirectXSdkDir, or use -RestoreNuGetDeps to restore Microsoft.DXSDK.D3DX."
}

$args = @(
    (Join-Path $root "laug_th.vcxproj"),
    "/p:Configuration=$Configuration",
    "/p:Platform=$Platform",
    "/p:ThirdPartyRoot=$ThirdPartyRoot\",
    "/p:BoostRoot=$BoostRoot",
    "/p:D3DXIncludeDir=$d3dxInclude",
    "/p:D3DXLibDir=$d3dxLib",
    "/p:UseStructuredOutput=false",
    "/m",
    "/v:minimal"
)
& $msbuild @args
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$exe = Join-Path $root "Debug\ggn_d.exe"
if ($Configuration -eq "Release") { $exe = Join-Path $root "ggn.exe" }
if (-not (Test-Path -LiteralPath $exe)) { Fail "Build succeeded but executable was not found: $exe" }
Write-Host "Built: $exe"
