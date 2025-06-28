# Single Digit Clock

This repository contains all the required files and resources for building a simple single-digit clock using a 7-segment display and a DS1302 real-time clock (RTC) module.

## Features

- Displays time using a single 7-segment display
- Uses DS1302 RTC for accurate timekeeping
- Includes Fritzing schematic and part files for easy hardware setup
- Arduino code provided for quick programming

## Folder Structure

- `7_Segment_RTC_Code/`  
  Arduino sketch for the clock (`sketch_jun28a.ino`)
- `RTC_for_fritzing/`  
  Fritzing part files and SVGs for the DS1302 RTC module
- `Schematic.fzz`  
  Fritzing schematic file
- `Schematic.zip`  
  Zipped schematic resources
- `arduino-ds1302-master.zip`  
  DS1302 Arduino library (if needed)

## Getting Started

1. **Hardware Setup:**  
   - Assemble the circuit using the provided Fritzing schematic (`Schematic.fzz`).
   - Use the DS1302 RTC and a 7-segment display as shown.

2. **Software Setup:**  
   - Install the DS1302 library (`arduino-ds1302-master.zip`) in your Arduino IDE.
   - Open `7_Segment_RTC_Code/sketch_jun28a.ino` in Arduino IDE.
   - Upload the code to your Arduino board.

3. **Fritzing:**  
   - Use the files in `RTC_for_fritzing/` to add the DS1302 part to your Fritzing project if needed.

## License

This project is licensed under the MIT License (see `LICENSE`).

The included DS1302 library is licensed under the GNU Lesser General Public License v2.1 (see `DS1302_LGPL_LICENSE`).

If you distribute this project, you must comply with the terms of both licenses.