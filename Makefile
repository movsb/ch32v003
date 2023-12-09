SHELL := /bin/bash

.PHONY: all
all: build sync flash reset

.PHONY: build
build:
	cd build && make

.PHONY: sync
sync:
	cd build && scp ch32v00x.hex embed:

.PHONY: flash
flash:
	ssh -t embed 'cd /root/MRS_Toolchain_Linux_x64_V1.90/OpenOCD/bin && ./openocd -f wch-riscv.cfg -c init -c halt -c "flash write_image /root/ch32v00x.hex" -c exit'

.PHONY: reset
reset:
	ssh -t embed 'cd /root/MRS_Toolchain_Linux_x64_V1.90/OpenOCD/bin && ./openocd -f wch-riscv.cfg -c init  -c halt -c wlink_reset_resume -c exit'
