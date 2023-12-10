# CH32V00X

## 如何在 MacOS 运行

官方对 MacOS 的支持非常差：

- 工具链不完整（缺少 libusb，安装完 arm 版报 team id 不对，brew 安装 x64 又麻烦）

- 使用 UTM 安装 Alpine Linux （其它的也行，Alpine 非常小）
- 安装 apk add gcompat，这是 glibc
- 插入设备，并共享主机设备
- 可以了

### Alpine 配置

- 开启  TCP 端口转发：`AllowTcpForwarding yes`


## 参考文档

- [Running glibc programs - Alpine Linux](https://wiki.alpinelinux.org/wiki/Running_glibc_programs#gcompat)
