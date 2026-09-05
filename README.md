# 🔬 Pocket Lab

**A pocket-sized DSO + Function Generator, built for students on a student's budget.**

Pocket Lab is a compact, low-power signal generation and measurement device that turns your Android smartphone into the display and processing unit for a digital storage oscilloscope (DSO) and function generator — combined into a single credit-card-sized board. It was built to bring lab-bench equipment within reach of students, cutting the cost of comparable devices by roughly **5–10x**.

> Built at **IICH Hackathon, Delhi Technological University (DTU), 2026.**

---

## The Problem

Every electronics student needs regular access to two instruments to actually learn the subject hands-on: a **function generator** (to produce test signals) and an **oscilloscope** (to observe them). In practice, most students don't get that access outside a scheduled lab slot, because:

- **Cost is the real barrier.** Even entry-level benchtop or USB-DSO options run from ~₹4,500 up to ₹25,000+ — out of reach for a personal, always-available unit for most students.
- **Bulk and low portability.** Standard DSOs and function generators are built as separate, deskbound instruments — not something a student carries around to experiment with outside the lab.
- **Shared-lab bottleneck.** With limited units and many students, hands-on practice time per student is scarce, which directly limits how much of the theory actually gets internalized.

India's electronics manufacturing base is also expanding quickly — the country's semiconductor industry alone represents a ~₹40,000 crore push — which means a growing need for engineers who are genuinely hands-on with signal generation and measurement, not just familiar with the theory. Pocket Lab exists to close that gap: give every student their **own** DSO + function generator, cheaply enough that "I don't have access to one" stops being a valid excuse.

---

## The Novelty — What Makes Pocket Lab Different

Pocket Lab isn't a smaller version of an existing DSO — it rethinks where the cost in a DSO actually comes from, and removes it:

1. **The smartphone *is* the display and processing unit.** The single biggest cost and bulk driver in any DSO is the dedicated screen + onboard processing to render waveforms. Pocket Lab eliminates that entirely by offloading it to a device every student already owns — their Android phone — via the open-source [Scoppy](https://github.com/fhdm-dev/scoppy) app. This alone is what makes the ~5–10x cost reduction possible, not just cheaper components.
2. **One microcontroller consolidation for the function generator.** Rather than needing separate analog circuits for each waveform shape (see [Previous Attempts](#previous-attempts--what-we-tried-before-this) below), a single ESP32 generates all four standard waveforms — sine, square, triangular, and sawtooth — in software. Fewer components, more precision, easier to tune.
3. **Credit-card form factor.** By moving both the display and most of the analog circuitry out of the physical unit, what's left fits in a footprint the size of a credit card — genuinely pocketable, not just "compact" by DSO standards.
4. **Real cost math, not a marketing number.** At ~₹1,500 in electronics (and an estimated ~₹3,000 landed cost including chassis, holder, buck converter, and battery), Pocket Lab lands at roughly **1/13th to 1/6th** the cost of comparable low-end DSOs on the market — see the comparison below.
5. **Built to get cheaper with scale, not just be cheap once.** As India's electronics manufacturing and engineering talent pool grows, the cost of a design like this is expected to fall further in production — the project is deliberately positioned to benefit from that trend rather than depend on one-off component sourcing.

### Cost & Spec Comparison

| Model | Frequency Range | Precision (Sampling / Resolution) | Cost (Approx. INR) | Size (Approx.) | Wireless |
|---|---|---|---|---|---|
| Miniware DS213 | 15 MHz | 100 MS/s, 8-bit | 20,000–25,000 | 100×56×10 mm | None (USB only) |
| Miniware DS211 | 200 kHz | 1 MS/s, 8-bit | 6,000–8,000 | 106×56×10 mm | None (USB only) |
| FNIRSI DSO-TC3 | 500 kHz | 10 MS/s, 10-bit | 4,500–5,500 | 103×79×31 mm | None |
| **Pocket Lab** | *(RP2040/Scoppy-limited — see Scoppy specs)* | *(RP2040/Scoppy-limited)* | **~1,500 (electronics) / ~3,000 (all-in)** | **Credit-card sized** | Via phone (Scoppy app) |

> Pocket Lab's frequency range and sampling precision are governed by the RP2040 + Scoppy firmware combination — if you need exact figures for your build, refer to [Scoppy's own specifications](https://github.com/fhdm-dev/scoppy).

---

## How It Works

Pocket Lab is built around two microcontrollers, each handling one half of the system:

```
                     ┌───────────────────────────┐
                     │      Pocket Lab Board       │
                     │                             │
   Function  ──────► │   ESP32 (WROOM-32 DevKit)   │──► Sine / Square /
   Generator          │   generates waveforms       │    Triangular / Sawtooth
                     │                             │
   Circuit Under ───► │  Raspberry Pi Pico (RP2040) │──► USB/Serial ──► Android Phone
   Test (e.g. diode,  │  samples signal, runs the   │              (Scoppy App = DSO)
   op-amp, RC, etc.)  │  Scoppy firmware            │
                     └───────────────────────────┘
```

- **Function Generator — ESP32 (WROOM-32 DevKit V1):**
  Generates sine, square, triangular, and sawtooth waveforms. This replaced our earlier discrete-component approach (NE555 timer for sawtooth, Arduino PWM + RC filtering for sine, and an LM358P op-amp integrator for triangular waves) — consolidating all waveform generation into a single microcontroller for better precision and flexibility.

- **Oscilloscope — Raspberry Pi Pico (RP2040):**
  Runs the [Scoppy](https://github.com/fhdm-dev/scoppy) firmware, sampling the circuit under test and streaming the data to the companion Android app, which renders it as a live oscilloscope trace — no dedicated display hardware required.

- **Test circuits explored during development** included a voltage divider, PN junction diode characteristic curves, and op-amp circuits — used to validate the DSO's measurement accuracy.

---

## Previous Attempts — What We Tried Before This

Before consolidating waveform generation into a single ESP32, each waveform shape was generated with its own dedicated analog/discrete circuit. These earlier approaches worked in isolation but didn't scale into one clean, tunable system — which is exactly the problem the current design solves. For context (and because the reasoning is worth documenting for anyone doing something similar):

| Waveform | Earlier Approach | How It Worked | Why It Didn't Scale |
|---|---|---|---|
| **Sawtooth** | NE555 Timer | Configured as a relaxation oscillator — linearly charges a capacitor, then discharges it rapidly to produce the ramp. | Fixed analog behavior, hard to precisely tune frequency/amplitude, and it's a separate physical circuit from every other waveform. |
| **Sine** | Arduino (PWM + RC filtering) | High-speed PWM pulses passed through a low-pass RC filter to smooth the digital switching into a sine-like shape. | Output quality depends heavily on filter tuning; not a clean or easily reconfigurable sine source. |
| **Square** | Raspberry Pi Pico (GPIO toggling) | Directly toggles GPIO pins between high and low states. | Worked well on its own, but still meant one more standalone circuit rather than a unified generator. |
| **Triangular** | LM358P Op-Amp (Integrator) | An integrator circuit transforms square waves into triangular signals through active linear voltage ramping. | Requires precise op-amp circuit tuning and adds another discrete analog stage. |

**The core issue** with this first approach: four waveforms meant four different circuits (a 555 timer, PWM+RC filtering, GPIO toggling, and an op-amp integrator), each with its own tuning quirks, board space, and component cost — working against the goal of a small, cheap, easily replicable device.

**Current improvement:** all four waveforms — sine, square, triangular, and sawtooth — are now generated from a **single ESP32 (WROOM-32 DevKit V1)**, replacing the NE555 timer and LM358P op-amp circuits entirely. This cut part count, simplified the board, and made waveform parameters (frequency, shape) tunable in firmware instead of by swapping analog components.

---

## Repository Structure

```
pocket-lab/
├── firmware/
│   ├── esp32/          # Function generator code (ESP32 WROOM-32)
│   └── rp2040/         # Scoppy-based DSO code (Raspberry Pi Pico)
├── hardware/
│   ├── schematic/      # Circuit schematics
│   └── images/         # Photos of the assembled circuit/PCB
├── docs/
│   └── presentations/  # Hackathon slide decks (IICH 2025, etc.)
└── README.md
```

> 📁 Update this tree if your actual folder names differ — this is meant as a map for anyone cloning the repo.

---

## Getting Started

### Hardware Required
- 1× ESP32 DevKit V1 (WROOM-32)
- 1× Raspberry Pi Pico (RP2040)
- Breadboard / PCB, jumper wires
- Basic passive components (resistors, capacitors) for the test circuits and voltage divider
- An Android smartphone (for the Scoppy app)
- USB cables for programming/power

### 1. Build the Circuit
Refer to the schematic in [`hardware/schematic`](./hardware/schematic) to wire up the ESP32 and RP2040 alongside the signal-conditioning circuitry. Reference photos of the assembled prototype are available in [`hardware/images`](./hardware/images).

### 2. Flash the RP2040 (Oscilloscope side)
1. Install the [Scoppy](https://github.com/fhdm-dev/scoppy) firmware on your Raspberry Pi Pico — follow the [Installation and Getting Started guide](https://oscilloscope.fhdm.xyz/wiki/Installation-&-Getting-Started) from the Scoppy project.
2. Flash the code from [`firmware/rp2040`](./firmware/rp2040) in this repo (our configuration/tuning on top of Scoppy).
3. Install the **Scoppy** app on your Android phone from the [Play Store](https://play.google.com/store/apps/details?id=xyz.fhdm.scoppy) (or sideload per the Scoppy repo instructions).

### 3. Flash the ESP32 (Function Generator side)
1. Open [`firmware/esp32`](./firmware/esp32) in the Arduino IDE or PlatformIO.
2. Select the correct board (ESP32 Dev Module / WROOM-32 DevKit V1).
3. Flash the code to generate sine, square, triangular, and sawtooth waveforms.

### 4. Connect and Test
1. Power up both microcontrollers.
2. Connect your RP2040 to your Android phone (per Scoppy's connection method — USB/serial).
3. Open the Scoppy app — it will display the live waveform as a DSO.
4. Use the ESP32 function generator output as your test signal, or probe an external circuit (diode, op-amp, RC network, etc.).

---

## Tech Stack

| Layer | Technology |
|---|---|
| Function Generation | ESP32 (WROOM-32 DevKit V1) |
| Signal Acquisition / DSO | Raspberry Pi Pico (RP2040) |
| DSO Display & UI | [Scoppy](https://github.com/fhdm-dev/scoppy) (open-source, Android) |
| Firmware Languages | C / C++ (Arduino framework / Pico SDK) |
| Hardware | Custom PCB / breadboard prototype |

---

## Roadmap

The current ESP32 + RP2040 setup is treated as a working proof of concept, not the final architecture. Planned next steps:

- [ ] **Move to a single STM32-based custom firmware**, replacing the ESP32 + RP2040 pairing, for greater scalability, reliability, and long-term performance headroom.
- [ ] **Design a custom PCB** — an initial schematic and PCB prototype have already been sketched — to replace the current breadboard/perfboard build.
- [ ] **Explore Simulink-based firmware development** as part of the STM32 transition, for more rigorous signal-processing design.
- [ ] Expand the test-circuit library (more standard lab experiments for students to run out of the box).
- [ ] Unlock further product differentiation once on custom hardware — better sampling precision, more waveform types, and a more polished companion app experience.

---

## Team

Built by **Team IICH 2025** at Delhi Technological University:

- **Anurag Kumar Jha** — Team Lead
- **Mohammed Adeeb Khan**
- **Abhishek**

---

## Acknowledgements

- [Scoppy](https://github.com/fhdm-dev/scoppy) — the open-source Android-oscilloscope project that made the display/UI side of this project possible. Full credit to its original developers; this project builds configuration and hardware integration around it, not a reimplementation of it.
- Delhi Technological University (DTU) and the IICH Hackathon 2025 for the platform to build this.

---

## License

> Add your chosen license here (e.g. MIT, GPL-3.0). If you're using Scoppy's firmware/code directly, make sure your license is compatible with Scoppy's own license — check the [Scoppy repository](https://github.com/fhdm-dev/scoppy) for its terms.

---

## Contributing

Issues and pull requests are welcome — whether it's schematic improvements, firmware optimizations, or documentation fixes. Open an issue first for larger changes so we can discuss the approach.
