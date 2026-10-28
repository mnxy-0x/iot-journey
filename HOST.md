# HOST.md — Where the MQTT broker runs

**Status:** decided (week 1) · **Owner:** Adel · **Revisit:** week 15

## Decision

The MQTT broker (Mosquitto) runs in **WSL2 Ubuntu on this PC**, with
`networkingMode=mirrored`. ESP-IDF stays on **Windows** and only flashes the
board. Nothing moves to a separate machine before week 15.

## Context

- Week 5 is the first time a broker exists at all (`Mosquitto: أول pub/sub`).
  Weeks 1–4 need no always-on service.
- The ESP32 must reach the broker over TCP 1883 at whatever LAN address the PC
  holds at that moment.
- This PC is on a **phone hotspot**, so the address is not stable. Measured
  2026-10-28: Wi-Fi `10.131.42.56/24`, gateway `10.131.42.205`. It changes with
  the upstream network, so the broker address is read at the start of each
  session (`ip -4 addr`), never hardcoded. (The earlier `192.168.1.4` came from
  the PC where this repo was created; it does not exist here.)
- WSL2 Ubuntu defaults to NAT, which the ESP32 cannot reach.
  `networkingMode=mirrored` is already set in `C:\Users\adelf\.wslconfig` and
  was verified: inside WSL2 the interface `eth2` holds `10.131.42.56`, the same
  address as the Windows Wi-Fi adapter. So WSL2 answers on the Windows address
  itself — no port-forward (`netsh interface portproxy`), no WSL IP bookkeeping.
- **Open risk for week 5:** a phone hotspot can isolate clients, and
  `10.131.42.x` with a `.205` gateway looks more like carrier NAT than a real
  LAN. If the ESP32 cannot open a TCP connection to the PC, this plan needs a
  fallback (Pi acting as its own AP, or a small router). Test reachability in
  both directions *before* writing the MQTT client.
- ESP-IDF already runs on Windows — `C:\esp\v6.1\esp-idf` (v6.1), projects in
  `D:\esp-idf_projects` — and needs the USB/serial port, which is awkward inside
  WSL2 (`usbipd-win`). Keeping the toolchain on Windows removes that whole class
  of problems.

## Options considered

| Option | Cost | Uptime | ESP32 can reach it | Verdict |
|---|---|---|---|---|
| WSL2 (mirrored) | 0 | only while the PC is on | yes, same LAN IP — unless the hotspot isolates clients (unverified) | **chosen for weeks 5–14** |
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
