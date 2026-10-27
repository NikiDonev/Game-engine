param(
    [ValidateSet("debug", "release")]
    [string]$Config = "debug"
)

cmake --preset $Config
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build --preset $Config
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$exe = Join-Path $PSScriptRoot "out\build\x64-$Config\mygame.exe"
if (-not (Test-Path $exe)) {
    Write-Error "Executable not found at $exe. Check your CMake runtime output directory."
    exit 1
}

& $exe
