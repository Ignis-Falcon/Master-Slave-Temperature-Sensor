#!/bin/bash

PROJECT_DIR=$(pwd)
NAME_PROJECT=$(basename "$PROJECT_DIR")
ELF_FILE_NAME="${NAME_PROJECT}.elf"
ELF_FILE_PATH="${PROJECT_DIR}/build/${ELF_FILE_NAME}"
PROGRAMMER_PATH="/opt/st/stm32cubeclt_1.21.0/STM32CubeProgrammer/bin/STM32_Programmer_CLI"

if [ -f "$ELF_FILE_PATH" ]; then
 $PROGRAMMER_PATH  -c port=SWD mode=UR -w "$ELF_FILE_PATH" -rst
else
  read -p "$ELF_FILE_NAME don't exist. Would do you compile source? (y/n): " choose

  if [ $choose == "y" ]; then
    make
  fi
fi
