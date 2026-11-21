# Week 4 — Measurement Sheet

**Week:** 4 · **Dates:** 2026-11-14 → 2026-11-20 · **Phase 0 — Foundation and Bench**
**Depth:** D2–D3 · **Deliverable:** this sheet.

---

## 0. Bench inventory — fill once, before the first measurement

| Item | Value |
|---|---|
| Board | ESP32 DevKit, model: v1 (30-pin) |
| Meter | brand/model: Generic DT-830B |
| Meter ranges available | DC V: 20V · DC mV: 200mV/2000mV · DC A: 10A · DC mA: 200mA · Ω: 2M · continuity: yes |
| Meter has Min/Max (peak hold)? | no — voltage minima were eyeballed during repeated TX loops; current was read as meter average, as stated in §5 |
| Supply used | USB Wall Charger, stated limit: 3.0 A |
| USB cable | length: 100 cm · thick/thin: medium |
| Capacitors on hand | 100 µF: yes · 10 µF: no · 100 nF: yes |
| Shunt resistor available | value: 0.5 Ω (custom wire) and 2.2 Ω |

---

## 1. Voltage

Voltage is measured **across** two points, meter in parallel. State the black-probe node for every row.

| # | Node measured (red probe) | Reference (black probe) | Condition | Value | Meter mode / range |
|---|---|---|---|---|---|
| 1 | 5V pin | GND | idle | 5.10 V | DC V 20V |
| 2 | 3V3 pin | GND | idle | 3.38 V | DC V 20V |
| 3 | 3V3 pin | GND | during WiFi TX | min 3.17 V | DC V 20V |
| 4 | 3V3 pin | GND | during WiFi TX (with 2.2Ω + diode) | min 2.67 V | DC V 20V |

**Notes on how row 3 was obtained:**

Meter has no Min/Max. The display was watched while repeatedly triggering the HTTP request via the button. The worst (lowest) stable value seen during the 3.17 V drop was recorded. For the brownout test, the voltage dropped too fast and caused a reset, with the meter showing 2.67 V just before the display froze.

---

## 2. Current

Current is measured **in series** — the meter (or a shunt) becomes part of the circuit.

**Method used** (tick one, and say why in one line):

- [ ] Inline USB power meter — dongle between cable and source
- [ ] USB breakout + meter in series in the 5 V line
- [x] Shunt resistor + voltage across it, `I = V / R`

**Method notes:**

| # | Condition | Value | How obtained | Meter mode / range |
|---|---|---|---|---|
| 0 | Shunt calibration | R_shunt = 0.500 Ω | 5.10 V through 100 Ω in series with shunt: I ≈ 5.10 / (100 + 0.5) = 50.75 mA; measured V_shunt = 25.4 mV; R = 25.4 mV / 50.75 mA = 0.500 Ω | DC mV 200mV |
| 1 | Idle / modem-sleep (WiFi connected, not transmitting) | 26.6 mA | Calibrated 0.500 Ω shunt: 13.3 mV | DC mV 200mV |
| 2 | Transmit average during RF activity (not µs/ms peak) | 28.8 mA | Calibrated 0.500 Ω shunt: 14.4 mV | DC mV 200mV |
| 3 | Transmit average during RF activity, second run (check it repeats) | 30.0 mA | Calibrated 0.500 Ω shunt: 15.0 mV | DC mV 200mV |

**Shunt arithmetic:**

R_shunt = 0.500 Ω (calibrated with a known current, not estimated from the DMM's direct ohms reading)
I = V_measured / R_shunt

idle: 0.0133 V / 0.500 Ω = 0.0266 A = 26.6 mA
TX average: 0.0144 V / 0.500 Ω = 0.0288 A = 28.8 mA
TX average, second run: 0.0150 V / 0.500 Ω = 0.0300 A = 30.0 mA

---

## 3. Supply budget

Use **your** measured numbers, not the datasheet's.

| Question | Answer |
|---|---|
| Supply chosen | 5V 3A USB Wall Charger |
| Its stated current limit | 3.0 A |
| Why it is enough at the **peak** (not the average) | The only measured TX figure is an average during RF activity: 30.0 mA, giving 100× headroom over 3.0 A. The DT-830B cannot capture the true µs/ms RF peak, so the conservative peak check uses the datasheet worst case of 240 mA: 3.0 A / 0.240 A = 12.5× headroom. |
| What would break first if the peak were exceeded | The DevKit's AMS1117 regulator (max ~800 mA) would overheat or current-limit before the 3.0 A charger. |

---

## 4. Reference values — from the datasheet, NOT your measurements

| Item | Datasheet value |
|---|---|
| TX 802.11b, DSSS 1 Mbps, +19.5 dBm | 240 mA |
| TX 802.11g, OFDM 54 Mbps, +16 dBm | 190 mA |
| TX 802.11n, OFDM MCS7, +14 dBm | 180 mA |
| RX 802.11b/g/n | 95–100 mA |
| Modem-sleep, 80 MHz, dual core | 20–31 mA |
| Deep-sleep (RTC timer + RTC memory) | 10 µA |
| Brownout threshold | 2.43 V – 2.80 V (configurable) |
| 3V3 tolerance band (±5%) | 3.13 V – 3.46 V |

**Comparison:**

Measured idle current (26.6 mA) falls inside the datasheet modem-sleep range (20–31 mA), which confirms the calibrated 0.500 Ω shunt and the calculation. The measured TX figures (28.8–30.0 mA) are averages during RF activity, not peak currents: the DT-830B refreshes at about 2–3 Hz and cannot capture the ~1 ms RF burst. Therefore they should not be compared directly with the datasheet TX peak (240 mA); they are lower because they are averaged over time.

---

## 5. Method declaration and limitations

| Question | Answer |
|---|---|
| Was the peak captured by the meter, or seen by eye? | Neither true peak was captured. Current was read by the meter as an average during TX activity; voltage minima were eyeballed. |
| What is the meter's sampling/refresh limitation here? | The DT-830B refreshes about 2–3 times per second. The RF burst is on the order of 1 ms, so the meter cannot see the true peak. |
| Anything measured with the wrong reference (ground) node? | No, all measurements were taken with the black probe firmly on the board's GND pin. |
| Any number in this sheet taken from a datasheet instead of measured? | No measurement row uses a datasheet value. Datasheet values appear only in §4 reference table and in the §3 worst-case peak headroom note. The shunt calibration is measured. |
| What would you need to measure the µs-scale dip properly? | An oscilloscope with a current probe, or a high-bandwidth shunt + differential probe. |