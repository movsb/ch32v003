# CH32V003

This is a minimal and very easy-to-use CH32V003(J4M6) project template made for developing in VsCode using CMake.

Before making this project, I found that the official MounRiver Studio doesn't support MacOS, they only provide toolchains.
This is the main reason why this project exists (and I hate Editors without Vim-Keybinding support.).
After some Googling and GitHub-ing, projects like [cnlohr/ch32v003fun](https://github.com/cnlohr/ch32v003fun)
and [openwch/ch32v003](https://github.com/openwch/ch32v003) are too difficult for me to understand where to begin,
and some of them are targeted Windows.

The source code in this project is mostly copied from the generated content of MRS v1.91 for Non-OS CH32V003(J4M6).

What changes are made?

- Re-structured.
- Added CMake support.
  - To use in command-line
  - To cooperate with CMake Tools extension
- Launch and Debug json file
  - Launch only, no Attach supported.
- C++ support by default
  - but, don't introduce `<iostream>`
- long-run SysTicks providing uptime support
  - mainly used for time/timer scheduling.
- Re-implemented Delay_Us and Delay_Ms
  - by using long-run SysTicks.
- Remapped USART to not using SDIO
  - SDIO is for flashing, sharing the same PIN with USART
    puts CH32V003 in a state in which you cannot flash again.
    and, WCH-LinkE's 'Power off/on and Erase all' is not available to OpenOCD.
  - I don't know if SDIO can work with USART, if it works for you, please help me out of this.
  - Since USART is re-mapped to another pin, another USB-UART bridge is required.
- `printf("\n")` instead of `printf("\r\n")`
  - because it's more common.
- and more.

## Prerequisites

- WCH's official OpenOCD and cross-platform toolchain installed
- A CH32V003 development board
- WCH-LinkE (not WCH-Link)

**Note:** if you're on MacOS, you should compile OpenOCD for yourself. See the sections at the end.

## Config

You should first have the WCH's official OpenOCD and toolchain installed.

* `main/Makefile`
  * `OPENOCD_DIR` and `WCH_CFG`
* `.vscode/launch.json`
  * `toolchainPrefix`
* `.vscode/c_cpp_properties.json`
  * `compilerPath`

## How to Build

Goto `main` folder, type `make cmake` to generate the cmake project, then you can `make build` to build the project.

Below is an example of how to build:

```bash
main (main) → make build
cd "build" && make
[  4%] Building C object stub/CMakeFiles/stub.dir/ch32v00x_it.c.obj
[  8%] Building C object stub/CMakeFiles/stub.dir/core_riscv.c.obj
...omits some build logs...
[100%] Built target ch32v00x.elf
[100%] Built target ch32v00x.hex
[100%] Built target ch32v00x.lst
   text    data     bss     dec     hex filename
  11800     172     432   12404    3074 ch32v00x.elf
[100%] Built target ch32v00x.siz
```

## How to Flash

`make flash` is all you need.

## How to Debug

Debugging is supported!

First, `make debug` to let OpenOCD listen for incoming connections from GDB.

Press `F5` (`Run`/`Start Debugging`) to enter debugging mode.

Features supported:

- Step in, Step out, Step over
- Watch, Eval
- Breakpoints
- Memory Viewer
- Stacks

## Copyright

File headers containing *Author: WCH* is copyrighted to *Nanjing Qinheng Microelectronics Co., Ltd.*.

## MacOS?

I'm on MacOS.

But, the OpenOCD provided by MounRiver Studio doesn't work on MacOS.
You can request the source code of OpenOCD by sending email to the MRS developers to obtain a copy.
The Email address can be found in the README file of MRS' OpenOCD directory.

See <https://blog.twofei.com/894/> to see more if you have problem compiling OpenOCD for your own.

## License

MIT.
