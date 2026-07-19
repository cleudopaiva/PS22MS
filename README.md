# PS22MS

This project aims to create an driver for the DualShock 2 (PS2 controller) for
the Tectoy Sega Master System console.

The console has 132 games and simple communication with the controller.

On the other hand, the DualShock 2 has a slightly more complex communication,
as the microcontroller inside uses the SPI interface to communicate with the PS2.

The project consists of two parts: the communicator for the MS and the PS2 interpreter.

## MS Communicator

A DB9 connector is used to conenct the controller and the Master System and
the buttons works in a simple PULL_UP resistor system.

The Master System's controller was initially designed to work with 2 buttons +
4 directions, which used 6 pins in the DB9 connector. Other version with 6
buttons + 4 directions + START button was released years later.

The version being parsed in this repo is the 6 button's version.

Since the DB9 has only 9 pins, where 1 of them is for GND and. So it has a
total of 8 pins available. One for the RESET(the reset is connected to the GND
when X, Y, Z are pressed simultaneously). 4 of them for the directions,
1 for A/B because these are shared, 1 for the START button and one for the C button.

Below there is a table of the PINOUT

| Pin | Console Signal | Controller Mapping | Behavior |
| ---: | -------------- | ------------------ | -------- |
| **1** | UP | D-Pad Up | Connected to GND when pressed |
| **2** | DOWN | D-Pad Down | Connected to GND when pressed |
| **3** | LEFT | D-Pad Left | Connected to GND when pressed |
| **4** | RIGHT | D-Pad Right | Connected to GND when pressed |
| **5** | RESET | **X + Y + Z** combination | All three buttons must be pressed simultaneously |
| **6** | Button 1 / TL | **A and B** buttons | A and B share the same input |
| **7** | PAUSE | **START** button | Triggers the console's Pause/Start function |
| **8** | GND | Ground / Common | Common return for all buttons |
| **9** | Button 2 / TR | **C** button | Second action button |

## Setup

This project uses PlatformIo as tool for building and communicating with the device.
Check [platformio.ini](platformio.ini) file for the device configuration used in
this project.

### Install

Install platformio core if you do not have it by following the instructions
on [Installation guide](https://docs.platformio.org/en/latest/core/installation/index.html)

#### Arch Linux

For Arch Linux users, you can install with:

```sh
sudo pacman -S platformio-core platformio-core-udev
```

Then reload the udev rules:

```sh
sudo udevadm control --reload-rules
sudo udevadm trigger
```

### Build

If you are using clangd or another LSP, generate the compilation database by running:

```sh
pio run -t compiledb
```

This creates the [compile_commands.json](compile_commands.json) file used by the
language server.

### Upload

Uploading the code to the board is simple, you just need to figure out
which is your board's serial port, then upload to it.

To see the available ports, run:

```sh
pio device list
```

Then upload using:

```sh
pio run -t upload --upload-port <device-port>
```
