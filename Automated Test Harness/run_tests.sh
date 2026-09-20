#!/usr/bin/env bash

# Exit immediately if a command exits with a non-zero status
set -e

VENV_DIR="venv"
REQUIREMENTS_FILE="requirements.txt"
LIB_NAME="libdsp.so"

# Relative paths pointing to your Nucleo Code directories
INC_DIR="../Nucleo Code/Inc"
SRC_DIR="../Nucleo Code/Src"

C_SRCS="${SRC_DIR}/tremolo.c ${SRC_DIR}/audio_pipeline.c"

echo "=== 1. Checking Virtual Environment ==="
if [ -n "$VIRTUAL_ENV" ]; then
    echo "Already active in virtual environment: $VIRTUAL_ENV"
else
    if [ ! -d "$VENV_DIR" ]; then
        echo "Creating new virtual environment in ./${VENV_DIR}..."
        python3 -m venv "$VENV_DIR"
    fi
    echo "Activating virtual environment from ./${VENV_DIR}..."
    source "$VENV_DIR/bin/activate"
fi

echo ""
echo "=== 2. Installing Python Dependencies ==="
if [ -f "$REQUIREMENTS_FILE" ]; then
    pip install --upgrade pip
    pip install -r "$REQUIREMENTS_FILE"
else
    echo "Warning: $REQUIREMENTS_FILE not found. Installing base test dependencies..."
    pip install pytest cffi numpy
fi

echo ""
echo "=== 3. Compiling C Shared Library (${LIB_NAME}) ==="
# Verify C source files exist before compilation
for src_file in $C_SRCS; do
    if [ ! -f "$src_file" ]; then
        echo "Error: Source file '$src_file' not found!"
        exit 1
    fi
done

# -I"${INC_DIR}": Includes header files from Nucleo Code/inc
# Quotes handle spaces in directory names ("Nucleo Code")
gcc -O2 -shared -fPIC -I"${INC_DIR}" $C_SRCS -o "${LIB_NAME}" -lm
echo "Successfully compiled ${LIB_NAME} inside $(pwd)"

echo ""
echo "=== 4. Running Pytest Suite ==="
if [ -f "test_pipeline.py" ]; then
    pytest -v test_pipeline.py
else
    echo "Error: test_pipeline.py not found in current directory!"
    exit 1
fi