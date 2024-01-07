# CH32V003

这是一个为 CH32V003 准备的项目模板（VSCode + CMake）。

我没有 CH32V00X 系列（CH32V003）的 Windows/Linux 开发/调试环境，官方的 MounRiver Studio 不支持 MacOS，而工具链又对 MacOS 支持非常差。
所以，我就装了个 Windows 虚拟机、创建了一个示例项目，把其中生成的文件提取了出来，研究了一下项目构建流程、下载烧录流程，制作了这个 CMake 库（并不理解为什么官方用 make 而不是 cmake）。
这个库是平台无关的，应该可以轻易地在 MacOS/Windows/Linux 上使用。

## 配置

* `main/Makefile`
  * 需要把文件最前面一些环境变量替换成你自己的。
* `.vscode/launch.json`
  * 需要把 `toolchainPrefix` 改成你自己的工具链目录。
* `.vscode/c_cpp_properties.json`
  * 需要把 `compilerPath` 改成你自己的编译器目录。

## 如何构建

需要先在 `main` 目录下 `make cmake` 生成 CMake，然后就可以 `make build` 编译项目。
具体的命令可以参考 `Makefile`。

直接 `make` 即可一键编译、下载。以下是示例编译输出：

```bash
main (main) → make build
cd "build" && make
[  4%] Building C object stub/CMakeFiles/stub.dir/ch32v00x_it.c.obj
[  8%] Building C object stub/CMakeFiles/stub.dir/core_riscv.c.obj
...省略一些内容...
[100%] Built target ch32v00x.elf
[100%] Built target ch32v00x.hex
[100%] Built target ch32v00x.lst
   text    data     bss     dec     hex filename
  11800     172     432   12404    3074 ch32v00x.elf
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

## 版权

- 文件头中署名 *Author: WCH* 的，归 *Nanjing Qinheng Microelectronics Co., Ltd.* 所有。

## 如何在 MacOS 运行

官方提供的 OpenOCD 不能在 MacOS 上跑起来（缺少 libusb，各种安装、自行编译没搞定），可以找官方要源代码，然后自己编译。可以参考：[在 MacOS 上编译 MounRiver Studio 的 OpenOCD](https://blog.twofei.com/894/)。
