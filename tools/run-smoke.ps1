param(
    [int]$Seconds = 8,
    [switch]$KeepRunning
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$exe = Join-Path $root "Debug\ggn_d.exe"
if (-not (Test-Path -LiteralPath $exe)) {
    throw "Debug executable not found: $exe. Run tools\build-debug.ps1 first."
}

$workDir = Split-Path -Parent $exe
$process = Start-Process -FilePath $exe -WorkingDirectory $workDir -PassThru
Start-Sleep -Seconds $Seconds
$process.Refresh()
if ($process.HasExited) {
    throw "ggn_d.exe exited during the smoke window. ExitCode=$($process.ExitCode)"
}

Write-Host "ggn_d.exe stayed alive for $Seconds seconds. PID=$($process.Id)"
if (-not $KeepRunning) {
    Stop-Process -Id $process.Id -Force
    Write-Host "Stopped smoke-test process. Use -KeepRunning to leave the game open."
}
