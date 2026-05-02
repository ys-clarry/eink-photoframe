# eink-server-lovyan

A web-based image display system for e-ink paper using ESP32 and LovyanGFX.

## Overview

This project enables remote image display on e-ink screens connected to an ESP32 development board through a web interface. It leverages the LovyanGFX graphics library for optimal e-ink rendering.

## Features

- Web interface for image upload and display
- E-ink screen support via LovyanGFX library
- ESP32-based server
- Real-time image streaming to specific e-ink models
- Responsive design

## Hardware Requirements

- ESP32 Development Kit ([waveshare](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/index.html))
- Compatible e-ink display (model supported by LovyanGFX)
  - This code is hardly wired with 7.8-inch greyscale with IT8951 HAT ([waveshare](https://www.waveshare.com/7.8inch-e-paper-hat.htm))
  - IT8951 is driven via SPI/GPIO. See code for pin assignment.
- USB cable for programming and power

## Software Requirements

- PlatformIO
- LovyanGFX library
- ESP32 Arduino framework

## Installation

1. Clone or open this project in PlatformIO
2. Install dependencies (LovyanGFX)
3. Configure your e-ink display model in the code
4. Build and upload to your ESP32

## Usage

1. Power on the ESP32
2. Connect to the web interface
3. Upload an image
4. View it rendered on your e-ink display

## Configuration

Update display settings in the main source file to match your specific e-ink model.

## License

MIT; Helped much with ChatGPT and GitHub copilot.