.PHONY: all build clean flash help sim sim-run sim-clean

BUILD_DIR = build
UF2_FILE = $(BUILD_DIR)/card_shuffler.uf2
ARM_TOOLCHAIN = /Applications/ArmGNUToolchain/15.2.rel1/arm-none-eabi

all: build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DPICO_TOOLCHAIN_PATH=$(ARM_TOOLCHAIN) .. && make -j4
	@echo ""
	@echo "Done: $(UF2_FILE)"

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned"

flash: build
	@if [ -d "/Volumes/RPI-RP2" ]; then \
		cp $(UF2_FILE) /Volumes/RPI-RP2/; \
		echo "Firmware copied to RPI-RP2"; \
	else \
		echo "Error: RPI-RP2 drive not found"; \
		echo "Hold BOOT and connect USB"; \
	fi

sim:
	@$(MAKE) -f Makefile.sim

sim-run:
	@$(MAKE) -f Makefile.sim run

sim-clean:
	@$(MAKE) -f Makefile.sim clean

help:
	@echo "Card Shuffler commands:"
	@echo ""
	@echo "  make           Build firmware"
	@echo "  make clean     Remove build directory"
	@echo "  make flash     Build and flash"
	@echo "  make sim       Build SDL simulator"
	@echo "  make sim-run   Run SDL simulator"
	@echo "  make sim-clean Clean simulator build"
	@echo "  make help      Show this help"
