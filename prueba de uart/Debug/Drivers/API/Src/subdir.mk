################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_debounce.c \
../Drivers/API/Src/API_delay.c \
../Drivers/API/Src/API_gps.c \
../Drivers/API/Src/API_hd44780.c \
../Drivers/API/Src/API_hd44780_port.c \
../Drivers/API/Src/API_i2c.c \
../Drivers/API/Src/API_pc.c \
../Drivers/API/Src/API_uart.c 

OBJS += \
./Drivers/API/Src/API_debounce.o \
./Drivers/API/Src/API_delay.o \
./Drivers/API/Src/API_gps.o \
./Drivers/API/Src/API_hd44780.o \
./Drivers/API/Src/API_hd44780_port.o \
./Drivers/API/Src/API_i2c.o \
./Drivers/API/Src/API_pc.o \
./Drivers/API/Src/API_uart.o 

C_DEPS += \
./Drivers/API/Src/API_debounce.d \
./Drivers/API/Src/API_delay.d \
./Drivers/API/Src/API_gps.d \
./Drivers/API/Src/API_hd44780.d \
./Drivers/API/Src/API_hd44780_port.d \
./Drivers/API/Src/API_i2c.d \
./Drivers/API/Src/API_pc.d \
./Drivers/API/Src/API_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o Drivers/API/Src/%.su Drivers/API/Src/%.cyclo: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/felipe/Desktop/PdM_workspace/prueba de uart/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/felipe/Desktop/PdM_workspace/prueba de uart/Drivers/CMSIS/Include" -I"/home/felipe/Desktop/PdM_workspace/prueba de uart/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/felipe/Desktop/PdM_workspace/prueba de uart/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/felipe/Desktop/PdM_workspace/prueba de uart/Drivers/Core/Inc" -I"/home/felipe/Desktop/PdM_workspace/prueba de uart/Drivers/API/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src

clean-Drivers-2f-API-2f-Src:
	-$(RM) ./Drivers/API/Src/API_debounce.cyclo ./Drivers/API/Src/API_debounce.d ./Drivers/API/Src/API_debounce.o ./Drivers/API/Src/API_debounce.su ./Drivers/API/Src/API_delay.cyclo ./Drivers/API/Src/API_delay.d ./Drivers/API/Src/API_delay.o ./Drivers/API/Src/API_delay.su ./Drivers/API/Src/API_gps.cyclo ./Drivers/API/Src/API_gps.d ./Drivers/API/Src/API_gps.o ./Drivers/API/Src/API_gps.su ./Drivers/API/Src/API_hd44780.cyclo ./Drivers/API/Src/API_hd44780.d ./Drivers/API/Src/API_hd44780.o ./Drivers/API/Src/API_hd44780.su ./Drivers/API/Src/API_hd44780_port.cyclo ./Drivers/API/Src/API_hd44780_port.d ./Drivers/API/Src/API_hd44780_port.o ./Drivers/API/Src/API_hd44780_port.su ./Drivers/API/Src/API_i2c.cyclo ./Drivers/API/Src/API_i2c.d ./Drivers/API/Src/API_i2c.o ./Drivers/API/Src/API_i2c.su ./Drivers/API/Src/API_pc.cyclo ./Drivers/API/Src/API_pc.d ./Drivers/API/Src/API_pc.o ./Drivers/API/Src/API_pc.su ./Drivers/API/Src/API_uart.cyclo ./Drivers/API/Src/API_uart.d ./Drivers/API/Src/API_uart.o ./Drivers/API/Src/API_uart.su

.PHONY: clean-Drivers-2f-API-2f-Src

