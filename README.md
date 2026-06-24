# grid-bot

Grid-bot is a two wheeled Arduino based robot that follows a user drawn path on a touchscreen. The project includes an on-board UI and is compiled with the standard Arduino IDE.

## Hardware requirements

* **Microcontroller** – ESP32 board (the firmware uses the ESP32 `FSPI` bus and the Raster Bot pin map).
* **Display** – 320×240 ILI9341 TFT display on `FSPI` (CS 17, DC 15, RST 16, SCLK 13, MOSI 14) with the backlight on pin 12.
* **Touchscreen** – FT6206 capacitive touch panel over I2C (SDA 21, SCL 18).
* **Motors** – Two DC drive motors connected through a motor driver (motor control is not yet implemented; the UI is display-only for now).
* **Power** – Suitable battery pack for the motors and microcontroller.

## Compiling with the Arduino IDE

1. Install the Arduino IDE and the ESP32 board support package.
2. Install the required libraries via the Library Manager:
   * **Adafruit GFX Library**
   * **Adafruit ILI9341**
   * **Adafruit FT6206 Library**
3. Install the **Raster_Bot** library so it can be found via `#include <Raster_Bot.h>`. Copy (or symlink) the `Raster_Bot` library from the [raster-bot](https://github.com/rhammell/raster-bot) project into your Arduino `libraries` folder (e.g. `~/Documents/Arduino/libraries/Raster_Bot`). The sketch drives the display and touch entirely through a `Raster_Bot` instance.
4. Open `grid_bot/grid_bot.ino` in the IDE.
5. Select your ESP32 board and serial port.
6. Click **Upload** to compile and flash the firmware.

## Using the UI

When the robot powers up the display shows a grid and three buttons: **Undo**, **Start** and **Settings**.

1. **Draw a path** – Tap cells on the grid. Each new cell must be adjacent to the previous one. A default start cell is provided at the bottom centre of the grid.
2. **Undo** – Press the **Undo** button to clear the drawn path and return to the default two starting cells.
3. **Settings** – Tap **Settings** to adjust display brightness, drive speed and drive distance, then tap the button again to return to the grid.
4. **Start** – Press **Start** to begin a short countdown. Once the countdown reaches zero the robot will execute the path. While running the button changes to **Stop** and can be pressed to abort.

After the last point in the path is reached the button displays **Done!** and the robot returns to the idle state ready for a new path.
