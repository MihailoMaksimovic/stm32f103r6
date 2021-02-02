BUILD_DIR = build

TARGET = 05_Tajmer_program

SOURCES_S = \
startup_code.s

SOURCES_C = \
Src/main.c

LINKER_SCRIPT = linker_script.ld

PREFIX = C:\Users\mihai\OneDrive\Desktop\domaci 2 i 3\domaci3\ARM Embedded GCC\8.3.1-1.1\bin

CC = $(PREFIX)/arm-none-eabi-gcc.exe -c
AS = $(PREFIX)/arm-none-eabi-as.exe
LD = $(PREFIX)/arm-none-eabi-ld.exe
OBJCOPY = $(PREFIX)/arm-none-eabi-objcopy.exe

MCU = -mcpu=cortex-m3 -mthumb

OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES_S:.s=.o))
vpath %.s $(sort $(dir $(SOURCES_S)))

OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES_C:.c=.o)))
vpath %.c $(sort $(dir $(SOURCES_C)))

all : $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR)/$(TARGET).elf : $(OBJECTS) makefile
	$(LD) -T $(LINKER_SCRIPT) -o $(@) $(OBJECTS)

$(BUILD_DIR)/$(TARGET).hex : $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(OBJCOPY) --output-target ihex $(<) $(@)

$(BUILD_DIR)/%.o : %.s makefile | $(BUILD_DIR)
	$(AS) $(MCU) -o $(@) $(<)

$(BUILD_DIR)/%.o : %.c makefile | $(BUILD_DIR)
	$(CC) $(MCU) -o $(@) $(<)

$(BUILD_DIR) :
	mkdir $(@)

clean :
	rm -rf $(BUILD_DIR)
