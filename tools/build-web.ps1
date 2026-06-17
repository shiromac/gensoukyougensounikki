param(
    [string]$ThirdPartyRoot,
    [string]$LuaRoot,
    [string]$LuabindRoot,
    [string]$BoostRoot,
    [string]$OutDir = "web-build",
    [string]$OutputName = "ggn",
    [switch]$GenerateOnly
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot

function Fail($message) {
    [Console]::Error.WriteLine("[build-web] $message")
    exit 1
}

function Resolve-ProjectPath([string]$path) {
    return (Resolve-Path -LiteralPath $path).Path
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

function Quote-ResponseArg([string]$value) {
    '"' + ($value -replace '"', '\"') + '"'
}

function Test-ImageAssetExtension([string]$extension) {
    $normalized = $extension.ToLowerInvariant()
    return $normalized -eq ".png" -or $normalized -eq ".jpg" -or $normalized -eq ".jpeg" -or $normalized -eq ".bmp"
}

function Write-LittleEndianUInt32([System.IO.BinaryWriter]$writer, [UInt32]$value) {
    $writer.Write($value)
}

function Convert-ImageToWebTexture([string]$sourcePath, [string]$targetPath) {
    if ((Test-Path -LiteralPath $targetPath) -and
        ((Get-Item -LiteralPath $targetPath).LastWriteTimeUtc -ge (Get-Item -LiteralPath $sourcePath).LastWriteTimeUtc)) {
        return $false
    }

    $targetDirectory = Split-Path -Parent $targetPath
    New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null

    Add-Type -AssemblyName System.Drawing
    $image = $null
    $bitmap = $null
    $graphics = $null
    $bits = $null
    try {
        $image = [System.Drawing.Image]::FromFile($sourcePath)
        $bitmap = New-Object System.Drawing.Bitmap $image.Width, $image.Height, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
        $graphics.DrawImage($image, 0, 0, $image.Width, $image.Height)

        $rectangle = New-Object System.Drawing.Rectangle 0, 0, $bitmap.Width, $bitmap.Height
        $bits = $bitmap.LockBits($rectangle, [System.Drawing.Imaging.ImageLockMode]::ReadOnly, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
        $rowBytes = $bitmap.Width * 4
        $pixels = New-Object byte[] ($rowBytes * $bitmap.Height)
        for ($y = 0; $y -lt $bitmap.Height; $y++) {
            $rowPointer = [IntPtr]($bits.Scan0.ToInt64() + ($y * $bits.Stride))
            [System.Runtime.InteropServices.Marshal]::Copy($rowPointer, $pixels, $y * $rowBytes, $rowBytes)
        }

        $stream = [System.IO.File]::Open($targetPath, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write, [System.IO.FileShare]::None)
        try {
            $writer = New-Object System.IO.BinaryWriter $stream, ([System.Text.Encoding]::ASCII)
            try {
                $writer.Write([byte[]](0x47, 0x47, 0x4e, 0x54, 0x45, 0x58, 0x31, 0x00))
                Write-LittleEndianUInt32 $writer ([UInt32]$bitmap.Width)
                Write-LittleEndianUInt32 $writer ([UInt32]$bitmap.Height)
                $writer.Write($pixels)
            }
            finally {
                $writer.Dispose()
            }
        }
        finally {
            if ($stream) { $stream.Dispose() }
        }
    }
    finally {
        if ($bits -ne $null -and $bitmap -ne $null) { $bitmap.UnlockBits($bits) }
        if ($graphics -ne $null) { $graphics.Dispose() }
        if ($bitmap -ne $null) { $bitmap.Dispose() }
        if ($image -ne $null) { $image.Dispose() }
    }

    return $true
}

function Copy-WebAssetFile([string]$sourcePath, [string]$targetPath) {
    if ((Test-Path -LiteralPath $targetPath) -and
        ((Get-Item -LiteralPath $targetPath).LastWriteTimeUtc -ge (Get-Item -LiteralPath $sourcePath).LastWriteTimeUtc)) {
        return $false
    }

    $targetDirectory = Split-Path -Parent $targetPath
    New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null
    Copy-Item -LiteralPath $sourcePath -Destination $targetPath -Force
    return $true
}

function Generate-WebLuabindTree([string]$sourceRoot, [string]$targetRoot) {
    if (-not (Test-Path -LiteralPath $sourceRoot)) {
        Fail "luabind root was not found: $sourceRoot"
    }

    New-Item -ItemType Directory -Force -Path $targetRoot | Out-Null
    $sourceRootFull = (Resolve-Path -LiteralPath $sourceRoot).Path.TrimEnd('\')
    $copied = 0
    Get-ChildItem -LiteralPath $sourceRootFull -Recurse -File | ForEach-Object {
        $relative = $_.FullName.Substring($sourceRootFull.Length).TrimStart('\')
        $target = Join-Path $targetRoot $relative
        if (Copy-WebAssetFile $_.FullName $target) { $copied++ }
    }

    $callFunctionHeader = Join-Path $targetRoot "luabind\detail\call_function.hpp"
    if (-not (Test-Path -LiteralPath $callFunctionHeader)) {
        Fail "luabind call_function.hpp was not found in web copy: $callFunctionHeader"
    }

    $utf8NoBom = [System.Text.UTF8Encoding]::new($false)
    $content = [System.IO.File]::ReadAllText($callFunctionHeader, $utf8NoBom)
    $oldNoexceptGuard = "#if defined(_MSC_VER) && _MSC_VER >= 1900"
    $newNoexceptGuard = "#if (defined(_MSC_VER) && _MSC_VER >= 1900) || (defined(__cplusplus) && __cplusplus >= 201103L)"
    if ($content.Contains($oldNoexceptGuard)) {
        $content = $content.Replace($oldNoexceptGuard, $newNoexceptGuard)
        [System.IO.File]::WriteAllText($callFunctionHeader, $content, $utf8NoBom)
    }

    Write-Host "Web luabind tree: copied $copied file(s) and applied C++11 noexcept compatibility patch."
}

function Test-ProjectSourceTextExtension([string]$extension) {
    $normalized = $extension.ToLowerInvariant()
    return $normalized -eq ".h" -or $normalized -eq ".hpp" -or $normalized -eq ".cpp" -or
        $normalized -eq ".c" -or $normalized -eq ".inl" -or $normalized -eq ".rc"
}

function Convert-ProjectSourceToUtf8([string]$sourcePath, [string]$targetPath) {
    $targetDirectory = Split-Path -Parent $targetPath
    New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null

    $bytes = [System.IO.File]::ReadAllBytes($sourcePath)
    $strictUtf8 = [System.Text.UTF8Encoding]::new($false, $true)
    $cp932 = [System.Text.Encoding]::GetEncoding(932)
    try {
        $content = $strictUtf8.GetString($bytes)
    }
    catch {
        $content = $cp932.GetString($bytes)
    }
    # Full-width percent signs are valid in MSVC identifiers but rejected by clang.
    $content = $content.Replace([string][char]0xff05, "Percent")
    $content = [regex]::Replace($content, '##(?=\s*[:(;\\])', '')
    $content = [regex]::Replace($content, '(?<![A-Za-z0-9_>.])\brandom\s*\(\s*\)', 'ggn_random()')
    if ([System.IO.Path]::GetFileName($sourcePath) -eq "cRandomGen.h") {
        $content = $content.Replace('inline double ggn_random(){return genrand_real2();};', 'inline double random(){return genrand_real2();};')
        $content = $content.Replace('inline double operator()(){return ggn_random();};', 'inline double operator()(){return random();};')
    }
    [System.IO.File]::WriteAllText($targetPath, $content, [System.Text.UTF8Encoding]::new($false))
}

function Generate-WebSourceTree([string]$sourceRoot, [string]$targetRoot) {
    if (-not (Test-Path -LiteralPath $sourceRoot)) {
        Fail "Source root was not found: $sourceRoot"
    }

    New-Item -ItemType Directory -Force -Path $targetRoot | Out-Null
    $sourceRootFull = (Resolve-Path -LiteralPath $sourceRoot).Path.TrimEnd('\')
    $converted = 0
    $copied = 0

    Get-ChildItem -LiteralPath $sourceRootFull -Recurse -File | ForEach-Object {
        $relative = $_.FullName.Substring($sourceRootFull.Length).TrimStart('\')
        $target = Join-Path $targetRoot $relative
        if (Test-ProjectSourceTextExtension $_.Extension) {
            Convert-ProjectSourceToUtf8 $_.FullName $target
            $converted++
        }
        else {
            if (Copy-WebAssetFile $_.FullName $target) { $copied++ }
        }
    }

    Write-Host "Web source tree: converted $converted text file(s), copied $copied file(s)."
}

function Convert-ToWebSourcePath([string]$path, [string]$sourceRoot, [string]$webSourceRoot) {
    $sourceRootFull = (Resolve-Path -LiteralPath $sourceRoot).Path.TrimEnd('\')
    $fullPath = (Resolve-Path -LiteralPath $path).Path
    if ($fullPath.StartsWith($sourceRootFull + '\', [System.StringComparison]::OrdinalIgnoreCase)) {
        $relative = $fullPath.Substring($sourceRootFull.Length).TrimStart('\')
        return (Resolve-Path -LiteralPath (Join-Path $webSourceRoot $relative)).Path
    }
    return $fullPath
}

function Test-TextRuntimeAssetExtension([string]$extension) {
    $normalized = $extension.ToLowerInvariant()
    return $normalized -eq ".id" -or $normalized -eq ".csv" -or $normalized -eq ".txt"
}

function Convert-TextRuntimeAsset([string]$sourcePath, [string]$targetPath) {
    if ((Test-Path -LiteralPath $targetPath) -and
        ((Get-Item -LiteralPath $targetPath).LastWriteTimeUtc -ge (Get-Item -LiteralPath $sourcePath).LastWriteTimeUtc)) {
        return $false
    }

    $targetDirectory = Split-Path -Parent $targetPath
    New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null

    $sourceBytes = [System.IO.File]::ReadAllBytes($sourcePath)
    if ($sourceBytes.Length -ge 2 -and $sourceBytes[0] -eq 0xff -and $sourceBytes[1] -eq 0xfe) {
        [System.IO.File]::WriteAllBytes($targetPath, $sourceBytes)
        return $true
    }

    $cp932 = [System.Text.Encoding]::GetEncoding(932)
    $utf16 = [System.Text.Encoding]::Unicode
    $content = $cp932.GetString($sourceBytes)
    $converted = $utf16.GetBytes($content)
    $output = New-Object byte[] ($converted.Length + 2)
    $output[0] = 0xff
    $output[1] = 0xfe
    [Array]::Copy($converted, 0, $output, 2, $converted.Length)
    [System.IO.File]::WriteAllBytes($targetPath, $output)
    return $true
}

function Generate-WebRuntimeAssets([string]$sourceRoot, [string]$targetRoot, [string]$label) {
    if (-not (Test-Path -LiteralPath $sourceRoot)) {
        Fail "$label asset root was not found: $sourceRoot"
    }

    New-Item -ItemType Directory -Force -Path $targetRoot | Out-Null
    $converted = 0
    $copied = 0
    $sourceRootFull = (Resolve-Path -LiteralPath $sourceRoot).Path.TrimEnd('\')

    Get-ChildItem -LiteralPath $sourceRootFull -Recurse -File | ForEach-Object {
        $relative = $_.FullName.Substring($sourceRootFull.Length).TrimStart('\')
        $target = Join-Path $targetRoot $relative
        if (Test-TextRuntimeAssetExtension $_.Extension) {
            if (Convert-TextRuntimeAsset $_.FullName $target) { $converted++ }
        }
        else {
            if (Copy-WebAssetFile $_.FullName $target) { $copied++ }
        }
    }

    Write-Host "Web $label assets: converted $converted text file(s), copied $copied file(s)."
}

function Generate-WebAssets([string]$graphicRoot, [string]$assetGraphicRoot) {
    if (-not (Test-Path -LiteralPath $graphicRoot)) {
        Fail "Graphic asset root was not found: $graphicRoot"
    }

    New-Item -ItemType Directory -Force -Path $assetGraphicRoot | Out-Null
    $converted = 0
    $copied = 0
    $skipped = 0
    $graphicRootFull = (Resolve-Path -LiteralPath $graphicRoot).Path.TrimEnd('\')

    Get-ChildItem -LiteralPath $graphicRootFull -Recurse -File | ForEach-Object {
        $relative = $_.FullName.Substring($graphicRootFull.Length).TrimStart('\')
        if ($_.Name -eq "graphicpack") {
            $skipped++
            return
        }

        $target = Join-Path $assetGraphicRoot $relative
        if (Test-ImageAssetExtension $_.Extension) {
            if (Convert-ImageToWebTexture $_.FullName ($target + ".ggntex")) { $converted++ }
        }
        else {
            if (Copy-WebAssetFile $_.FullName $target) { $copied++ }
        }
    }

    Write-Host "Web assets: converted $converted image(s), copied $copied file(s), skipped $skipped file(s)."
}

function Decode-PackInt([int]$value, [int]$key) {
    return (-bnot ($value - $key))
}

function Decode-PackTChar([UInt16]$value, [int]$key) {
    return [char]((-bnot ((([int]$value - $key) -band 0xffff))) -band 0xffff)
}

function Normalize-PackPath([string]$value) {
    $normalized = $value.Replace('/', '\')
    if ($normalized.StartsWith(".\")) {
        $normalized = $normalized.Substring(2)
    }
    return $normalized
}

function Find-WebFfmpeg() {
    if ($script:WebFfmpegPath) { return $script:WebFfmpegPath }

    $command = Get-Command "ffmpeg" -ErrorAction SilentlyContinue
    if ($command) {
        $script:WebFfmpegPath = $command.Source
        return $script:WebFfmpegPath
    }

    $knownPaths = @(
        "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\Gyan.FFmpeg_Microsoft.Winget.Source_8wekyb3d8bbwe\ffmpeg-8.1.1-full_build\bin\ffmpeg.exe"
    )
    foreach ($candidate in $knownPaths) {
        if (Test-Path -LiteralPath $candidate) {
            $script:WebFfmpegPath = (Resolve-Path -LiteralPath $candidate).Path
            return $script:WebFfmpegPath
        }
    }

    Fail "ffmpeg was not found. Install ffmpeg or add ffmpeg.exe to PATH before generating compressed browser BGM."
}

function Get-WebSoundTargetPath([string]$targetRoot, [string]$relativePath, [string]$label) {
    $target = Join-Path $targetRoot $relativePath
    if ($label -eq "BGM" -and [System.IO.Path]::GetExtension($target).ToLowerInvariant() -eq ".wav") {
        return [System.IO.Path]::ChangeExtension($target, ".m4a")
    }
    return $target
}

function Convert-WebBgmWavBytes([byte[]]$data, [string]$targetPath) {
    $targetDirectory = Split-Path -Parent $targetPath
    New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null

    $ffmpeg = Find-WebFfmpeg
    $tempPath = Join-Path ([System.IO.Path]::GetTempPath()) ("ggn-web-bgm-" + [System.Guid]::NewGuid().ToString("N") + ".wav")
    try {
        [System.IO.File]::WriteAllBytes($tempPath, $data)
        & $ffmpeg -hide_banner -loglevel error -y -i $tempPath -vn -c:a aac -b:a 128k -movflags +faststart $targetPath
        if ($LASTEXITCODE -ne 0) {
            Fail "ffmpeg failed while encoding browser BGM: $targetPath"
        }
    }
    finally {
        if (Test-Path -LiteralPath $tempPath) {
            Remove-Item -LiteralPath $tempPath -Force
        }
    }
}

function Remove-WebStaleBgmWav([string]$targetRoot) {
    $musicRoot = Join-Path $targetRoot "sound\music"
    if (-not (Test-Path -LiteralPath $musicRoot)) { return }
    Get-ChildItem -LiteralPath $musicRoot -File -Filter *.wav -ErrorAction SilentlyContinue |
        Remove-Item -Force
}

function Expand-WebSoundPack([string]$packPath, [string]$targetRoot, [string]$label) {
    if (-not (Test-Path -LiteralPath $packPath)) {
        Write-Host "Web sound assets: skipped missing $label pack: $packPath"
        return
    }

    $packItem = Get-Item -LiteralPath $packPath
    $stream = [System.IO.File]::Open($packItem.FullName, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::Read)
    try {
        $reader = New-Object System.IO.BinaryReader $stream, ([System.Text.Encoding]::Unicode)
        try {
            $key = $reader.ReadInt32()
            $fileCount = Decode-PackInt $reader.ReadInt32() $key
            $extracted = 0
            $skipped = 0

            for ($i = 0; $i -lt $fileCount; $i++) {
                $headerOffset = 16 + (256 * $i)
                $stream.Seek($headerOffset, [System.IO.SeekOrigin]::Begin) | Out-Null
                $nameLength = Decode-PackInt $reader.ReadInt32() $key
                if ($nameLength -lt 0 -or $nameLength -gt 96) {
                    Fail "Unexpected filename length in $label pack: $nameLength"
                }

                $chars = New-Object char[] $nameLength
                for ($j = 0; $j -lt $nameLength; $j++) {
                    $chars[$j] = Decode-PackTChar $reader.ReadUInt16() $key
                }
                $relative = Normalize-PackPath (-join $chars)

                $stream.Seek(256 * ($i + 1), [System.IO.SeekOrigin]::Begin) | Out-Null
                $fileSize = Decode-PackInt $reader.ReadInt32() $key
                $fileOffset = Decode-PackInt $reader.ReadInt32() $key
                if ($fileSize -lt 0 -or $fileOffset -lt 0) {
                    Fail "Unexpected file entry in $label pack: $relative"
                }

                $target = Get-WebSoundTargetPath $targetRoot $relative $label
                if ((Test-Path -LiteralPath $target) -and
                    ((Get-Item -LiteralPath $target).LastWriteTimeUtc -ge $packItem.LastWriteTimeUtc)) {
                    $skipped++
                    continue
                }

                $targetDirectory = Split-Path -Parent $target
                New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null
                $stream.Seek($fileOffset, [System.IO.SeekOrigin]::Begin) | Out-Null
                $data = $reader.ReadBytes($fileSize)
                if ($data.Length -ne $fileSize) {
                    Fail "Could not read $relative from $label pack."
                }
                if ($label -eq "BGM" -and [System.IO.Path]::GetExtension($relative).ToLowerInvariant() -eq ".wav") {
                    Convert-WebBgmWavBytes $data $target
                }
                else {
                    [System.IO.File]::WriteAllBytes($target, $data)
                }
                $extracted++
            }

            Write-Host "Web $label sound assets: extracted $extracted file(s), skipped $skipped up-to-date file(s)."
        }
        finally {
            $reader.Dispose()
        }
    }
    finally {
        $stream.Dispose()
    }
}

function Generate-WebSoundAssets([string]$soundRoot, [string]$targetRoot) {
    Expand-WebSoundPack (Join-Path $soundRoot "data1") $targetRoot "SE"
    Remove-WebStaleBgmWav $targetRoot
    Expand-WebSoundPack (Join-Path $soundRoot "data2") $targetRoot "BGM"
}

function Write-WebGzipFile([string]$sourcePath, [string]$targetPath) {
    if (Test-Path -LiteralPath $targetPath) {
        Remove-Item -LiteralPath $targetPath -Force
    }

    $inputStream = [System.IO.File]::OpenRead($sourcePath)
    try {
        $outputStream = [System.IO.File]::Create($targetPath)
        try {
            $gzipStream = New-Object System.IO.Compression.GZipStream(
                $outputStream,
                [System.IO.Compression.CompressionLevel]::Optimal)
            try {
                $inputStream.CopyTo($gzipStream)
            }
            finally {
                $gzipStream.Dispose()
            }
        }
        finally {
            $outputStream.Dispose()
        }
    }
    finally {
        $inputStream.Dispose()
    }
}

function Split-WebFileIfNeeded([string]$sourcePath, [string]$chunkBaseName, [Int64]$maxChunkBytes) {
    $directory = Split-Path -Parent $sourcePath
    Get-ChildItem -LiteralPath $directory -File -Filter "$chunkBaseName.part*" -ErrorAction SilentlyContinue |
        Remove-Item -Force

    $sourceItem = Get-Item -LiteralPath $sourcePath
    if ($sourceItem.Length -le $maxChunkBytes) {
        return @($sourceItem.Name)
    }

    $chunkNames = New-Object System.Collections.Generic.List[string]
    $bufferLength = [int][Math]::Min($maxChunkBytes, 4MB)
    $buffer = New-Object byte[] $bufferLength
    $inputStream = [System.IO.File]::OpenRead($sourcePath)
    try {
        $index = 0
        while ($inputStream.Position -lt $inputStream.Length) {
            $chunkName = "$chunkBaseName.part$index"
            $chunkPath = Join-Path $directory $chunkName
            $chunkStream = [System.IO.File]::Create($chunkPath)
            try {
                $remaining = [Math]::Min($maxChunkBytes, $inputStream.Length - $inputStream.Position)
                while ($remaining -gt 0) {
                    $readTarget = [int][Math]::Min([Int64]$buffer.Length, $remaining)
                    $read = $inputStream.Read($buffer, 0, $readTarget)
                    if ($read -le 0) { break }
                    $chunkStream.Write($buffer, 0, $read)
                    $remaining -= $read
                }
            }
            finally {
                $chunkStream.Dispose()
            }
            $chunkNames.Add($chunkName)
            $index++
        }
    }
    finally {
        $inputStream.Dispose()
    }

    Remove-Item -LiteralPath $sourcePath -Force
    return $chunkNames.ToArray()
}

function Patch-WebDataLoader([string]$jsPath, [string]$remotePackageBase, [string[]]$compressedChunkNames, [Int64]$compressedSize) {
    if (-not (Test-Path -LiteralPath $jsPath)) {
        Fail "Generated JavaScript was not found: $jsPath"
    }

    $utf8NoBom = [System.Text.UTF8Encoding]::new($false)
    $content = [System.IO.File]::ReadAllText($jsPath, $utf8NoBom)

    $basePattern = "var\s+REMOTE_PACKAGE_BASE\s*=\s*['""]$([System.Text.RegularExpressions.Regex]::Escape($remotePackageBase))['""]\s*;"
    $baseMatch = [System.Text.RegularExpressions.Regex]::Match($content, $basePattern)
    if (-not $baseMatch.Success) {
        Fail "Could not find data package marker in generated JavaScript."
    }

    $chunkJson = ConvertTo-Json @($compressedChunkNames) -Compress
    $manifest = $baseMatch.Value +
        "Module[""dataFileCompressedChunks""]=Module[""dataFileCompressedChunks""]||{};" +
        "Module[""dataFileCompressedChunks""][REMOTE_PACKAGE_BASE]=$chunkJson;" +
        "Module[""dataFileCompressedSize""]=Module[""dataFileCompressedSize""]||{};" +
        "Module[""dataFileCompressedSize""][REMOTE_PACKAGE_BASE]=$compressedSize;"
    $content = $content.Remove($baseMatch.Index, $baseMatch.Length).Insert($baseMatch.Index, $manifest)

    $downloadPattern = "if\s*\(\s*!Module\s*\[\s*['""]dataFileDownloads['""]\s*\]\s*\)\s*Module\s*\[\s*['""]dataFileDownloads['""]\s*\]\s*=\s*\{\s*\}\s*;"
    $downloadMatch = [System.Text.RegularExpressions.Regex]::Match($content, $downloadPattern)
    if (-not $downloadMatch.Success) {
        Fail "Could not find data package download hook in generated JavaScript."
    }

    $compressedLoader = @'
        var compressedChunks = Module['dataFileCompressedChunks'] && (Module['dataFileCompressedChunks'][packageName] || Module['dataFileCompressedChunks'][packageName.split('/').pop()]);
        if (compressedChunks && compressedChunks.length) {
          if (typeof DecompressionStream === 'undefined') {
            throw new Error('This browser cannot decompress the compressed game data. Please use a current Chrome, Edge, Firefox, or Safari.');
          }
          if (!Module['dataFileDownloads']) Module['dataFileDownloads'] = {};
          var compressedSize = Module['dataFileCompressedSize'] && (Module['dataFileCompressedSize'][packageName] || Module['dataFileCompressedSize'][packageName.split('/').pop()]);
          var compressedLoaded = 0;
          var compressedParts = [];
          var basePath = packageName.lastIndexOf('/') >= 0 ? packageName.substring(0, packageName.lastIndexOf('/') + 1) : '';
          for (var chunkIndex = 0; chunkIndex < compressedChunks.length; ++chunkIndex) {
            var chunkName = compressedChunks[chunkIndex];
            var chunkUrl = basePath + chunkName;
            var chunkResponse = await fetch(chunkUrl);
            if (!chunkResponse.ok) {
              throw new Error(`${chunkResponse.status}: ${chunkResponse.url}`);
            }
            var chunkBytes = new Uint8Array(await chunkResponse.arrayBuffer());
            compressedParts.push(chunkBytes);
            compressedLoaded += chunkBytes.length;
            Module['dataFileDownloads'][chunkUrl] = { loaded: chunkBytes.length, total: chunkBytes.length };
            Module['setStatus'] && Module['setStatus'](`Downloading compressed data... (${compressedLoaded}/${compressedSize || compressedLoaded})`);
          }
          var compressedData = new Uint8Array(compressedLoaded);
          var compressedOffset = 0;
          for (var compressedPart of compressedParts) {
            compressedData.set(compressedPart, compressedOffset);
            compressedOffset += compressedPart.length;
          }
          Module['setStatus'] && Module['setStatus']('Decompressing data...');
          var decompressedResponse = new Response(new Blob([compressedData]).stream().pipeThrough(new DecompressionStream('gzip')));
          var decompressedBuffer = await decompressedResponse.arrayBuffer();
          if (packageSize && decompressedBuffer.byteLength !== packageSize) {
            throw new Error(`Decompressed data size mismatch: ${decompressedBuffer.byteLength}/${packageSize}`);
          }
          return decompressedBuffer;
        }

'@

    $content = $content.Remove($downloadMatch.Index, 0).Insert($downloadMatch.Index, $compressedLoader)
    [System.IO.File]::WriteAllText($jsPath, $content, $utf8NoBom)
}

function Patch-WebHtmlCacheBust([string]$targetRoot, [string]$outputName) {
    $htmlPath = Join-Path $targetRoot ($outputName + ".html")
    $jsPath = Join-Path $targetRoot ($outputName + ".js")
    if (-not (Test-Path -LiteralPath $htmlPath) -or -not (Test-Path -LiteralPath $jsPath)) { return }

    $utf8NoBom = [System.Text.UTF8Encoding]::new($false)
    $buildId = (Get-FileHash -LiteralPath $jsPath -Algorithm SHA256).Hash.Substring(0, 12).ToLowerInvariant()
    $content = [System.IO.File]::ReadAllText($htmlPath, $utf8NoBom)
    $scriptPattern = "src\s*=\s*(['""]?)$([System.Text.RegularExpressions.Regex]::Escape($outputName)).js(\?v=[^'"">\s]+)?\1"
    $content = [System.Text.RegularExpressions.Regex]::Replace($content, $scriptPattern, "src=""$outputName.js?v=$buildId""")

    if ($content -notmatch "locateFile\s*\(") {
        $modulePattern = "Module\s*=\s*\{"
        $wasmName = "$outputName.wasm"
        $locateFile = 'Module={locateFile(e,t){return e==="' + $wasmName + '"?t+e+"?v=' + $buildId + '":t+e},'
        $content = [System.Text.RegularExpressions.Regex]::Replace($content, $modulePattern, $locateFile, 1)
    }

    [System.IO.File]::WriteAllText($htmlPath, $content, $utf8NoBom)
}

function Finalize-WebDataPackage([string]$targetRoot, [string]$outputName) {
    $dataFileName = "$outputName.data"
    $dataPath = Join-Path $targetRoot $dataFileName
    if (-not (Test-Path -LiteralPath $dataPath)) { return }

    $gzipFileName = "$dataFileName.gz"
    $gzipPath = Join-Path $targetRoot $gzipFileName
    Get-ChildItem -LiteralPath $targetRoot -File -Filter "$dataFileName.gz*" -ErrorAction SilentlyContinue |
        Remove-Item -Force
    Write-WebGzipFile $dataPath $gzipPath

    $maxChunkBytes = 64MB
    $chunkNames = Split-WebFileIfNeeded $gzipPath $gzipFileName $maxChunkBytes
    $compressedSize = 0L
    foreach ($chunkName in $chunkNames) {
        $compressedSize += (Get-Item -LiteralPath (Join-Path $targetRoot $chunkName)).Length
    }

    Remove-Item -LiteralPath $dataPath -Force
    Patch-WebDataLoader (Join-Path $targetRoot "$outputName.js") $dataFileName $chunkNames $compressedSize

    Write-Host "Web data package: compressed $dataFileName to $([Math]::Round($compressedSize / 1MB, 2)) MB in $($chunkNames.Count) file(s)."
}

function Expand-ProjectMacro([string]$value) {
    $expanded = $value
    $expanded = $expanded.Replace('$(ProjectDir)', $root + '\')
    $expanded = $expanded.Replace('$(LuaRoot)', $LuaRoot.TrimEnd('\') + '\')
    $expanded = $expanded.Replace('$(LuabindRoot)', $LuabindRoot.TrimEnd('\') + '\')
    return $expanded
}

function Should-ExcludeSource([string]$relativePath) {
    $path = $relativePath -replace '/', '\'
    $excludes = @(
        '^source\\laug_th\.cpp$',
        '^source\\stdafx\.cpp$',
        '^source\\gameMainSystem\\patInput\.cpp$',
        '^source\\gameMainSystem\\Wiicon\\',
        '^source\\sound\\',
        '^source\\utility\\ATLcompati\\CString\.cpp$',
        '^source\\TexSeBgmFilepack\\DirectSound(.*)\.cpp$'
    )
    foreach ($exclude in $excludes) {
        if ($path -match $exclude) { return $true }
    }
    return $false
}

if (-not $ThirdPartyRoot) { $ThirdPartyRoot = (Resolve-Path -LiteralPath (Join-Path $root "..")).Path }
if (-not $LuaRoot) { $LuaRoot = Join-Path $ThirdPartyRoot "lua5.1" }
if (-not $LuabindRoot) { $LuabindRoot = Join-Path $ThirdPartyRoot "luabind-0.9.1" }

if (-not (Test-Path -LiteralPath (Join-Path $LuaRoot "include\lua.h"))) {
    Fail "Lua 5.1 headers were not found: $LuaRoot"
}
if (-not (Test-Path -LiteralPath (Join-Path $LuabindRoot "luabind\luabind.hpp"))) {
    Fail "luabind 0.9.1 headers were not found: $LuabindRoot"
}
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
if (-not (Test-BoostRoot $BoostRoot)) {
    Fail "Boost headers were not found. Pass -BoostRoot <path> or set BOOST_ROOT."
}

$outPath = Join-Path $root $OutDir
New-Item -ItemType Directory -Force -Path $outPath | Out-Null
$sourceLuabindRoot = $LuabindRoot
$webLuabindRoot = Join-Path $outPath "luabind-0.9.1-web"
Generate-WebLuabindTree $sourceLuabindRoot $webLuabindRoot
$LuabindRoot = $webLuabindRoot

$webSourceRoot = Join-Path $outPath "source-utf8"
Generate-WebSourceTree (Join-Path $root "source") $webSourceRoot

$projectPath = Join-Path $root "laug_th.vcxproj"
[xml]$project = Get-Content -LiteralPath $projectPath -Raw
$namespace = New-Object System.Xml.XmlNamespaceManager($project.NameTable)
$namespace.AddNamespace("msb", "http://schemas.microsoft.com/developer/msbuild/2003")

$sourceNodes = $project.SelectNodes("//msb:ClCompile[@Include]", $namespace)
$sources = New-Object System.Collections.Generic.List[string]
$excluded = New-Object System.Collections.Generic.List[string]
foreach ($node in $sourceNodes) {
    $include = $node.Include
    if (Should-ExcludeSource $include) {
        $excluded.Add($include)
        continue
    }

    $expanded = Expand-ProjectMacro $include
    if (-not [System.IO.Path]::IsPathRooted($expanded)) {
        $expanded = Join-Path $root $expanded
    }
    if (-not (Test-Path -LiteralPath $expanded)) {
        Fail "Source file from project was not found: $include -> $expanded"
    }
    $sources.Add((Convert-ToWebSourcePath $expanded (Join-Path $root "source") $webSourceRoot))
}

$assetRoot = Join-Path $outPath "assets"
$assetGraphicRoot = Join-Path $assetRoot "graphic"
$runtimeDataRoot = Join-Path $outPath "data"
$runtimeLanguageRoot = Join-Path $outPath "Language"
Generate-WebAssets (Join-Path $root "graphic") $assetGraphicRoot
Generate-WebRuntimeAssets (Join-Path $root "data") $runtimeDataRoot "data"
Generate-WebRuntimeAssets (Join-Path $root "Language") $runtimeLanguageRoot "Language"
Generate-WebSoundAssets (Join-Path $root "sound") $outPath
$sourceRsp = Join-Path $outPath "web-sources.rsp"
$buildRsp = Join-Path $outPath "web-build.rsp"
$excludedList = Join-Path $outPath "web-excluded-sources.txt"
$cObjectRoot = Join-Path $outPath "c-objects"

$includeDirs = @(
    (Join-Path $webSourceRoot "web_compat"),
    $webSourceRoot,
    (Join-Path $webSourceRoot "utility"),
    (Join-Path $webSourceRoot "gameMainSystem"),
    (Join-Path $webSourceRoot "gameMainSystem\filemanage"),
    (Join-Path $LuaRoot "include"),
    (Join-Path $LuaRoot "src"),
    $LuabindRoot,
    $BoostRoot
)

$flags = New-Object System.Collections.Generic.List[string]
$flags.Add("-std=gnu++11")
$flags.Add("-O3")
$flags.Add("-fexceptions")
$flags.Add("-DNDEBUG")
$flags.Add("-D_ARCHIVE")
$flags.Add("-DUNICODE")
$flags.Add("-D_UNICODE")
$flags.Add("-DBOOST_NO_RVALUE_REFERENCES")
$flags.Add("-DBOOST_ALL_NO_LIB")
foreach ($includeDir in $includeDirs) {
    $flags.Add("-I" + (Resolve-ProjectPath $includeDir))
}
$flags.Add("-s")
$flags.Add("WASM=1")
$flags.Add("-s")
$flags.Add("ALLOW_MEMORY_GROWTH=1")
$flags.Add("-s")
$flags.Add("FORCE_FILESYSTEM=1")
$flags.Add("-lidbfs.js")
$flags.Add("-s")
$flags.Add("DISABLE_EXCEPTION_CATCHING=0")
$flags.Add("--preload-file")
$flags.Add((Resolve-ProjectPath $assetRoot) + "@/assets")
$flags.Add("--preload-file")
$flags.Add((Resolve-ProjectPath $runtimeDataRoot) + "@/data")
$flags.Add("--preload-file")
$flags.Add((Resolve-ProjectPath $runtimeLanguageRoot) + "@/Language")
$flags.Add("-o")
$flags.Add((Join-Path $outPath ($OutputName + ".html")))

$cSources = New-Object System.Collections.Generic.List[string]
$cppSources = New-Object System.Collections.Generic.List[string]
foreach ($source in $sources) {
    if ([System.IO.Path]::GetExtension($source).ToLowerInvariant() -eq ".c") {
        $cSources.Add($source)
    }
    else {
        $cppSources.Add($source)
    }
}

$cObjects = New-Object System.Collections.Generic.List[string]
New-Item -ItemType Directory -Force -Path $cObjectRoot | Out-Null
foreach ($source in $cSources) {
    $objectName = [System.IO.Path]::GetFileNameWithoutExtension($source) + ".o"
    $cObjects.Add((Join-Path $cObjectRoot $objectName))
}

$sourceLines = $sources | ForEach-Object { Quote-ResponseArg $_ }
[System.IO.File]::WriteAllLines($sourceRsp, $sourceLines, [System.Text.Encoding]::ASCII)

$buildLines = New-Object System.Collections.Generic.List[string]
foreach ($flag in $flags) { $buildLines.Add((Quote-ResponseArg $flag)) }
foreach ($source in $cppSources) { $buildLines.Add((Quote-ResponseArg $source)) }
foreach ($object in $cObjects) { $buildLines.Add((Quote-ResponseArg $object)) }
[System.IO.File]::WriteAllLines($buildRsp, $buildLines, [System.Text.Encoding]::ASCII)
[System.IO.File]::WriteAllLines($excludedList, $excluded, [System.Text.Encoding]::ASCII)

Write-Host "Generated: $sourceRsp"
Write-Host "Generated: $buildRsp"
Write-Host "Source count: $($sources.Count)"
Write-Host "C source count: $($cSources.Count)"
Write-Host "Excluded platform source count: $($excluded.Count)"

if ($GenerateOnly) {
    Write-Host "GenerateOnly was specified; skipping em++ invocation."
    exit 0
}

$emcc = Get-Command "emcc" -ErrorAction SilentlyContinue
if (-not $emcc) {
    Fail "emcc was not found on PATH. Install/activate Emscripten, then rerun tools\build-web.ps1."
}

$emxx = Get-Command "em++" -ErrorAction SilentlyContinue
if (-not $emxx) {
    Fail "em++ was not found on PATH. Install/activate Emscripten, then rerun tools\build-web.ps1."
}

for ($i = 0; $i -lt $cSources.Count; $i++) {
    $source = $cSources[$i]
    $object = $cObjects[$i]
    $cArgs = New-Object System.Collections.Generic.List[string]
    $cArgs.Add("-O3")
    $cArgs.Add("-DNDEBUG")
    foreach ($includeDir in $includeDirs) {
        $cArgs.Add("-I" + (Resolve-ProjectPath $includeDir))
    }
    $cArgs.Add("-c")
    $cArgs.Add($source)
    $cArgs.Add("-o")
    $cArgs.Add($object)
    & $emcc.Source @cArgs
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

& $emxx.Source "@$buildRsp"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Finalize-WebDataPackage $outPath $OutputName
Patch-WebHtmlCacheBust $outPath $OutputName

Write-Host "Built: $(Join-Path $outPath ($OutputName + '.html'))"
