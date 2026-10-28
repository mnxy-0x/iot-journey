# HOST.md — Where the MQTT broker runs

**Status:** decided (week 1) · **Owner:** Adel · **Revisit:** week 15

## Decision

The MQTT broker (Mosquitto) runs in **WSL2 Ubuntu on the work PC** — the machine
that also holds ESP-IDF and the roadmap — so the broker, the test clients and the
flashing toolchain sit on one machine. Access from the ESP32 needs the WSL2 port
to be reachable on the LAN; the first choice is `networkingMode=mirrored`, and
the tested fallback is NAT plus a `netsh interface portproxy` rule (or Mosquitto
installed natively as a Windows service).

Nothing moves to a separate machine before week 15.

## Context

- Week 5 is the first time a broker exists at all (`Mosquitto: أول pub/sub`).
  Weeks 1–4 need no always-on service.
- The ESP32 sits on the home LAN and must reach the broker over TCP 1883.
- WSL2 defaults to NAT, where the distro sits on a private subnet and LAN devices
  cannot reach it without a port-forward. Mirrored networking publishes the
  host's LAN address inside the distro, which removes the port-forward entirely —
  **but it is not free**: on the secondary PC (WSL 3.0.3, Windows 11, VMware
  virtual adapters present) enabling it left the distro with no default route,
  no DNS and no `/etc/resolv.conf`. Deleting `.wslconfig` and restarting WSL
  restored it. The setting is therefore treated as an optimisation to test, not
  a foundation to trust.
- ESP-IDF needs the USB/serial port to flash. Keeping it on Windows avoids
  `usbipd-win` and its device-sharing problems.

## Options considered

| Option | Cost | Uptime | ESP32 can reach it | Verdict |
|---|---|---|---|---|
| WSL2 on the work PC (mirrored, else NAT + portproxy) | 0 | only while the PC is on | yes, with one of the two methods | **chosen for weeks 5–14** |
| Raspberry Pi | hardware cost (verify price at purchase) | 24/7 | yes, own LAN IP | week 15 |
| VPS | monthly cost (verify) | 24/7 | only via tunnel/VPN | rejected now |

## Why not the others yet

A Pi buys uptime that is not needed while learning: in weeks 5–14 every test
happens while sitting at the machine. A VPS adds a tunnel before the MQTT
fundamentals are solid, and moves device data off-site for no gain.

## What would change my mind

1. Week 15 — the plan needs a broker that survives a PC reboot. → move to Pi.
2. A test requires the broker to stay up overnight or while travelling. → Pi earlier.
3. A remote device or colleague must publish from outside the LAN. → VPS.
4. No WSL2 networking mode gives the ESP32 a stable path within one working
   session. → Pi earlier, or Mosquitto as a native Windows service as a stopgap.

## Split of responsibilities

| Thing | Lives on | Why |
|---|---|---|
| Mosquitto broker | WSL2 Ubuntu (work PC) | Linux, systemd, matches production |
| Python MQTT clients | WSL2 Ubuntu (work PC) | same network namespace as the broker |
| ESP-IDF (`idf.py build/flash/monitor`) | Windows (work PC) | USB/serial access to the board |
| Source code / git | Windows + GitHub | one clone, one history |

## Note on this PC (secondary, no ESP-IDF)

The secondary PC holds neither ESP-IDF nor the roadmap, so it does not host the
broker. Its WSL2 is left in the default NAT mode. Mirroring was tried here, broke
all distro networking, and was reverted — the failure is recorded above as the
reason the fallback exists.

## Acceptance for this document

A stranger reads this page and can answer: where the broker runs, why not the
alternatives, what it costs, what the fallback is, and what would trigger the
move — without asking me.
