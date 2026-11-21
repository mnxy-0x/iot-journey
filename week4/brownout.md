# Week 4 — Brownout: provoke, identify, fix, prove

**Rule for this file:** the *same* test must be run twice — once before the fix, once after.

---

## 1. What I am provoking, and why

The brownout detector resets the chip when the supply falls below a configurable threshold (2.43 V – 2.80 V). To fix it, I must first see it.

**Method used** (tick one):

- [ ] Long/thin USB cable + weak port
- [x] Series resistance in the 5 V line, value: 2.2 Ω + 1N4007 Diode
- [ ] Other: ____

**Why this method:**

The 2.2 Ω resistor drops 0.53 V at 240 mA. The 1N4007 diode adds a fixed 0.7 V drop. Together with the cable resistance, this degraded the 5 V input down to ~3.7 V, which forces the AMS1117 regulator into dropout, causing the 3V3 rail to collapse under the WiFi TX load.

---

## 2. Baseline — before the fix

| Item | Value |
|---|---|
| Date/time of run | 2026-11-20 14:30 |
| WiFi behaviour at the time | connect loop / single connect (HTTP request) |
| 3V3 idle voltage | 3.38 V |
| 3V3 **minimum during TX** | 2.67 V |
| Meter mode / range used | DC V 20V |
| Captured by Min/Max, or eyeballed? | eyeballed (meter lacks Min/Max) |
| Reboot observed? | yes — reset loop every time the button was pressed |

---

## 3. Console output — quoted verbatim

Captured from the serial monitor while repeating the HTTP request. The stable brownout line captured was:

```text
Brownout detector was triggered
```

On some resets the boot banner began but did not complete; the reset-cause line seen was:

```text
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
```

**Observation:** The message was very brief, indicating the rail fell extremely fast. The chip reset immediately before it could print the full boot banner.


## 4. Diagnosis — power or code?

| Question | My answer |
|---|---|
| Is there a `Guru Meditation Error` or a backtrace? | No. |
| Does the reboot happen only when the radio starts? | Yes, only when the HTTP request is sent. |
| Did the 3V3 minimum go under 3.13 V? | Yes, dropped to 2.67 V (below the 3.13 V minimum band). |
| Therefore: power / code — and the one measurement that decides it | Power. The 3V3 Min voltage measurement decides it. |

---

## 5. The fix

| Item | Value |
|---|---|
| Component added | 100 µF electrolytic + 100 nF ceramic |
| Where exactly (which pins, distance from the module) | Across the 3V3 and GND pins, as close to the ESP32 module as physically possible by jumpers. |
| Photo | `week4/board-power.jpg` |
| Anything else changed in the same step? | No. The exact same 2.2 Ω resistor and diode were kept in place to prove the capacitor alone fixed the issue. |

---

## 6. Re-measure — the identical experiment

| Item | Before fix | After fix | Delta |
|---|---|---|---|
| 3V3 minimum during TX | 2.67 V | 3.15 V | +0.48 V |
| Reboots in 10 s | continuous | 0 | 0 |

**Same conditions check:**
Same charger, same 2.2 Ω resistor, same 1N4007 diode, same HTTP request code. Only the capacitors were added.

---

## 7. Verdict

The brownout was caused by power, not code. The series 2.2 Ω resistor + 1N4007 diode degraded the 5 V input enough to drive the AMS1117 into dropout, and the 3V3 rail collapsed to 2.67 V during WiFi TX, below the 3.13 V tolerance minimum and into the brownout detector range. Adding 100 µF + 100 nF decoupling directly across 3V3/GND raised the TX minimum to 3.15 V and eliminated all reboots under the identical test conditions. The single change — the capacitors — is therefore the fix.

---

## 8. Before you submit — self-check

- [x] The two runs were identical apart from the fix
- [x] The console output shows the brownout/reset cause
- [x] The "before" number exists — I did not fix first and measure after
- [x] The fix is attributed to one change
- [x] Photo shows the capacitor where I said it is
