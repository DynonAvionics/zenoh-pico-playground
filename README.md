# Dynon embedded Zenoh-Pico application with ZephyrRTOS

This project is based off of the [Zephyr Example Application][example_app] and implements a
[T3 Forest Topology][t3_topology].

[example_app]: https://github.com/zephyrproject-rtos/example-application
[t3_topology]: https://docs.zephyrproject.org/latest/develop/west/workspaces.html#t3-forest-topology

## Dependencies

1. Install [host dependencies](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#install-dependencies) has per Zephyr's Getting Started
2. Install [Zephyr SDK](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#install-zephyr-sdk)

Note: There is probably no need to follow the [Zephyr and Python dependencies](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#get-zephyr-and-install-python-dependencies)
instructions because Zephyr gets installed when running the `west update` command locally.

## The fun stuff

### Building

1. Once Zephyr is properly installed, initialize and update the West Workspace. This step is usually
   done once or everytime the `west.yml` manifest file changes. The manifest file is used to express
   the project's dependencies.

    ```shell
    west init -l
    west update
    ```

2. From this directory, build the code:

    ```shell
    west build -d build/nucleo_f439zi -b nucleo_f439zi -p always apps/zenoh-session-restart
    ```

    Note: the `-d` option specifies the output build directory and can be set to whatever directory
    you want. The only caveat is that at the moment the `west_flash_all.sh` script expects the build
    directories to be `build/<board>`. If you want to change the output build directory, this will
    affect the build directory option for some of the commands explained in the following sections
    and you'll have to do flash the "manual" way (ie: use `west flash`).

    Note2: This repository contains a board definition for the Nucleo-F439zi but it is essentially
    identical to the Zephyr defined Nucleo-F429zi. It is only present because the hardware we
    actually use is the F439zi and we don't need the Crypto/Hash Processor at the moment.

### Flashing

Flashing is also done through `west` but has a two different approaches depending on the number
or boards that you have connected to your computer:

#### Script

The simplest method is to use `west_flash_all.sh` as it works well wether you have one or multiple
`nucleo` boards:

```shell
./west_flash_all.sh
```

Reminder: This script expect the output build directory to be `build/<board>`

#### Manual

This require some extra steps if you have multiple boards but allows you to provide any build
directory if that's your current situation.

- One board connected to your computer:

    ```shell
    west flash -d build/nucleo_f439zi
    ```

    Change the build directory if needed.

- Many boards connected to your computer:

    Getting a list of the connected boards:

    ```shell
    pyocd list
    ```

    This will give a list of unique IDs for the **ST-Link** that are on the Nucleo boards, not the STM32
    chip.

    Flashing the board:

    ```shell
    west flash -d build/nucleo_f439zi -r openocd --cmd-pre-init 'adapter serial <STLINK_SERIAL_NUM>'
    ```

### Debugging

Zephyr provide a gdb debug interface through the `west` command. It is possible to `debug` or `attach`
to an already running piece of code. The command is similar to the `west flash` command but with
`debug` or `attach` instead of `flash`

- One board connected to your computer:

    ```shell
    west <debug|attach> -d <build dir>
    ```

    Change the build directory if needed.

- Many boards connected to your computer:

    Flashing the board:

    ```shell
    west <debug|attach> -d <build dir> -r openocd --cmd-pre-init 'adapter serial <STLINK_SERIAL_NUM>'
    ```

#### Modifying project config

Zephyr provides a `menuconfig` interface to change the OS configuration similar to `buildroot`.
The input of the config is stored in `prj.conf` and various config overlays you provide. To view
that menu based on the board you are building for:

```shell
west build -d <build dir> -b <board> -t menuconfig
```

The recommended workflow is to use `menuconfig` to search for configs and their dependencies. Once
you found the dependency you want to turn on or off, add it to the project's `prj.conf`. It can also
be useful to make changes through `menuconfig` and then press `[D]` to `Save minimal config`. Note
the file that it gets saved to (typically `build/nucleo_g474re/zephyr/kconfig/defconfig`) and copy
paste to your project's `prj.conf`.
