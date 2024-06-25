################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/liquidcrystal_i2c.c \
../Src/main.c 

OBJS += \
./Src/liquidcrystal_i2c.o \
./Src/main.o 

C_DEPS += \
./Src/liquidcrystal_i2c.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F429ZITx -DSTM32F4 -DNUCLEO_F429ZI -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Inc -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/CMSIS/Include" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/liquidcrystal_i2c.cyclo ./Src/liquidcrystal_i2c.d ./Src/liquidcrystal_i2c.o ./Src/liquidcrystal_i2c.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su

.PHONY: clean-Src

