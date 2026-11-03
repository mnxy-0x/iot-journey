sparkfun datasheet:

| # | Parameter | Your value (from the PDF) |
|---|---|---|
| 1 | Supply voltage | 3.3-6V DC |
| 2 | Temperature range | -40~80 Celsius |
| 3 | Humidity range | 0-100% RH |
| 4 | Temperature &  Humidity accuracy | +-2%RH(Max +-5%RH)  +-0.5 Celsius |
| 5 | Sampling period (minimum interval between reads) | 2s |


why the resistor: open-drain line needs a pull-up
why the capacitor: two different jobs — 100 nF decoupling close to the sensor absorbs the current spike during transmission, 100 µF bulk on the rail carries the ESP32 through a WiFi transmit spike
