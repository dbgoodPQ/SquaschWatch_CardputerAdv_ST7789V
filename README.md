# SquaschWatch_CardputerAdv_ST7789V

A fork of **[SquachWatch-CYD](https://github.com/skizzophrenic/SquachWatch-CYD)** by
**[skizzophrenic](https://github.com/skizzophrenic)**, adapted for one setup only: the
**M5Stack Cardputer ADV** with a **Waveshare 2" LCD module (ST7789V, 240x320)** as its external
screen.

Everything about the firmware itself — detections, Squachy, menus, SquachMesh, keyboard
controls — is the original author's work and is unchanged. For what SquachWatch does and how to use
it, read the [upstream README](https://github.com/skizzophrenic/SquachWatch-CYD#readme). This page
only lists what differs.

## What changed

| | Upstream `cardputer-adv` | This fork |
| --- | --- | --- |
| External screen driver | 2.8" ILI9341 (320x240) | Waveshare 2" **ST7789V** (240x320 IPS, driven as 320x240 landscape) |
| Init sequence | ILI9341 | ST7789 (same as Arduino_GFX `Arduino_ST7789`), inversion on for IPS |
| SPI | 27 MHz, mode 0 | 40 MHz, mode 3 (as in [LauncherDual](https://github.com/dbgoodPQ/LauncherDual)) |
| External backlight | wired to 5 V, always on | **G4**, software-controlled: follows BRIGHTNESS and dims with the screen, off when EXT SCREEN is OFF |
| `EXT ROT` console command | cycles ILI9341 orientations | cycles ST7789 landscape orientations |
| Default build (`pio run`) | the CYD boards | `cardputer-adv` only |
| CI | builds the CYD family | one GitHub Action that builds only `cardputer-adv` and uploads the binaries |

Code changes are limited to the `EXT SCREEN` block in `src/main.cpp` (inside `#if defined(CARDPUTER_ADV)`)
and the `[env:cardputer-adv]` section of `platformio.ini`. The built-in 1.14" screen is untouched.

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

Turn it on in **Settings → EXT SCREEN → ON** (saved across reboots). Squachy then runs on the
external screen and the built-in one keeps the menus, as in upstream.

## Build and flash

Download the firmware from the **Actions** tab → *Build Cardputer ADV (ST7789V)* → latest run →
artifact. It contains a merged image and the separate parts, with `FLASHING.txt`.

```
# fresh install (erases saved settings)
esptool.py --chip esp32s3 write_flash 0x0 SquachWatch-CardputerAdv-ST7789V-merged.bin
```

Download mode: hold **G0** while pressing **RST**.

Or build it yourself:

```
git clone https://github.com/dbgoodPQ/SquaschWatch_CardputerAdv_ST7789V
cd SquaschWatch_CardputerAdv_ST7789V
pio run -e cardputer-adv -t upload
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
