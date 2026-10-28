# iot-journey

My working log on the way to an IoT Integration Engineer role: one week, one
deliverable, every commit kept.

This repository covers **week 1 — Linux, Git and C for embedded work**, plus the
hosting decision for the MQTT broker that arrives in week 5.

## What is inside

| File | What it is |
|---|---|
| `COMMANDS.md` | 22 Linux and Git commands I ran myself, with what each one does and what I actually saw on screen |
| `median.cpp` | Small C++ program: median of 5 numbers, written twice — once passing the struct by reference, once by pointer |
| `HOST.md` | One-page decision record: the MQTT broker runs in WSL2 on the work PC (the one with ESP-IDF) for weeks 1–6 and moves to a Raspberry Pi at week 15, with the cost, the fallback and the conditions that would change the decision |
| `blink.jpg` | Photo of the ESP32 board running the ESP-IDF blink example |
| `.gitignore` | Keeps build output and compiled binaries out of git |

## Requirements

- Linux or WSL2 Ubuntu (everything here was built in Ubuntu on WSL2)
- `g++` with C++17 support: `sudo apt install build-essential`
- `git`

## Build and run the median program

```bash
git clone <your-repo-url> iot-journey
cd iot-journey

g++ -std=c++17 -Wall -o median median.cpp
./median
```

Enter five numbers, one per line. Example session:

```
$ ./median
Enter 5 float values:
5
3
9
1
7
Median (by ref):        5.00
Median (by pointer):    5.00
```

## What the program demonstrates

1. **Struct by reference vs by pointer.** `get_median_ref(FloatWindow& win)`
   takes a C++ reference; `get_median_ptr(FloatWindow* win)` takes a pointer and
   checks for `nullptr` first. Same result, two different safety models.
2. **A hand-written insertion sort.** No `std::sort` — the point was to write
   the loop and get the index arithmetic right.
3. **Non-destructive sorting.** Both functions copy the data into a local array
   before sorting, so the caller's struct keeps its original order.

## Hosting decision

The broker is not part of week 1. `HOST.md` records *where it will run* so the
choice is not re-argued later: Mosquitto in WSL2 on the work PC — the machine
that already holds ESP-IDF — so broker, test clients and toolchain live in one
place. LAN access from the ESP32 uses mirrored networking when it works, and
NAT plus a port-forward when it does not; that fallback is there because
mirroring was tried on the secondary PC and killed all WSL networking, which was
reverted. Revisit trigger: week 15, when the plan needs a broker that survives a
reboot.

## Where this is going

- Week 2: embedded C, plus a Python and networking check
- Week 5: Mosquitto, first publish/subscribe between two clients
- Week 6: ESP32 connects to WiFi from scratch
- Week 7: DHT22 sensor publishes JSON every 10 seconds

## Status

Week 1 complete: command drill, median program, host decision, and the ESP-IDF
blink verified on hardware.
