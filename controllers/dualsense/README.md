# Dualsense

This project aims to interpret dualsense HID reports, converting
them into Master System Commands.

There are 3 possible uses of a DualSense controller, each mode sends
a different amount of data in the HID reports.

| Mode            | Report ID | Size         | Contains                                                       |
| --------------- | --------- | ------------ | -------------------------------------------------------------- |
| Basic Bluetooth | `0x01`    | **10 bytes** | Sticks, triggers, buttons                                      |
| USB             | `0x01`    | **64 bytes** | Full controller state, IMU, touchpad, battery, timestamps      |
| Full Bluetooth  | `0x31`    | **78 bytes** | Same information as USB plus Bluetooth-specific header and CRC |

## Report Layouts

Verify the file in `docs/report-layouts.md` to check the tables explaining
the layout of the bytes in the HID reports.

## Adapter

The adapter will send the expected format from master system command layout.
The layout usually is a byte structure, where each bit means one button pressed
in the Master System.

In this repository the following DualSense buttons were mapped

| Bit |   Mask | Master System command | DualSense mapping                 |
| --: | -----: | --------------------- | --------------------------------- |
|   0 | `0x01` | `up`                  | D-pad Up or Left Stick Y ≤ 100    |
|   1 | `0x02` | `down`                | D-pad Down or Left Stick Y ≥ 156  |
|   2 | `0x04` | `left`                | D-pad Left or Left Stick X ≤ 100  |
|   3 | `0x08` | `right`               | D-pad Right or Left Stick X ≥ 156 |
|   4 | `0x10` | `btn_1`               | Cross (`×`)                       |
|   5 | `0x20` | `btn_2`               | Square (`□`)                      |
|   6 | `0x40` | `start`               | Options or Create                 |
|   7 | `0x80` | `reset`               | PS button                         |
