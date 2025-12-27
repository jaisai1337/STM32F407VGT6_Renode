DEVICE = STM32F407xx
PROJECT = TIMER_INTERRUPT

# Compiler and tools
CC = arm-none-eabi-gcc
CPP = arm-none-eabi-g++
AS = arm-none-eabi-gcc -x assembler-with-cpp
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Linker script
LINKER_SCRIPT = STM32F407VGTx_FLASH.ld

# Directories
BUILD_DIR = build
SRC_DIR = Core/Src
SYS_DIR = Core/Src/System

# Include directories
INC_DIR = \
    -I Core/Inc \
    -I Core/Inc/CMSIS/Include \

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
SYS_SRCS = \
			$(wildcard $(SYS_DIR)/*.c) \
			$(wildcard $(SYS_DIR)/*.s)

# Object files
OBJS = \
		$(patsubst $(SYS_DIR)/%.s,$(BUILD_DIR)/$(SYS_DIR)/%.o,$(filter %.s,$(SYS_SRCS))) \
		$(patsubst $(SYS_DIR)/%.c,$(BUILD_DIR)/$(SYS_DIR)/%.o,$(filter %.c,$(SYS_SRCS))) \
		$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/$(SRC_DIR)/%.o,$(filter %.c,$(SRCS)))

# MPU settings
MPU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

# Enable floating point support in printf/scanf
FLOATING = -u _printf_float -u _scanf_float

# Compiler flags
CFLAGS = $(MPU) -g -std=gnu11 -D$(DEVICE) -MMD -MP $(INC_DIR) -O3 -Wall -fdata-sections -ffunction-sections

# Linker flags
LDFLAGS = $(MPU) -T$(LINKER_SCRIPT) -specs=nano.specs -lc -lm -lnosys
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map -Wl,--gc-sections


# Default target
all: $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex

# Create build directory if not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRC_DIR) $(BUILD_DIR)/$(SYS_DIR)

# Compile source files
$(BUILD_DIR)/$(SYS_DIR)/%.o: $(SYS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(SYS_DIR)/%.o: $(SYS_DIR)/%.s
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Final build
$(BUILD_DIR)/$(PROJECT).elf: $(BUILD_DIR) $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	+@make -s print_size

# Create hex and bin files
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O binary -S $< $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)


print_size: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Memory usage summary:"
	@$(SIZE) -B $(BUILD_DIR)/$(PROJECT).elf
	@$(SIZE) -B -A $(BUILD_DIR)/$(PROJECT).elf | awk '\
		/\.isr_vector/     { flash += $$2 } \
		/\.text/           { flash += $$2 } \
		/\.rodata/         { flash += $$2 } \
		/\.ARM/ 		   { flash += $$2 } \
		/\.ARM.extab/      { flash += $$2 } \
		/\.ARM.exidx/      { flash += $$2 } \
		/\.init_array/     { flash += $$2 } \
		/\.fini_array/     { flash += $$2 } \
		/\.data/           { flash += $$2; ram += $$2 } \
		/\.bss/            { ram += $$2 } \
		/_user_heap_stack/ { ram += $$2 } \
		/\.ccmram/         { ccm += $$2 } \
		END { \
			flash_total = 1048576; \
			ram_total   = 131072; \
			ccm_total   = 65536; \
			\
			ram_perc    = ram / ram_total * 100; \
			ccm_perc    = ccm / ccm_total * 100; \
			flash_perc  = flash / flash_total * 100; \
			\
			ram_bar     = int(ram_perc / 10); \
			ccm_bar     = int(ccm_perc / 10); \
			flash_bar   = int(flash_perc / 10); \
			\
			printf "RAM:     [%s%s] %5.1f%% (used %d of %d bytes)\n", \
				repeat("#", ram_bar), repeat(" ", 10 - ram_bar), \
				ram_perc, ram, ram_total; \
			printf "CCM:     [%s%s] %5.1f%% (used %d of %d bytes)\n", \
				repeat("#", ccm_bar), repeat(" ", 10 - ccm_bar), \
				ccm_perc, ccm, ccm_total; \
			printf "Flash:   [%s%s] %5.1f%% (used %d of %d bytes)\n", \
				repeat("#", flash_bar), repeat(" ", 10 - flash_bar), \
				flash_perc, flash, flash_total; \
		} \
		function repeat(s, n, r) { \
			r = ""; for (i = 0; i < n; i++) r = r s; return r; \
		}'

.PHONY: all clean print_size
