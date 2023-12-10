SHELL       := /bin/bash
OPENOCD_DIR := /root/MRS_Toolchain_Linux_x64_V1.90/OpenOCD
HOST        := embed
HEX         := ch32v00x.hex
TMP         := /tmp/
GDB_PORT    := 3333

.PHONY: all
all: build sync flash reset

.PHONY: build
build:
	cd build && make

.PHONY: sync
sync:
	cd build && scp "$(HEX)" $(HOST):"$(TMP)"

.PHONY: kill
kill:
	ssh -t $(HOST) 'kill $$(ps aux | grep openocd | grep -v grep | awk "{print \$$1}") 2>/dev/null || true'

.PHONY: flash
flash: kill
	ssh -t $(HOST) 'cd "$(OPENOCD_DIR)/bin" && ./openocd -f wch-riscv.cfg -c init -c halt -c "flash write_image $(TMP)/$(HEX)" -c exit'

.PHONY: reset
reset: kill
	ssh -t $(HOST) 'cd "$(OPENOCD_DIR)/bin" && ./openocd -f wch-riscv.cfg -c init -c halt -c wlink_reset_resume -c exit'

.PHONY: debug
debug: kill
	ssh -t $(HOST) 'cd "$(OPENOCD_DIR)/bin" && ./openocd -f wch-riscv.cfg' & ssh -NL localhost:$(GDB_PORT):localhost:$(GDB_PORT) -o ExitOnForwardFailure=yes $(HOST) & wait
