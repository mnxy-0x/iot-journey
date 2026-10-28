# Week 1 — Evidence

**Dates:** 2026-10-24 → 2026-10-30 · **Hours:** ~11
**Tag:** `week-01`

## Deliverable

Linux, Git and C fundamentals for embedded work: a command drill, a C++ program
that exercises reference-vs-pointer, the host decision for the future MQTT
broker, and a photo of the ESP-IDF blink running on hardware.

- `COMMANDS.md` — 22 commands, each with what it does and what actually appeared
- `median.cpp` — median of five floats, struct passed by reference and by pointer
- `HOST.md` — where the broker will run, with the fallback and the revisit trigger
- `blink.jpg` — the board running the blink example

## How it was verified

```
$ g++ -std=c++17 -Wall -o median median.cpp
(no output, no warnings)

$ printf "5\n3\n9\n1\n7\n" | ./median
Enter 5 float values:
Median (by ref):        5.00
Median (by pointer):    5.00
```

Five values, one repeated computation by two different call conventions, same
answer, input struct left unsorted (verified by reading the code path: both
functions sort a local copy).

## Gate

`tracker.py quiz 1`, answered without opening files: 3 of 4.

- `>` truncates, `>>` appends — correct
- commit is local until push; fetch downloads without merging — correct
- reference is an alias, cannot be null, cannot be reseated — correct
- what `idf.py build` produces versus what `idf.py flash` writes — **missed**;
  worked through afterwards

## What broke, and what it taught me

Enabling `networkingMode=mirrored` for WSL2 killed all distro networking: no
default route, no DNS, `/etc/resolv.conf` missing, `ping` returned "Network is
unreachable". Cause not isolated (WSL 3.0.3 with VMware virtual adapters
present). Deleting `.wslconfig` and running `wsl --shutdown` restored everything.
Lesson: after any change to WSL networking, verify `ip route` and one `curl`
before assuming it worked — and never enable a mode whose failure mode I have
not tested.

## Carried over

- `idf.py build` run from a terminal rather than the VS Code buttons. The buttons
  call the same commands, but from week 15 the build happens on headless
  hardware, so the terminal path has to be familiar.
- ESP-IDF and the broker live on the work PC, not this one; `HOST.md` reflects
  that after the week-1 correction.
