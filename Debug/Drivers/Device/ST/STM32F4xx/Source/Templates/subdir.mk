################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c 

OBJS += \
./Drivers/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o 

C_DEPS += \
./Drivers/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Device/ST/STM32F4xx/Source/Templates/%.o Drivers/Device/ST/STM32F4xx/Source/Templates/%.su Drivers/Device/ST/STM32F4xx/Source/Templates/%.cyclo: ../Drivers/Device/ST/STM32F4xx/Source/Templates/%.c Drivers/Device/ST/STM32F4xx/Source/Templates/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Drivers/CMSIS/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I"D:/STM32CubeIDE_1.17.0/teststm/Drivers/CMSIS/Lib/ARM" -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../STM32Cube/Repository/STM32Cube_FW_F4/Middlewares/Third_Party/ARM/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Device-2f-ST-2f-STM32F4xx-2f-Source-2f-Templates

clean-Drivers-2f-Device-2f-ST-2f-STM32F4xx-2f-Source-2f-Templates:
	-$(RM) ./Drivers/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.cyclo ./Drivers/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.d ./Drivers/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o ./Drivers/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.su

.PHONY: clean-Drivers-2f-Device-2f-ST-2f-STM32F4xx-2f-Source-2f-Templates

