################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f429zitx.s 

OBJS += \
./Startup/startup_stm32f429zitx.o 

S_DEPS += \
./Startup/startup_stm32f429zitx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/CMSIS/Include" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Inc" -I"/home/felipe/Desktop/PdM_workspace/TP Final/Drivers/Core/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32f429zitx.d ./Startup/startup_stm32f429zitx.o

.PHONY: clean-Startup

