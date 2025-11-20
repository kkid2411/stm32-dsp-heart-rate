################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_f32.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q15.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q31.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q7.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_f32.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q15.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q31.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q7.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.c \
../Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.c 

OBJS += \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_f32.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q15.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q31.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q7.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_f32.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q15.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q31.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q7.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.o \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.o 

C_DEPS += \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_f32.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q15.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q31.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q7.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_f32.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q15.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q31.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q7.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.d \
./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DSP_Lib/Source/SupportFunctions/%.o Drivers/DSP_Lib/Source/SupportFunctions/%.su Drivers/DSP_Lib/Source/SupportFunctions/%.cyclo: ../Drivers/DSP_Lib/Source/SupportFunctions/%.c Drivers/DSP_Lib/Source/SupportFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../STM32Cube/Repository/STM32Cube_FW_F4/Middlewares/Third_Party/ARM/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DSP_Lib-2f-Source-2f-SupportFunctions

clean-Drivers-2f-DSP_Lib-2f-Source-2f-SupportFunctions:
	-$(RM) ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_f32.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_f32.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_f32.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_f32.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q15.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q15.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q15.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q15.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q31.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q31.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q31.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q31.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q7.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q7.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q7.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_copy_q7.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_f32.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_f32.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_f32.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_f32.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q15.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q15.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q15.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q15.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q31.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q31.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q31.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q31.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q7.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q7.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q7.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_fill_q7.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q15.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q31.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_float_to_q7.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_float.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q31.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q15_to_q7.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_float.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q15.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q31_to_q7.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_float.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q15.su ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.cyclo ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.d ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.o ./Drivers/DSP_Lib/Source/SupportFunctions/arm_q7_to_q31.su

.PHONY: clean-Drivers-2f-DSP_Lib-2f-Source-2f-SupportFunctions

