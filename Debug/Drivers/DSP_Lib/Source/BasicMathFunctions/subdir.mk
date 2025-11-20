################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.c \
../Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.c 

OBJS += \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.o \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.o 

C_DEPS += \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.d \
./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DSP_Lib/Source/BasicMathFunctions/%.o Drivers/DSP_Lib/Source/BasicMathFunctions/%.su Drivers/DSP_Lib/Source/BasicMathFunctions/%.cyclo: ../Drivers/DSP_Lib/Source/BasicMathFunctions/%.c Drivers/DSP_Lib/Source/BasicMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"../STM32Cube/Repository/STM32Cube_FW_F4/Middlewares/Third_Party/ARM/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DSP_Lib-2f-Source-2f-BasicMathFunctions

clean-Drivers-2f-DSP_Lib-2f-Source-2f-BasicMathFunctions:
	-$(RM) ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_abs_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_add_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_dot_prod_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_mult_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_negate_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_offset_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.d
	-$(RM) ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_scale_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_shift_q7.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_f32.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q15.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q31.su ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.cyclo ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.d ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.o ./Drivers/DSP_Lib/Source/BasicMathFunctions/arm_sub_q7.su

.PHONY: clean-Drivers-2f-DSP_Lib-2f-Source-2f-BasicMathFunctions

