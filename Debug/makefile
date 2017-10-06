################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: demo_lucas_board.axf

# Tool invocations
demo_lucas_board.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Manuel Guerrero\TD2\WorkspaceTest2\lpc_board_nxp_lpcxpresso_1769\Debug" -L"C:\Users\Manuel Guerrero\TD2\WorkspaceTest2\lpc_chip_175x_6x\Debug" -Xlinker -Map="demo_lucas_board.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "demo_lucas_board_Debug.ld" -o "demo_lucas_board.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) demo_lucas_board.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "demo_lucas_board.axf"; # arm-none-eabi-objcopy -v -O binary "demo_lucas_board.axf" "demo_lucas_board.bin" ; # checksum -p LPC1769 -d "demo_lucas_board.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
