################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f745xx.s \
../Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f746xx.s \
../Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f756xx.s 

OBJS += \
./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f745xx.o \
./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f746xx.o \
./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f756xx.o 

S_DEPS += \
./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f745xx.d \
./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f746xx.d \
./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f756xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Device/ST/STM32F7xx/Source/Templates/arm/%.o: ../Drivers/Device/ST/STM32F7xx/Source/Templates/arm/%.s Drivers/Device/ST/STM32F7xx/Source/Templates/arm/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Drivers-2f-Device-2f-ST-2f-STM32F7xx-2f-Source-2f-Templates-2f-arm

clean-Drivers-2f-Device-2f-ST-2f-STM32F7xx-2f-Source-2f-Templates-2f-arm:
	-$(RM) ./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f745xx.d ./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f745xx.o ./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f746xx.d ./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f746xx.o ./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f756xx.d ./Drivers/Device/ST/STM32F7xx/Source/Templates/arm/startup_stm32f756xx.o

.PHONY: clean-Drivers-2f-Device-2f-ST-2f-STM32F7xx-2f-Source-2f-Templates-2f-arm

