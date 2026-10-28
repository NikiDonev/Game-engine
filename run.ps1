param(
    [string]$Preset = "debug",
    [switch]$NoRun
)

$code = 0
Push-Location $PSScriptRoot
try {
    cmake --workflow --preset $Preset
    $code = $LASTEXITCODE

    if ($code -eq 0 -and -not $NoRun) {
        $exeName = if ($env:OS -eq "Windows_NT") { "mygame.exe" } else { "mygame" }
        $exe = Join-Path $PSScriptRoot "out/build/$Preset/$exeName"
        if (Test-Path $exe) {
            & $exe
            $code = $LASTEXITCODE
        } else {
            Write-Error "Executable not found at $exe"
            $code = 1
        }
    }
}
finally {
    Pop-Location
}
exit $code
