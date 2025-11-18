# Morse-Code-LED-and-Interpreter
## Project Overview
The Raspberry Pi 4 converts stdin input (a string) into Morse code using an LED. A photodiode connected to the ESP32 detects the LED’s ON/OFF state and converts the Morse signal back into a string.

<p align="center">
  <img src="https://github.com/suhyeonk03/Morse-Code-LED-and-Interpreter/blob/main/i1.jpg" alt="Hardware Implementation" width="350" height="300">
  <img src="https://github.com/suhyeonk03/Morse-Code-LED-and-Interpreter/blob/main/Receiver.jpg" alt="Receiver (ESP32)" width="300" height="250">
  <img src="https://github.com/suhyeonk03/Morse-Code-LED-and-Interpreter/blob/main/i3.jpg" alt="Terminal executing python code" width="350" height="300">
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
- Raspberry Pi 4
- Breadboard, wires, resistors (330, 10k ohms), LED, photodiode, female header pins, and male header pins

## Phase 1: Hardware Implementation
Raspberry Pi side: GPIO 17 -------> Resistor (330 ohm) -------> LED -------> GND
ESP32 side: 3v3 -------> Photodiode (-) -------> Photodiode (+) -------> GPIO 3 (ADC1-3), Resistor (10k ohm) -------> GND

## Phase 2: String to Morse Code (STDIN to LED)
I represented Morse code using an LED connected to a Raspberry Pi 4. Based on the datasheet, I chose GPIO pin 17. I configured the GPIO using GPIO.setmode() and GPIO.setup(). Then, using GPIO.output() and time.sleep(), I transformed stdin input into Morse code based on a custom morse_table.

## Phase 3: Morse Code to String (LED to STDOUT)
I used an ESP32 with a photodiode to read the Morse code transmitted via LED and translate it back into text. The photodiode detects whether the LED is ON or OFF through an ADC channel. Based on the duration of LED state changes, the ESP32 distinguishes between dots, dashes, letter gaps, word gaps, and line endings. Each symbol is stored in a morse_buffer, and when a letter gap is detected, the contents of the buffer are translated into a character using a morse_table. These characters are appended to a string array, and once a line-ending duration is detected, the full decoded string is printed.
