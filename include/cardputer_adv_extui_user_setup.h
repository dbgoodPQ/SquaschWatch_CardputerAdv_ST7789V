// SquaschWatch_CardputerAdv_ST7789V -- TFT_eSPI user setup for the
// Cardputer ADV "EXT UI" build: the whole interface on a Waveshare 2" LCD
// module (ST7789V, 240x320 IPS) wired to the EXT header, driven as the main
// display in landscape (320x240), navigated from the keyboard exactly as the
// built-in screen is. The built-in 1.14" screen is not used (backlight off).
//
// Wiring (same as LauncherDual):
//   DIN/MOSI G14, CLK/SCK G40 (both shared with the SD card's wires),
//   CS G5, DC G6, RST G3, BL G4 (main.cpp's LEDC dimmer), VCC 3V3, GND.
#pragma once

#define USER_SETUP_INFO    "SquachWatch / Cardputer ADV / EXT UI on Waveshare 2in ST7789V 240x320"
#define ST7789_DRIVER
#define TFT_WIDTH   240
#define TFT_HEIGHT  320

#define TFT_MISO  -1
#define TFT_MOSI  14
#define TFT_SCLK  40
#define TFT_CS    5
#define TFT_DC    6
#define TFT_RST   3
// The module's backlight, run by main.cpp's LEDC dimmer (BL_PIN_S3).
#ifndef EXTUI_TFT_BL
#define EXTUI_TFT_BL  4
#endif
// 1 = landscape as LauncherDual shows it; 3 = turned 180 degrees.
#ifndef EXTUI_ROTATION
#define EXTUI_ROTATION 1
#endif

// IPS panel: main.cpp's PANEL_NEEDS_INVERSION (true on the Cardputer) is what
// actually sets it; Settings > INVERT flips it if wrong.
#define TFT_INVERSION_ON
// RGB, as Arduino_GFX drives this module in LauncherDual. Settings has a
// colour-order swap if red and blue come out exchanged.
#define TFT_RGB_ORDER TFT_RGB

#ifndef SPI_FREQUENCY
#define SPI_FREQUENCY         40000000
#endif
#define SPI_READ_FREQUENCY    16000000

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define SMOOTH_FONT
