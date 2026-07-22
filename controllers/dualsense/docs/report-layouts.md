# Report Layouts

These tables are useful to identify the report layouts and how the bytes
are allocated.

> [!NOTE]
> The first byte is always the Report ID, so consider the Byte 0 in the tables
> as the second byte.

## Basic Bluetooth (0x01)

| Byte | Name          | Description                             |
| ---: | ------------- | --------------------------------------- |
|    0 | Left Stick X  | `0-255`                                 |
|    1 | Left Stick Y  | `0-255`                                 |
|    2 | Right Stick X | `0-255`                                 |
|    3 | Right Stick Y | `0-255`                                 |
|    4 | L2 Trigger    | Analog value                            |
|    5 | R2 Trigger    | Analog value                            |
|    6 | Buttons 1     | D-Pad, Square, Cross, Circle, Triangle  |
|    7 | Buttons 2     | L1, R1, L2, R2, Create, Options, L3, R3 |
|    8 | Buttons 3     | PS, Touchpad Click, Mute                |
|    9 | Counter       | Sequence number / packet counter        |

## USB (0x01)

|  Byte | Name                   | Description                                       |
| ----: | ---------------------- | ------------------------------------------------- |
|     0 | Left Stick X           | `0-255`                                           |
|     1 | Left Stick Y           | `0-255`                                           |
|     2 | Right Stick X          | `0-255`                                           |
|     3 | Right Stick Y          | `0-255`                                           |
|     4 | L2 Trigger             | Analog                                            |
|     5 | R2 Trigger             | Analog                                            |
|     6 | Buttons 1              | D-Pad + face buttons                              |
|     7 | Buttons 2              | Shoulder buttons + Create/Options + stick buttons |
|     8 | Buttons 3              | PS, Touchpad Click, Mute                          |
|     9 | Counter                | Packet counter                                    |
| 10-11 | Gyroscope X            | Little-endian `int16`                             |
| 12-13 | Gyroscope Y            | Little-endian `int16`                             |
| 14-15 | Gyroscope Z            | Little-endian `int16`                             |
| 16-17 | Accelerometer X        | Little-endian `int16`                             |
| 18-19 | Accelerometer Y        | Little-endian `int16`                             |
| 20-21 | Accelerometer Z        | Little-endian `int16`                             |
| 22-25 | Sensor Timestamp       | `uint32`                                          |
|    26 | Battery / Cable Status | Battery percentage and charging flags             |
|    27 | Connection Status      | USB status flags                                  |
| 28-31 | Reserved               | Unknown                                           |
| 32-35 | Touch 1                | Touch state and coordinates                       |
| 36-39 | Touch 2                | Touch state and coordinates                       |
| 40-63 | Reserved               | Currently undocumented or vendor-specific         |

## Full Bluetooth (0x31)

Unlike USB mode, Bluetooth reports prepend a small header and append a CRC32.

|  Byte | Name            | Description                            |
| ----: | --------------- | -------------------------------------- |
|     0 | Report ID       | Always `0x31`                          |
|     1 | Sequence Number | Bluetooth packet counter               |
|     2 | Tag             | Bluetooth header                       |
|  3-66 | Payload         | Same layout as USB report bytes `0-63` |
| 67-73 | Reserved        | Bluetooth-specific fields              |
| 74-77 | CRC32           | Packet checksum                        |

## Shared Button bytes

Byte 6:

| Bits | Meaning         |
| ---- | --------------- |
| 0-3  | D-Pad direction |
| 4    | Square          |
| 5    | Cross           |
| 6    | Circle          |
| 7    | Triangle        |

For the D-Pad interpretation, you can follow this table:

|       Value | Binary | Direction          |
| ----------: | :----: | ------------------ |
|       `0x0` | `0000` | Up                 |
|       `0x1` | `0001` | Up + Right         |
|       `0x2` | `0010` | Right              |
|       `0x3` | `0011` | Down + Right       |
|       `0x4` | `0100` | Down               |
|       `0x5` | `0101` | Down + Left        |
|       `0x6` | `0110` | Left               |
|       `0x7` | `0111` | Up + Left          |
|       `0x8` | `1000` | Neutral / released |
| `0x9`–`0xF` |    —   | Reserved / invalid |

The rest of the buttons follow this rule: 1 if the button is pressed, 0 if the
button is released.

Byte 7:

| Bit | Button       |
| --: | ------------ |
|   0 | L1           |
|   1 | R1           |
|   2 | L2 (digital) |
|   3 | R2 (digital) |
|   4 | Create       |
|   5 | Options      |
|   6 | L3           |
|   7 | R3           |

Byte 8:

| Bit | Button         |
| --: | -------------- |
|   0 | PS             |
|   1 | Touchpad Click |
|   2 | Mute           |
| 3-7 | Reserved       |
