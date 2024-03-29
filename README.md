# Arduino Weather Station with SSD1306 Display and BME280 Sensor

This Arduino project creates a compact weather station that reads temperature, humidity, and pressure using a BME280 sensor and displays the information on an SSD1306 OLED display. It includes a button to toggle between display modes and employs a CoRTOS scheduler for task management.

## Hardware Requirements

- Arduino Uno/Nano/Mega or any compatible board
- BME280 sensor module for temperature, humidity, and pressure
- SSD1306 OLED display (128x32 pixels or similar)
- Push button
- Connecting wires

## Software Dependencies

Make sure to install the following libraries through the Arduino Library Manager:

- `Adafruit_BME280`
- `SSD1306Ascii`
- `SSD1306AsciiWire`
- `CoRTOS` Link to repo: https://github.com/ksaeed7/CoRTOS
- `Wire`

## Wiring Instructions

1. **BME280 Sensor to Arduino**: Connect VCC to 3.3V, GND to GND, SCL to A5, and SDA to A4.
2. **SSD1306 Display to Arduino**: Connect VCC to 3.3V or 5V (depending on your display's voltage), GND to GND, SCL to A5, and SDA to A4.
3. **Button to Arduino**: Connect one pin to digital pin 2 and the other pin to GND.

## Features

- Real-time measurement and display of temperature, humidity, pressure, and altitude.
- Weather prediction based on pressure changes with indicators for rising, falling, or steady conditions.
- Toggle between detailed data display and simple weather condition indicators with a button.
- Auto-disable display after a set period of inactivity to conserve energy.
- Debug support for easier troubleshooting.

## Setup

1. Connect all hardware components as per the wiring instructions.
2. Install all required libraries using the Arduino IDE's Library Manager.
3. Upload the provided code to your Arduino board.
4. Press the button to toggle between display modes or reset the idle timer.

## Usage

The weather station will automatically begin displaying sensor data on the OLED screen after startup. Press the button to switch between the detailed sensor data view and the simplified weather condition view. The display will turn off automatically after a period of inactivity to save power. Press the button to wake it up again.

## Troubleshooting

- If the sensor is not detected, check the wiring and ensure that the correct I2C address is used in the code.
- For display issues, verify the OLED display's connections and confirm its I2C address matches the one in the code.
- If the button does not function, ensure it's properly connected to pin 2 and GND.


