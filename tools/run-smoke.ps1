param(
    [int]$Seconds = 8,
    [string]$ExePath,
    [switch]$KeepRunning
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
if (-not $ExePath) {
    $ExePath = Join-Path $root "Debug\ggn_d.exe"
}

$resolvedExe = Resolve-Path -LiteralPath $ExePath -ErrorAction SilentlyContinue
if (-not $resolvedExe) {
    throw "Executable not found: $ExePath. Run tools\build-debug.ps1 first, or pass -ExePath for an existing runtime."
}
$exe = $resolvedExe.Path
$workDir = Split-Path -Parent $exe

$process = Start-Process -FilePath $exe -WorkingDirectory $workDir -PassThru
Start-Sleep -Seconds $Seconds
$process.Refresh()
if ($process.HasExited) {
    throw "$(Split-Path -Leaf $exe) exited during the smoke window. ExitCode=$($process.ExitCode)"
}

Write-Host "$(Split-Path -Leaf $exe) stayed alive for $Seconds seconds. PID=$($process.Id)"
if (-not $KeepRunning) {
    Stop-Process -Id $process.Id -Force
    Write-Host "Stopped smoke-test process. Use -KeepRunning to leave it open."
}
