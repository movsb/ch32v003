MRS MAC Toolchain&OpenOCD使用说明

软件依赖于libusb库，请自行安装，可以使用brew install libusb,或者下载libusb源码，自行编译安装设置环境变量。

一、	FLASH CMD

[RISC-V]
sudo ./openocd -f wch-riscv.cfg -c init -c halt -c "flash erase_sector wch_riscv 0 last " -c exit                         #erase all

sudo ./openocd -f wch-riscv.cfg  -c init -c halt  -c "flash write_image xxx.hex\bin\elf "  -c exit                        &&program

sudo ./openocd -f wch-riscv.cfg -c init -c halt -c "verify_image xxx.hex\bin\elf"    -c exit                               #verify

sudo ./openocd -f wch-riscv.cfg -c init -c halt -c wlink_reset_resume    -c exit 
					#reset and resume

[ARM]
sudo ./openocd -f wch-arm.cfg -c init -c halt -c "flash erase_sector wch_arm 0 last " -c exit                             #erase all

sudo ./openocd -f wch-arm.cfg  -c init -c halt  -c "program xxx.hex\bin\elf "  -c exit                                      #erase all &&program

sudo ./openocd -f wch-arm.cfg -c init -c halt -c "verify_image xxx.hex\bin\elf"    -c exit                                  #verify


二、	GDB CMD

1.创建OpenOCD进程:sudo ./[OPENOCD_PATH]/openocd -f  [CFGFILE_PATH]

2.启动GDB进程:sudo ./[GDB_PATH]/riscv-none-embed-gdb

3.GDB指令(*指令必须先依次执行):
*指定调试文件:  file  [FILE_PATH]
*连接端口:      target remote localhost:3333
*烧写代码：     load
查看寄存器:     info register [REGISTER_NAME]
查看当前pc值:  i r pc
查看断点信息:  info breakpoint
设置断点:      break [LINE_NUM/FUNC_NAME/SYMBOL_NAME]
继续运行:      continue
执行下一步:    next
单步进入:      step
打印变量值:    print
查看当前代码:  list（需要工程目录包含源码, 编译时调试等级-g以上）
暂停状态时，可执行查看寄存器、查看当前代码，查看反汇编等操作.

三、	调试步骤演示

1.创建openocd进程
格式：OPENOCD_PATH  -f  CFG_PATH
如果是在ubuntu平台下提倡使用拖拽的方式，提升效率，避免输入路径出错
回车后， openocd进程挂起，等待连接。显示等待连接的端口号，此处为3333
cd openocd路径
指令：
sudo  ./openocd -f wch-riscv.cfg


2. 启动gdb进程。指定调试elf
格式:  GDB_PATH  [FILE_PATH]  [–ARGS]
无参数形式，riscv-none-embed-gdb。默认支持gdb cli命令。未指定FILE_PATH，则后续需要file命令指定调试文件， 
riscv-none-embed-gdb xxxx.elf 带调试文件，后续不需要file指定调试文件
riscv-none-embed-gdb xxxx.elf–interpreter mi 在支持cli基础上，同时支持gdb mi命令。MRS调试打开gdb trace之后，Console输出的命令就是gdb mi命令。可以复制到此模式下逐个运行。

若为无参数形式，后续需要指定调试文件时，命令为file  FILE_PATH
另起一个终端，
指令
cd   riscv-none-embed-gdb路径
Sudo ./riscv-none-embed-gdb 
file +xxx.elf（elf文件目录+文件名）
target remote localhost:3333    //连接端口
Load                         //下载	
info register + 寄存器名， 查看寄存器。查看当前pc值，i r pc
info breakpoint，查看所有断点。当前未设置断点
设置断点 break 行号/方法名/符号名
继续跑到下一个断点 continue, 如果没有断点则一直运行
调试相关的命令：
执行下一步next
单步进入step
打印变量值 print
查看当前代码 list（需要工程目录包含源码, 编译时调试等级-g以上）
暂停状态时，可执行查看寄存器、查看当前代码，查看反汇编等操作。。。

gdb模式下，可输入help all列出所有命令，如搜索、变量打印、查看堆栈等。

