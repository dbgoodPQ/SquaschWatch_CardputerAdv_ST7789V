# SquaschWatch_CardputerAdv_ST7789V

A fork of **[SquachWatch-CYD](https://github.com/skizzophrenic/SquachWatch-CYD)** by
**[skizzophrenic](https://github.com/skizzophrenic)**, adapted for one setup only: the
**M5Stack Cardputer ADV** with a **Waveshare 2" LCD module (ST7789V, 240x320)** as its external
screen.

Everything about the firmware itself — detections, Squachy, menus, SquachMesh, keyboard
controls — is the original author's work and is unchanged. For what SquachWatch does and how to use
it, read the [upstream README](https://github.com/skizzophrenic/SquachWatch-CYD#readme). This page
only lists what differs.

Two builds are provided:

| Build | Built-in 1.14" screen | Waveshare 2" ST7789V |
| --- | --- | --- |
| **`cardputer-adv`** (dual) | menus, keyboard cursor | Squachy's main scene (turn on in Settings → EXT SCREEN) |
| **`cardputer-adv-extui`** (EXT UI) | off | **the whole interface** at 320x240, navigated with the keyboard cursor |

## What changed

| | Upstream `cardputer-adv` | This fork |
| --- | --- | --- |
| External screen driver | 2.8" ILI9341 (320x240) | Waveshare 2" **ST7789V** (240x320 IPS, driven as 320x240 landscape) |
| Init sequence | ILI9341 | ST7789 (same as Arduino_GFX `Arduino_ST7789`), inversion on for IPS |
| SPI | 27 MHz, mode 0 | 40 MHz, mode 3 (as in [LauncherDual](https://github.com/dbgoodPQ/LauncherDual)) |
| External backlight | wired to 5 V, always on | **G4**, software-controlled: follows BRIGHTNESS and dims with the screen, off when EXT SCREEN is OFF |
| `EXT ROT` console command | cycles ILI9341 orientations | cycles ST7789 landscape orientations |
| Full UI on the external screen | — | new `cardputer-adv-extui` build: TFT_eSPI drives the ST7789V as the main display (CYD 320x240 layouts) with keyboard cursor navigation; built-in screen off |
| Default build (`pio run`) | the CYD boards | `cardputer-adv` and `cardputer-adv-extui` |
| CI | builds the CYD family | one GitHub Action that builds only the two Cardputer ADV builds and uploads their binaries |

| Keyboard cursor | stops on the main screen's buttons, lists, a few dialogs; a coarse grid elsewhere | stops on every tap target of every screen; typing on text screens; ESC cannot leave the lock screen |

Code changes: the `EXT SCREEN` block, `CARDPUTER_EXTUI` guards and the keyboard cursor in
`src/main.cpp`; a small `…Stops()` function in the UI files whose screens keep their own button
rectangles; the PIN pad's keyboard path in `src/ui_phone.cpp`; the new
`include/cardputer_adv_extui_user_setup.h`; and the two Cardputer ADV sections of `platformio.ini`.

## EXT UI build: keyboard navigation

The ST7789V has no touch panel, so the EXT UI build uses the same keyboard cursor the built-in screen
uses in upstream. Every screen is the 320x240 layout of the 2.8" CYD, and the cursor stops on every tap
target each screen draws: on the main screen that includes the message bubble, the watch pill, the
squad badge, each visiting Squachy and NEARBY; elsewhere the stops come from each screen's own hit
test, so they sit exactly on its buttons and rows.

| Key | Action |
| --- | --- |
| `↓` `→` or `TAB` | next stop (the first press only shows the cursor) |
| `↑` `←` | previous stop |
| `ENTER` or `SPACE` | press the stop; hold it for a long press (LOG and SCAN rows, NEARBY) |
| `ESC` or `DEL` | back to the main screen (not from the lock screen) |

The arrows are the Cardputer's `;` `.` `,` `/` keys. The cursor (amber brackets) hides after 5 seconds
without a key; the arrows at the right edge of a list scroll it.

**Text screens** (WiFi password, Squachy's name, messages, PIN pads) type from the keyboard instead:

| Key | Action |
| --- | --- |
| letters, digits, symbols | typed as printed; `SHIFT` for the upper character |
| `DEL` | delete |
| `ENTER` | OK — or, while the cursor is showing, press the cursor's stop |
| `FN` + `;` `.` `,` `/`, or `TAB` | move the cursor (to reach BACK, SHOW…) |
| `ESC` | back to the main screen |

The screen is fixed in landscape; build with `-DEXTUI_ROTATION=3` to turn it 180°. Pins for this build
are in `include/cardputer_adv_extui_user_setup.h`.

## Pinout (Cardputer ADV EXT header → Waveshare 2" module)

| Waveshare pin | Cardputer ADV | Notes |
| --- | --- | --- |
| DIN (MOSI) | G14 | shared with the SD card |
| CLK (SCK) | G40 | shared with the SD card |
| CS | G5 | |
| DC | G6 | |
| RST | G3 | |
| BL | G4 | backlight, software-controlled |
| VCC | 3V3 | set the module's voltage selector to **3.3V** |
| GND | GND | |

Same wiring as the LauncherDual project. Every pin can be changed with the `-DEXT_TFT_*` flags in
`[env:cardputer-adv]` of `platformio.ini`. If BL is wired to 3V3 instead of G4, set `-DEXT_TFT_BL=-1`
(the backlight will then always be on).

Optional flags in the same section:

- `-DEXT_SPI_FREQ=20000000` — lower the SPI clock if the picture is noisy (long wires).
- `-DEXT_MADCTL=0xA0` — turn the picture 180°. The `EXT ROT` serial command tries the orientations
  live and prints the value to use.

## Using it

- **Dual build:** turn it on in **Settings → EXT SCREEN → ON** (saved across reboots). Squachy then
  runs on the external screen and the built-in one keeps the menus, as in upstream.
- **EXT UI build:** nothing to turn on; everything is on the external screen from boot.

## Build and flash

Download the firmware from the **Actions** tab → *Build Cardputer ADV (ST7789V)* → latest run →
artifacts: `SquachWatch-CardputerAdv-ST7789V-dual-…` or `SquachWatch-CardputerAdv-ST7789V-extui-…`.
Each contains a merged image and the separate parts, with `FLASHING.txt`.

```
# fresh install (erases saved settings)
esptool.py --chip esp32s3 write_flash 0x0 SquachWatch-CardputerAdv-ST7789V-extui-merged.bin
```

Download mode: hold **G0** while pressing **RST**.

Or build it yourself:

```
git clone https://github.com/dbgoodPQ/SquaschWatch_CardputerAdv_ST7789V
cd SquaschWatch_CardputerAdv_ST7789V
pio run -e cardputer-adv-extui -t upload   # or -e cardputer-adv for the dual build
```

## Credits

- **SquachWatch-CYD** and everything in it: **[skizzophrenic](https://github.com/skizzophrenic)** and
  the contributors listed in the
  [upstream credits](https://github.com/skizzophrenic/SquachWatch-CYD#credits).
- ST7789 init sequence: [Arduino_GFX](https://github.com/moononournation/Arduino_GFX) by
  **moononournation**.
- Waveshare 2" ST7789V wiring on the Cardputer ADV: [LauncherDual](https://github.com/dbgoodPQ/LauncherDual).

## License

**GPL-3.0**, as upstream. See [LICENSE](LICENSE).
