################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/GPIO.c \
../src/LCD.c \
../src/Pulsador.c \
../src/RTC.c \
../src/Teclado.c \
../src/UART.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/demo_lucas_board.c \
../src/sysinit.c 

OBJS += \
./src/GPIO.o \
./src/LCD.o \
./src/Pulsador.o \
./src/RTC.o \
./src/Teclado.o \
./src/UART.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/demo_lucas_board.o \
./src/sysinit.o 

C_DEPS += \
./src/GPIO.d \
./src/LCD.d \
./src/Pulsador.d \
./src/RTC.d \
./src/Teclado.d \
./src/UART.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/demo_lucas_board.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
<<<<<<< HEAD
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Manuel Guerrero\WorkspaceLPC\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\Manuel Guerrero\WorkspaceLPC\lib_lucas_board\inc" -I"C:\Users\Manuel Guerrero\WorkspaceLPC\lpc_chip_175x_6x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
=======
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC17XX__ -D__REDLIB__ -I"C:\Datos\RepoLocal\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Datos\RepoLocal\lpc_chip_175x_6x\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
>>>>>>> master
	@echo 'Finished building: $<'
	@echo ' '


