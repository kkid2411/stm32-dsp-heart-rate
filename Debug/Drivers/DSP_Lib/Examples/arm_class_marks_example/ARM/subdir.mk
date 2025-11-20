################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/arm_class_marks_example_f32.c 

OBJS += \
./Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/arm_class_marks_example_f32.o 

C_DEPS += \
./Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/arm_class_marks_example_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/%.o Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/%.su Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/%.cyclo: ../Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/%.c Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../STM32Cube/Repository/STM32Cube_FW_F4/Middlewares/Third_Party/ARM/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DSP_Lib-2f-Examples-2f-arm_class_marks_example-2f-ARM

clean-Drivers-2f-DSP_Lib-2f-Examples-2f-arm_class_marks_example-2f-ARM:
	-$(RM) ./Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/arm_class_marks_example_f32.cyclo ./Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/arm_class_marks_example_f32.d ./Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/arm_class_marks_example_f32.o ./Drivers/DSP_Lib/Examples/arm_class_marks_example/ARM/arm_class_marks_example_f32.su

.PHONY: clean-Drivers-2f-DSP_Lib-2f-Examples-2f-arm_class_marks_example-2f-ARM

