# Cursed Clocks

This repository contains all the required files and resources for building a cursed clock using a DS1302 real-time clock (RTC) module.

## Features

- Displays time using a single 7-segment display or a NeoPixel ring
- Uses DS1302 RTC for accurate timekeeping
- Includes Fritzing schematic and part files for easy hardware setup
- Arduino code provided for both display types
- Python script for setting the RTC time

## Folder Structure

- `7_Segment_Clock/`
  - `Schematic.fzz`: Fritzing schematic file for the 7-segment clock
  - `7_Segment_RTC_Code/`
    - `7_Segment_RTC_Code.ino`: Arduino sketch for the 7-segment display clock
- `NEOPIXEL_Clock/`
  - `Schematic.fzz`: Fritzing schematic file for the NeoPixel clock
  - `NEOPIXEL_RTC_Code/`
    - `NEOPIXEL_RTC_Code.ino`: Arduino sketch for the NeoPixel ring clock
- `RTC_for_fritzing/`
  - Fritzing part files and SVGs for the DS1302 RTC module
- `set_time.py`: Python script to set the time on the DS1302 RTC module
- `arduino-ds1302-master.zip`: DS1302 Arduino library (if needed)
- `DS1302_LGPL_LICENSE`: License for the DS1302 library
- `LICENSE`: Project license
- `readme.md`: This file

## Getting Started

1. **Hardware Setup:**
   - Assemble the circuit using the provided Fritzing schematic (`7_Segment_Clock/Schematic.fzz` for 7-segment, `NEOPIXEL_Clock/Schematic.fzz` for NeoPixel).
   - Use the DS1302 RTC and the appropriate display as shown.

2. **Software Setup:**
   - Install the DS1302 library (`arduino-ds1302-master.zip`) in your Arduino IDE.
   - For 7-segment display: Open `7_Segment_Clock/7_Segment_RTC_Code/7_Segment_RTC_Code.ino` in Arduino IDE.
   - For NeoPixel ring: Open `NEOPIXEL_Clock/NEOPIXEL_RTC_Code/NEOPIXEL_RTC_Code.ino` in Arduino IDE.
   - Upload the code to your Arduino board.

3. **Set RTC Time (Optional):**
   - Use the `set_time.py` Python script to set the time on the DS1302 RTC module if needed. Make sure you have Python installed and the required serial libraries.

4. **Fritzing:**
   - Use the files in `RTC_for_fritzing/` to add the DS1302 part to your Fritzing project if needed.

## License

This project is licensed under the MIT License (see `LICENSE`).

The included DS1302 library is licensed under the GNU Lesser General Public License v2.1 (see `DS1302_LGPL_LICENSE`).

If you distribute this project, you must comply with the terms of both licenses.