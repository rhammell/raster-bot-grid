# raster-bot-grid

`raster-bot-grid` is an Arduino sketch for the [Raster Bot](https://github.com/rhammell/raster-bot). It turns the bot's touchscreen into a drawable grid: tap adjacent cells to sketch a path, then the bot drives it. An on-board UI lets you undo the path, start/stop a run, and tweak display brightness, drive speed and drive distance.

## Compiling with the Arduino IDE

1. Install the Arduino IDE and the ESP32 board support package.
2. Install the **Raster_Bot** library so it can be found via `#include <Raster_Bot.h>`. Copy (or symlink) the `Raster_Bot` library from the [raster-bot](https://github.com/rhammell/raster-bot) project into your Arduino `libraries` folder (e.g. `~/Documents/Arduino/libraries/Raster_Bot`). Since the library is installed manually, also install its dependencies via the Library Manager: **Adafruit GFX Library**, **Adafruit ILI9341**, and **Adafruit FT6206 Library**.
3. Open `raster_bot_grid/raster_bot_grid.ino` in the IDE.
4. Select your ESP32 board and serial port.
5. Click **Upload** to compile and flash the firmware.

## Using the UI

When the robot powers up the display shows a grid and three buttons: **Undo**, **Start** and **Settings**.

1. **Draw a path** – Tap cells on the grid. Each new cell must be adjacent to the previous one. A default start cell is provided at the bottom centre of the grid.
2. **Undo** – Press the **Undo** button to clear the drawn path and return to the default two starting cells.
3. **Settings** – Tap **Settings** to adjust display brightness, drive speed and drive distance, then tap the button again to return to the grid.
4. **Start** – Press **Start** to begin a short countdown. Once the countdown reaches zero the robot will execute the path. While running the button changes to **Stop** and can be pressed to abort.

After the last point in the path is reached the button displays **Done!** and the robot returns to the idle state ready for a new path.
