# Week 5 — Mosquitto pub/sub between two machines

## What this shows
The Mosquitto broker runs on the laptop (machine A, 10.208.230.56:1883). The subscriber runs on the phone in Termux (machine B) with `-i sub_phone`, listening on `bench/#` at QoS 1. The laptop publishes on `bench/dndn` at QoS 0, 1 and 2, and the phone receives the message over Wi-Fi. The packet log below is both sides: the broker log plus the phone's own `-d` trace, with the publisher's `-d` trace from the laptop.

## Setup (reproduce it)
1. Install: `winget install --id EclipseFoundation.Mosquitto -e`
2. Find the address: run `ipconfig`, take the IPv4 of the Wi-Fi adapter (10.208.230.56).
3. Firewall: `New-NetFirewallRule -DisplayName "Mosquitto MQTT" -Direction Inbound -Protocol TCP -LocalPort 1883 -Action Allow` and network profile Private.
4. Broker: `mosquitto -c D:/001-New_path/Hermes_/projects/iot-journey/week5/mosquitto.conf -v`
5. Subscriber (machine B, phone in Termux): `mosquitto_sub -h 10.208.230.56 -p 1883 -t 'bench/#' -q 1 -i sub_phone -d -v`
6. Publisher (machine A): `mosquitto_pub -h 10.208.230.56 -p 1883 -t bench/dndn -q 1 -m 'file2...'`

## The config, line by line
# Accept connections on TCP port 1883, on ALL IPv4 interfaces (not just loopback).
listener 1883 0.0.0.0

# Allow clients that send no username. Fine on a home LAN for one week; passwords/TLS come in Weeks 9-10.
allow_anonymous true

# Save sessions and retained messages to disk so a broker restart does not forget them.
persistence true

# Folder (must already exist, path must end with a slash) where that saved state goes.
persistence_location D:/001-New_path/Hermes_/projects/iot-journey/week5/data/

# Send the log to the terminal (stdout) so you can watch it and copy it into your packet log.
log_dest stdout
# file D:/001-New_path/Hermes_/projects/iot-journey/week5/data/mosquitto.log

# Log everything: connections, subscriptions, every publish.
log_type all

## Topic tree 
home/<room>/<device-id>/temperature    a reading        payload: JSON, not retained
home/<room>/<device-id>/humidity       a reading        payload: JSON, not retained
home/<room>/<device-id>/status         online/offline   retained, also used as the LWT topic
home/<room>/<device-id>/cmd            a command to the device
home/gateway/<gateway-id>/status       gateway health   retained

## Payload schema
{"id":"sensor01","ts":1732190400,"t":22.5,"h":41.0}

## What I observed
### QoS
| Run | Publish QoS | Subscriber alive? | Delivered | Packets seen in -d |
|---|---|---|---|---|
| 1 | 0 | yes | yes | PUBLISH |
| 2 | 1 | yes | yes | PUBLISH, PUBACK |
| 3 | 2 | yes | yes | PUBLISH, PUBREC, PUBREL, PUBCOMP |
| 4 | 0 | no (killed first) | no | PUBLISH only; lost silently |
| 4b | 1 | no (clean session) | no — but the publisher still got PUBACK | PUBLISH, PUBACK |
| 5 | 1 | no, persistent (-c, fixed -i, -q 1) | yes, on reconnect | queued, then delivered |


### Retain
r1: the msg was retained and stored on broker db, and it's not a new one. and it deleted from broker db only by retain another msg on the same topic, or add empty one (null).
r0: it's a fresh msg, published now, not retained. not stored in broker db, so it deleted after published to subscriper.

### Last Will
| Symptom | Cause | Fix |
|---|---|---|
| Will never arrives after `Ctrl+C` | Clean `DISCONNECT` discards the will | Kill the process or drop the network instead |
| Will arrives immediately after `kill -9`, you expected a delay | Process death closes the TCP socket, so there is nothing to time out | Use `kill -STOP` or airplane mode to test the keepalive |
| Will arrives very late | Keepalive is long (default 60 → ~90 s) | Lower `-k`, accepting more traffic |
| Will never arrives at all | Not registered (no `--will-topic`), or the watcher's filter does not match the will topic, or the will was QoS 0 and the watcher was offline | Look for `Will message specified` in the broker log; check the filter |
| Device status says `online` forever after it died | Retained `online` with no will | Add a retained will on the same topic |

### Client id collision
1795679395: Received SUBSCRIBE from same
1795679395:     bench/# (QoS 0)
1795679395: same 0 bench/#
1795679395: Sending SUBACK to same
1795679396: New connection from 10.208.230.56:37829 on port 1883.
1795679396: Client same [10.208.230.56:37828] disconnected: session taken over.
1795679396: New client connected from 10.208.230.56:37829 as same (p4, c1, k60).
1795679396: No will message specified.
1795679396: Sending CONNACK to same (0, 0)
1795679396: Received SUBSCRIBE from same
1795679396:     bench/# (QoS 0)
1795679396: same 0 bench/#
1795679396: Sending SUBACK to same
1795679397: New connection from 10.208.230.56:37830 on port 1883.
1795679397: Client same [10.208.230.56:37829] disconnected: session taken over.

### Clean vs persistent session
Clean: nothing arrives after the reconnect. Broker log at each connect: (p4, c1, k60) — `p4` is the MQTT protocol level (4 = MQTT 3.1.1), `c1` means clean session on.
persistent: the reconnect prints "bench/lab/f1/status update" at once. Broker log: c0 at each connect, and the CONNACK on the second connect shows session-present 1 — the broker found the session.

## Problems I hit and how I fixed them
| Symptom | Cause | Fix |
|---|---|---|
| A message was published but the subscriber never printed it | public wifi settings | i changed wifi network setting to Private |
