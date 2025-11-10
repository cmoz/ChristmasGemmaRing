# ChristmasGemmaRing
Wearable Christmas fun!

# Christmas Brooch with Touch Input

This project creates a festive Christmas brooch using the Adafruit Gemma M0 with touch input to cycle through different Christmas light sequences on NeoPixels.

## Features

- Touch-activated Christmas light sequences
- 5 different festive patterns:
  1. **Twinkle Red/Green** - Alternates between red and green
  2. **Rainbow Chase** - Smooth rainbow color transitions
  3. **Breathing Gold** - Gentle gold breathing effect
  4. **Candy Cane Flash** - Red and white alternating flashes
  5. **Warm White Fade** - Cozy warm white fading

## Hardware Requirements

- Adafruit Gemma M0
- Built-in NeoPixel (or external NeoPixel strip connected to pin 1)
- Touch sensor or conductive material connected to pin A1/D2

## Wiring

- **NeoPixel**: Pin 1 (built-in on Gemma M0)
- **Touch Sensor**: Pin A1/D2
- **Power**: USB or battery pack

## Touch Sensor Setup

You can use various touch input methods:

1. **Capacitive Touch Pad**: Connect a piece of copper tape or conductive fabric to pin A1/D2
2. **Touch Sensor Module**: Connect a dedicated capacitive touch sensor
3. **Simple Wire**: Even a short wire can work as a basic touch sensor

## Software Setup

1. Install PlatformIO in VS Code
2. Open this project in PlatformIO
3. Build and upload to your Gemma M0

The required libraries will be automatically installed:
- Adafruit NeoPixel
- Adafruit FreeTouch

## Usage

1. Power on the Gemma M0
2. The built-in NeoPixel will flash white briefly to indicate startup
3. Touch the sensor to cycle through Christmas sequences
4. Each sequence runs for 10 seconds before automatically advancing
5. Touch again to manually advance to the next sequence

## Customization

### Adjusting Touch Sensitivity
Modify the `touchThreshold` variable in the code. Lower values = more sensitive.

### Adding More NeoPixels
Change `NUM_PIXELS` and connect an external NeoPixel strip to pin 1.

### Modifying Sequences
Each sequence function can be customized:
- Change timing by modifying the delay values
- Adjust colors by changing the RGB values
- Add new sequences by creating new functions and adding them to the enum

### Brightness Control
Adjust the brightness in `setup()` by changing `pixels.setBrightness(100)` (0-255).

## Troubleshooting

- **Touch not working**: Check wiring and adjust `touchThreshold`
- **NeoPixel not lighting**: Verify power and pin connections
- **Serial output**: Connect to Serial Monitor at 115200 baud for debug info

## Power Consumption

The project is designed to be battery-friendly with:
- Moderate brightness settings
- Efficient timing loops
- Low power delays

For extended battery life --> consider reducing brightness or adding sleep modes between sequences.
