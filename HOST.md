# HOST.md — Where the MQTT broker runs

**Status:** decided (week 1) · **Owner:** Adel · **Revisit:** week 15

## Decision

The MQTT broker (Mosquitto) runs in **WSL2 Ubuntu on this PC**, with
`networkingMode=mirrored`. ESP-IDF stays on **Windows** and only flashes the
board. Nothing moves to a separate machine before week 15.

## Context

- Week 5 is the first time a broker exists at all (`Mosquitto: أول pub/sub`).
  Weeks 1–4 need no always-on service.
- The ESP32 sits on the home LAN and must reach the broker over TCP 1883 at the
  PC's LAN address.
- This PC: Ethernet `192.168.1.4`. WSL2 Ubuntu defaults to NAT
  (`192.168.122.191`, gateway `192.168.112.1`), which the ESP32 **cannot**
  reach. Mirrored networking makes WSL2 answer on `192.168.1.4` itself, so no
  port-forward (`netsh interface portproxy`) and no WSL IP bookkeeping.
- ESP-IDF already runs on Windows and needs the USB/serial port, which is
  awkward inside WSL2 (`usbipd-win`). Keeping the toolchain on Windows removes
  that whole class of problems.

## Options considered

| Option | Cost | Uptime | ESP32 can reach it | Verdict |
|---|---|---|---|---|
| WSL2 (mirrored) | 0 | only while the PC is on | yes, same LAN IP | **chosen for weeks 5–14** |
| Raspberry Pi | hardware cost (verify price at purchase) | 24/7 | yes, own LAN IP | week 15 |
| VPS | monthly cost (verify) | 24/7 | only via tunnel/VPN | rejected now |

## Why not the others yet

A Pi buys uptime I do not need while learning: in weeks 5–14 every test happens
while I am sitting at the machine. A VPS adds a tunnel before the MQTT
fundamentals are solid, and puts device data outside the house for no gain.

## What would change my mind

1. Week 15 — the plan needs a broker that survives a PC reboot. → move to Pi.
2. A test requires the broker to stay up overnight or while travelling. → Pi earlier.
3. A remote device or colleague must publish from outside the LAN. → VPS.
4. Mirrored networking breaks the Windows toolchain (Docker, VPN) badly enough
   that I lose more time than a Pi costs. → Pi.

## Split of responsibilities (so nothing is duplicated)

| Thing | Lives on | Why |
|---|---|---|
| Mosquitto broker | WSL2 Ubuntu | Linux, systemd, matches production |
| Python MQTT clients | WSL2 Ubuntu | same network namespace as the broker |
| ESP-IDF (`idf.py build/flash/monitor`) | Windows | USB/serial access to the board |
| Source code / git | Windows + GitHub | one clone, one history |

## Acceptance for this document

A stranger reads this page and can answer: where the broker runs, why not the
alternatives, what it costs, and what would trigger the move — without asking me.
