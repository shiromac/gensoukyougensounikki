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
    return $null
}

function Restore-Package([string]$packageId) {
    $nuget = Find-NuGet
    if (-not $nuget) { Fail "nuget.exe が見つかりません。$packageId を復元できません。" }
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

function Find-BoostRoot([string]$base) {
    if (-not (Test-Path -LiteralPath $base)) { return $null }
    $version = Get-ChildItem -LiteralPath $base -Recurse -File -Filter version.hpp -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -match '[\\/]boost[\\/]version\.hpp$' } |
        Select-Object -First 1
    if (-not $version) { return $null }
    return Split-Path -Parent $version.DirectoryName
}

$vs = Find-VsInstall
if (-not $vs) {
    Fail "Visual Studio C++ tools が見つかりません。Visual Studio Installer で 'Desktop development with C++' / 'MSVC v143 x86/x64 build tools' / Windows SDK を追加してください。"
}
$msbuild = Join-Path $vs "MSBuild\Current\Bin\MSBuild.exe"
$cppTargets = Join-Path $vs "MSBuild\Microsoft\VC\v170\Microsoft.Cpp.Default.props"
if (-not (Test-Path -LiteralPath $msbuild)) { Fail "MSBuild.exe が見つかりません: $msbuild" }
if (-not (Test-Path -LiteralPath $cppTargets)) { Fail "C++ MSBuild targets が見つかりません: $cppTargets" }

if (-not $ThirdPartyRoot) { $ThirdPartyRoot = (Resolve-Path -LiteralPath (Join-Path $root "..")).Path }
$luaRoot = Join-Path $ThirdPartyRoot "lua5.1"
$luabindRoot = Join-Path $ThirdPartyRoot "luabind-0.9.1"
if (-not (Test-Path -LiteralPath (Join-Path $luaRoot "include\lua.h"))) { Fail "Lua 5.1 headers が見つかりません: $luaRoot" }
if (-not (Test-Path -LiteralPath (Join-Path $luabindRoot "luabind\luabind.hpp"))) { Fail "luabind 0.9.1 headers が見つかりません: $luabindRoot" }

if (-not $BoostRoot) {
    $candidate = Join-Path $root "packages"
    $BoostRoot = Find-BoostRoot $candidate
}
if (-not $BoostRoot -or -not (Test-Path -LiteralPath (Join-Path $BoostRoot "boost\shared_ptr.hpp"))) {
    if ($RestoreNuGetDeps) {
        Restore-Package "boost"
        $BoostRoot = Find-BoostRoot (Join-Path $root "packages")
    }
}
if (-not $BoostRoot -or -not (Test-Path -LiteralPath (Join-Path $BoostRoot "boost\shared_ptr.hpp"))) {
    Fail "Boost headers が見つかりません。-BoostRoot <boostの親フォルダ> を指定するか、-RestoreNuGetDeps を使ってください。"
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
    if ($RestoreNuGetDeps) {
        Restore-Package "Microsoft.DXSDK.D3DX"
        $packages = Join-Path $root "packages"
        $d3dxInclude = Find-FileDir $packages "d3dx9.h" $null
        $d3dxLib = Find-FileDir $packages "d3dx9.lib" "x86"
    }
}
if (-not $d3dxInclude -or -not (Test-Path -LiteralPath (Join-Path $d3dxInclude "d3dx9.h")) -or -not (Test-Path -LiteralPath (Join-Path $d3dxLib "d3dx9.lib"))) {
    Fail "D3DX9 headers/libs が見つかりません。DirectX SDK (June 2010) を入れるか、-RestoreNuGetDeps を使って Microsoft.DXSDK.D3DX を復元してください。"
}

$args = @(
    (Join-Path $root "laug_th.vcxproj"),
    "/p:Configuration=$Configuration",
    "/p:Platform=$Platform",
    "/p:ThirdPartyRoot=$ThirdPartyRoot\",
    "/p:BoostRoot=$BoostRoot",
    "/p:D3DXIncludeDir=$d3dxInclude",
    "/p:D3DXLibDir=$d3dxLib",
    "/m",
    "/v:minimal"
)
& $msbuild @args
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$exe = Join-Path $root "Debug\ggn_d.exe"
if ($Configuration -eq "Release") { $exe = Join-Path $root "ggn.exe" }
if (-not (Test-Path -LiteralPath $exe)) { Fail "Build succeeded but executable was not found: $exe" }
Write-Host "Built: $exe"
