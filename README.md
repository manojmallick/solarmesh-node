# SolarMesh Node

A PlatformIO firmware simulation of an adaptive solar-powered Meshtastic node. The current code models battery voltage, solar input, beacon counts, and sleep intervals, then prints simulated telemetry over serial.

## Important limitation

This repository is a simulation only. It does not read a real battery or solar controller, enter deep sleep, or send/receive LoRa or Meshtastic packets. Do not use it as firmware for unattended or safety-critical equipment. Hardware support and field behavior have not been implemented or validated.

## Build and run

Requirements: PlatformIO Core and an ESP32 development board for device builds.

```bash
pio run -e esp32dev
pio run -e native
```

Flash/monitor only if you understand the target board and serial settings:

```bash
pio run -e esp32dev --target upload
pio device monitor -b 115200
```

The `esp32dev` loop scales its simulated sleep delay for demonstration. Native tests exercise the charge model and adaptive interval calculations.

## Project files

- `src/main.cpp`: simulated telemetry loop.
- `test/test_solar.cpp`: native checks for battery bounds and sleep interval selection.
- `platformio.ini`: ESP32 and native build environments.

## License

Licensed under the [PolyForm Noncommercial License 1.0.0](LICENSE). This source-available license restricts commercial use; it is not an OSI-approved open-source license. See the license for terms and exceptions. Third-party dependencies retain their own licenses.
