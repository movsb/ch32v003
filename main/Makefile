SHELL       := /bin/bash
OPENOCD_DIR := /Users/tao/code/toolchains/openocd
HEX         := ch32v00x.hex
TMP         := /tmp/
GDB_PORT    := 3333
WCH_CFG     := ./etc/wch-riscv.cfg
BUILD       := build

.DEFAULT_GOAL := all

.PHONY: clean
clean:
	[ -d "$(BUILD)" ] && cd "$(BUILD)" && rm -rf * || :

.PHONY: cmake
cmake:
	mkdir -p "$(BUILD)" && cd "$(BUILD)" && cmake ..

.PHONY: all
all: build sync flash reset

.PHONY: build
build:
	cd "$(BUILD)" && make

# TODO remove
.PHONY: sync
sync:
	cp "$(BUILD)/$(HEX)" "$(TMP)"

.PHONY: kill
kill:
	kill $(ps aux | grep openocd | grep -v grep | awk '{print $1}') 2>/dev/null || true

.PHONY: flash sync
flash: kill
	cd "$(OPENOCD_DIR)" && ./bin/openocd -f "$(WCH_CFG)" -c init -c halt -c "flash write_image $(TMP)/$(HEX)" -c exit

.PHONY: reset
reset: kill
	cd "$(OPENOCD_DIR)" && ./bin/openocd -f "$(WCH_CFG)" -c init -c halt -c wlink_reset_resume -c exit

.PHONY: debug
debug: kill
	cd "$(OPENOCD_DIR)" && ./bin/openocd -f "$(WCH_CFG)"
