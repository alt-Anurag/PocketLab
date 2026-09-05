# RP2040 Firmware — Scoppy

This folder is where the oscilloscope firmware for the Raspberry Pi Pico (RP2040) goes. Pocket Lab does **not** write its own DSO firmware — it uses the existing, open-source [Scoppy](https://github.com/fhdm-dev/scoppy) project as-is for the oscilloscope side.

## Download the Firmware

Scoppy firmware is distributed as a `.uf2` file directly by the Scoppy project — it is **not** built from source in this repo.

👉 **Download from the official Scoppy firmware page:**
[https://oscilloscope.fhdm.xyz/wiki/firmware-versions](https://oscilloscope.fhdm.xyz/wiki/firmware-versions)

## Pick the Correct Version

Make sure you grab the file that matches your exact board:

| Board | Firmware File |
|---|---|
| Raspberry Pi Pico | `scoppy-pico-v18.uf2` |
| Raspberry Pi Pico W | `scoppy-picow-v18.uf2` |
| Raspberry Pi Pico 2 | `scoppy-pico2-v19.uf2` |
| Raspberry Pi Pico 2 W | `scoppy-pico2w-v19.uf2` |

> Pocket Lab's original build used a standard Raspberry Pi Pico (RP2040) — use `scoppy-pico-v18.uf2` unless you've swapped in a different board.

## Flashing Instructions

1. Hold the **BOOTSEL** button on the Pico while plugging it into your computer via USB.
2. It will mount as a USB mass storage drive (named `RPI-RP2`).
3. Drag and drop the downloaded `.uf2` file onto that drive.
4. The Pico will reboot automatically, running Scoppy firmware.

For full setup after flashing (pairing with the Android app, wiring, etc.), refer to the [Scoppy Installation and Getting Started guide](https://oscilloscope.fhdm.xyz/wiki/Installation-&-Getting-Started).
