# PS22MS

This project aims to create an driver for different kinds of controllers to
the Tectoy Sega Master System console.

The console has 132 games and simple communication with the controller.

On the other hand, the DualShock 2 has a slightly more complex communication,
as the microcontroller inside uses the SPI interface to communicate with the PS2.

The project consists of two parts: the communicator for the MS and the PS2 interpreter.

## Setup

### Install

- PlatformIO Core
  This project uses PlatformIo as tool for building and communicating with the device.
  Check [platformio.ini](ms_communicator/platformio.ini) file for the device
  configuration used in this project.

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

- Task
  This project uses Task for managing command accross different projects. Follow
  the official documentation to install it [Task Installation Guide](https://taskfile.dev/docs/installation)
- CMake
  The projects which do not depend on a board and uses c++ compilers will use
  CMake for build process managment. Install CMake following the official
  documentation by [CMake Installation Guide](https://cmake.org/download/) or
  use your package manager to install it.

### Build

Compile all project components by running

```sh
task build
```

This command generate the compilation database for `ms_communicator`
and configures and compiles dualsense_interpreter.

### Run

You need to upload the `ms_communicator` code to the board used to send
signals to the Master System. You also need to run the controller interpreter.

```sh
task run
```
