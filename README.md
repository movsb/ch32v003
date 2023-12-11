# CH32V00X

我没有 CH32V00X 系列（CH32V003）的 Windows/Linux 开发/调试环境，官方的 MounRiver Studio 不支持 MacOS，而工具链又对 MacOS 支持非常差。
所以，我就装了个 Windows 虚拟机、创建了一个示例项目，把其中生成的文件提取了出来，研究了一下项目构建流程、下载烧录流程，制作了这个 CMake 库（并不理解为什么官方用 make 而不是 cmake）。
这个库是平台无关的，可以轻易地在 MacOS/Windows/Linux 上使用。

## 如何在 MacOS 运行

官方提供的 OpenOCD 不能在 MacOS 上跑起来（缺少 libusb，各种安装、自行编译没搞定），所以我在 MacOS 上用 UTM 安装了一个 Alpine Linux 虚拟机（非常省资源：1G 硬盘 + 128M 内存 就足够）。
系统运行时，把主机设备一键共享到虚拟机，然后通过 ssh 执行虚拟机内的 OpenOCD 命令。
可以通过基于 ssh 创建的 make 命令一键完成 编译、上传、下载、复位、调试，非常便捷。

记得在 `$PATH` 中导出工具链目录，例如：

- 在 `~/.bash_profile` 中添加一行：

  ```bash
  alias get-riscv='export PATH="/Users/Shared/MRS_Toolchain_MAC_V191/xpack-riscv-none-embed-gcc-8.2.0/bin:${PATH}"'
  ```

- 然后在使用前 `get-riscv` 一下即可，可以避免添加到全局环境中造成污染。

### Alpine 配置

- 使用 UTM 安装 Alpine Linux （其它的也行，Alpine 非常小）
- 安装 apk add gcompat，这是 glibc
- 插入设备，并共享主机设备
- 可以了

- 开启  TCP 端口转发：`AllowTcpForwarding yes`

**待补充：** 安装工具链的过程

## 如何编译

直接 `make` 即可一键编译、下载。以下是示例编译输出：

```bash
ch32v00x (main) → make build
cd "build" && make
[  3%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_adc.c.obj
[  7%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_dbgmcu.c.obj
[ 11%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_dma.c.obj
[ 15%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_exti.c.obj
[ 19%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_flash.c.obj
[ 23%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_gpio.c.obj
[ 26%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_i2c.c.obj
[ 30%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_iwdg.c.obj
[ 34%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_misc.c.obj
[ 38%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_opa.c.obj
[ 42%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_pwr.c.obj
[ 46%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_rcc.c.obj
[ 50%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_spi.c.obj
[ 53%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_tim.c.obj
[ 57%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_usart.c.obj
[ 61%] Building C object peripheral/CMakeFiles/peripheral.dir/ch32v00x_wwdg.c.obj
[ 65%] Linking C static library libperipheral.a
[ 65%] Built target peripheral
[ 69%] Building C object stub/CMakeFiles/stub.dir/ch32v00x_it.c.obj
[ 73%] Building C object stub/CMakeFiles/stub.dir/core_riscv.c.obj
[ 76%] Building C object stub/CMakeFiles/stub.dir/debug.c.obj
[ 80%] Building ASM object stub/CMakeFiles/stub.dir/startup_ch32v00x.S.obj
[ 84%] Building C object stub/CMakeFiles/stub.dir/system_ch32v00x.c.obj
[ 88%] Linking C static library libstub.a
[ 88%] Built target stub
[ 92%] Building C object CMakeFiles/ch32v00x.elf.dir/main.c.obj
[ 96%] Building CXX object CMakeFiles/ch32v00x.elf.dir/test.cpp.obj
[100%] Linking CXX executable ch32v00x.elf
[100%] Built target ch32v00x.elf
[100%] Built target ch32v00x.hex
[100%] Built target ch32v00x.lst
   text    data     bss     dec     hex filename
   9660     172     420   10252    280c ch32v00x.elf
[100%] Built target ch32v00x.siz
```

## 如何调试

执行 `make debug` 开启端口监听。
然后保存下面的文件为 `.vscode/launch.json`：

```json
{
	// Use IntelliSense to learn about possible attributes.
	// Hover to view descriptions of existing attributes.
	// For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
	"version": "0.2.0",
	"configurations": [
		{
			"variables": {
				"toolchainPrefix": "/Users/Shared/MRS_Toolchain_MAC_V191/xpack-riscv-none-embed-gcc-8.2.0"
			},
			"name": "OpenOCD",
			"type": "cppdbg",
			"request": "launch",
			"cwd": ".",
			"program": "${workspaceFolder}/build/ch32v00x.elf",
			"MIMode": "gdb",
			"miDebuggerPath": "${toolchainPrefix}/bin/riscv-none-embed-gdb",
			"useExtendedRemote": true,
			"miDebuggerServerAddress": "localhost:3333",
			"setupCommands": []
		}
	]
}
```

修改其中的 `variables` 部分为你自己环境的。

按快捷键 `F5` 或点击菜单 `Run` / `Start Debugging` 即可进入调试。

支持的功能：

- 单步、步进、步出
- 查看变量、添加观察
- 断点调试
- 内存查看
- 调用栈

**注：** 目前仅支持*启动*级别的调试，*附加*调试还没配好。

## 参考文档

- <https://nc-pin.com/>
- [Running glibc programs - Alpine Linux](https://wiki.alpinelinux.org/wiki/Running_glibc_programs#gcompat)

## 版权

- 文件头中署名 *Author: WCH* 的，归 *Nanjing Qinheng Microelectronics Co., Ltd.* 所有。

## TODO

- 把我的测试挪出此库
- 修正部分相对目录为 CMake 变量
