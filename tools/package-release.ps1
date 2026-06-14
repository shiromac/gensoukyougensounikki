param(
    [string]$Version = "1.0.5.2",
    [string]$RuntimeRoot,
    [string]$OutputRoot
)

$ErrorActionPreference = "Stop"
$repo = Split-Path -Parent $PSScriptRoot
if (-not $OutputRoot) { $OutputRoot = Join-Path $repo "Release_Archive" }

function Fail($message) {
    throw "[package-release] $message"
}

function Resolve-RequiredFile([string]$path) {
    $resolved = Resolve-Path -LiteralPath $path -ErrorAction SilentlyContinue
    if (-not $resolved) { Fail "Required file was not found: $path" }
    return $resolved.Path
}

function Resolve-RequiredDir([string]$path) {
    $resolved = Resolve-Path -LiteralPath $path -ErrorAction SilentlyContinue
    if (-not $resolved) { Fail "Required directory was not found: $path" }
    return $resolved.Path
}

function Copy-RequiredFile([string]$from, [string]$to) {
    $source = Resolve-RequiredFile $from
    $destDir = Split-Path -Parent $to
    New-Item -ItemType Directory -Force -Path $destDir | Out-Null
    Copy-Item -LiteralPath $source -Destination $to -Force
}

function Copy-RequiredDir([string]$from, [string]$to) {
    $source = Resolve-RequiredDir $from
    if (Test-Path -LiteralPath $to) { Remove-Item -LiteralPath $to -Recurse -Force }
    Copy-Item -LiteralPath $source -Destination $to -Recurse -Force
}

$exe = Resolve-RequiredFile (Join-Path $repo "ggn.exe")
$versionInfo = [Diagnostics.FileVersionInfo]::GetVersionInfo($exe)
$expectedCommaVersion = ($Version -split "\.") -join ", "
if ($versionInfo.FileVersion -ne $expectedCommaVersion -or $versionInfo.ProductVersion -ne $expectedCommaVersion) {
    Fail "ggn.exe version is $($versionInfo.FileVersion) / $($versionInfo.ProductVersion), expected $expectedCommaVersion. Build Release first."
}

if (-not $RuntimeRoot) { $RuntimeRoot = $repo }
$runtimeRootPath = Resolve-RequiredDir $RuntimeRoot

$outputRootPath = New-Item -ItemType Directory -Force -Path $OutputRoot
$packageName = "ggn-$Version"
$packageDir = Join-Path $outputRootPath.FullName $packageName
$zipPath = Join-Path $outputRootPath.FullName "$packageName.zip"

$repoRoot = (Resolve-Path -LiteralPath $repo).Path
$packageParent = (Resolve-Path -LiteralPath $outputRootPath.FullName).Path
if (-not $packageDir.StartsWith($packageParent, [StringComparison]::OrdinalIgnoreCase)) {
    Fail "Refusing to remove package directory outside output root: $packageDir"
}

if (Test-Path -LiteralPath $packageDir) { Remove-Item -LiteralPath $packageDir -Recurse -Force }
if (Test-Path -LiteralPath $zipPath) { Remove-Item -LiteralPath $zipPath -Force }
New-Item -ItemType Directory -Force -Path $packageDir | Out-Null

Copy-RequiredFile $exe (Join-Path $packageDir "ggn.exe")
Copy-RequiredFile (Join-Path $repo "README.md") (Join-Path $packageDir "README.md")
Copy-RequiredFile (Join-Path $repo "LICENSE.md") (Join-Path $packageDir "LICENSE.md")
Copy-RequiredFile (Join-Path $repo "NOTICE.md") (Join-Path $packageDir "NOTICE.md")
Copy-RequiredDir (Join-Path $repo "docs\manual") (Join-Path $packageDir "docs\manual")

New-Item -ItemType Directory -Force -Path (Join-Path $packageDir "log") | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $packageDir "savedata") | Out-Null

New-Item -ItemType Directory -Force -Path (Join-Path $packageDir "data") | Out-Null
foreach ($name in @(
    "Packed_Chara.id.cdat",
    "Packed_Dungeon.id.cdat",
    "Packed_Effect.id.cdat",
    "Packed_HomeDataBase.id.cdat",
    "Packed_Item.id.cdat",
    "Packed_Mapchip.id.cdat"
)) {
    Copy-RequiredFile (Join-Path (Join-Path $runtimeRootPath "data") $name) (Join-Path (Join-Path $packageDir "data") $name)
}

New-Item -ItemType Directory -Force -Path (Join-Path $packageDir "graphic") | Out-Null
Copy-RequiredFile (Join-Path (Join-Path $runtimeRootPath "graphic") "graphicpack") (Join-Path (Join-Path $packageDir "graphic") "graphicpack")
Copy-RequiredFile (Join-Path (Join-Path $runtimeRootPath "graphic") "Packed_graphicpass.id.cdat") (Join-Path (Join-Path $packageDir "graphic") "Packed_graphicpass.id.cdat")
Copy-RequiredDir (Join-Path (Join-Path $runtimeRootPath "graphic") "skin") (Join-Path (Join-Path $packageDir "graphic") "skin")
Copy-RequiredFile (Join-Path $runtimeRootPath "graphic\skinについて.txt") (Join-Path $packageDir "graphic\skinについて.txt")
# Keep loose effect textures as a fallback for lazily loaded packed textures.
Copy-RequiredDir (Join-Path $repo "graphic\effect") (Join-Path $packageDir "graphic\effect")

Copy-RequiredDir (Join-Path $runtimeRootPath "Language") (Join-Path $packageDir "Language")

New-Item -ItemType Directory -Force -Path (Join-Path $packageDir "sound") | Out-Null
Copy-RequiredFile (Join-Path (Join-Path $runtimeRootPath "sound") "data1") (Join-Path (Join-Path $packageDir "sound") "data1")
Copy-RequiredFile (Join-Path (Join-Path $runtimeRootPath "sound") "data2") (Join-Path (Join-Path $packageDir "sound") "data2")

$commit = (& git -C $repo rev-parse --short HEAD).Trim()
$tag = (& git -C $repo describe --tags --exact-match HEAD 2>$null)
$tag = if ($LASTEXITCODE -eq 0) { $tag.Trim() } else { "" }
$releaseText = @(
    "GGN release package",
    "Version: $Version",
    "Commit: $commit",
    "Tag: $tag",
    "Packaged: $((Get-Date).ToString("yyyy-MM-dd HH:mm:ss zzz"))",
    "",
    "Run ggn.exe to start the game.",
    "Open docs/manual/index.html to read the original manual locally.",
    "Large runtime data/audio files are packaged here for local distribution and are not committed to the public source branch.",
    "You may unpack this ZIP to run the game. Do not extract, redistribute, or reuse packed audio, graphics, scripts, scenarios, or data files as standalone material assets.",
    "Included BGM/music is not MIT-licensed and may only be used as part of playing this game."
)
Set-Content -LiteralPath (Join-Path $packageDir "RELEASE.txt") -Value $releaseText -Encoding UTF8

$manifest = Get-ChildItem -LiteralPath $packageDir -Recurse -File |
    Sort-Object FullName |
    ForEach-Object {
        $relative = $_.FullName.Substring($packageDir.Length + 1)
        $hash = Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256
        "{0}`t{1}`t{2}" -f $hash.Hash, $_.Length, $relative
    }
Set-Content -LiteralPath (Join-Path $packageDir "SHA256SUMS.txt") -Value $manifest -Encoding ASCII

Compress-Archive -LiteralPath $packageDir -DestinationPath $zipPath -CompressionLevel Optimal -Force

Write-Host "Package directory: $packageDir"
Write-Host "Package zip: $zipPath"
