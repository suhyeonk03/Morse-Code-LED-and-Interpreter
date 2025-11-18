# Morse-Code-LED-and-Interpreter
## Project Overview
Raspberry PI4 converts stdin input (string) into morse code via LED. And photodiode connected to ESP32 reads condition of LED (ON or OFF) to convert morse code into the string. 

<p align="center">
  <img src="https://github.com/suhyeonk03/Morse-Code-LED-and-Interpreter/blob/main/i1.jpg" alt="Hardware Implementation" width="300" height="250">
  <img src="https://github.com/suhyeonk03/Morse-Code-LED-and-Interpreter/blob/main/i2.jpg" alt="Sender (Raspberry PI4)" width="300" height="250">
  <img src="https://github.com/suhyeonk03/Morse-Code-LED-and-Interpreter/blob/main/i3.jpg" alt="Receiver (ESP32)" width="300" height="250">
</p>

## Used Libraries (C, ESP32)
- <stdio.h>
- <string.h>
- "esp_adc/adc_oneshot.h"
- "freertos/FreeRTOS.h"
- "freertos/task.h"

## Used Libraries (Python, Raspberry PI4) 
- sys
- time
- RPi.GPIO

## Hardware
- ESP32-C3-DevKit-RUST-1
- Raspberry PI4
- Breadboard, wires, resistors (330, 10k ohms), LED, photodiode, female header pins, and male header pins

## Phase 1: String to Morse Code (STDIN to LED)

## Phase 2: Morse Code to String (LED to STDOUT)
