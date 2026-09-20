# Exit immediately if any command fails
$ErrorActionPreference = "Stop"

$VenvDir = "venv"
$RequirementsFile = "requirements.txt"
$LibName = "libdsp.dll"  # Windows uses DLLs instead of .so

# Relative paths to your C code
$IncDir = "..\Nucleo Code\Inc"
$SrcDir = "..\Nucleo Code\Src"

$CSrcs = "$SrcDir\tremolo.c", "$SrcDir\audio_process.c"

Write-Host "=== 1. Checking Virtual Environment ===" -ForegroundColor Cyan
if ($env:VIRTUAL_ENV) {
    Write-Host "Already active in virtual environment: $env:VIRTUAL_ENV"
} else {
    if (-not (Test-Path -Path $VenvDir)) {
        Write-Host "Creating new virtual environment in .\'$VenvDir'..."
        python -m venv $VenvDir
    }
    Write-Host "Activating virtual environment from .\'$VenvDir'..."
    & ".\$VenvDir\Scripts\Activate.ps1"
}

Write-Host "`n=== 2. Installing Python Dependencies ===" -ForegroundColor Cyan
if (Test-Path -Path $RequirementsFile) {
    python -m pip install --upgrade pip
    pip install -r $RequirementsFile
} else {
    Write-Host "Warning: $RequirementsFile not found. Installing base dependencies..."
    pip install pytest cffi numpy
}

Write-Host "`n=== 3. Compiling C Shared Library ($LibName) ===" -ForegroundColor Cyan
foreach ($file in $CSrcs) {
    if (-not (Test-Path -Path $file)) {
        Write-Error "Error: Source file '$file' not found!"
        exit 1
    }
}

# Compile DLL on Windows
gcc -O2 -shared -I"$IncDir" $CSrcs -o "$LibName"
Write-Host "Successfully compiled $LibName" -ForegroundColor Green

Write-Host "`n=== 4. Running Pytest Suite ===" -ForegroundColor Cyan
if (Test-Path -Path "test_pipeline.py") {
    pytest -v test_pipeline.py
} else {
    Write-Error "Error: test_pipeline.py not found in current directory!"
    exit 1
}