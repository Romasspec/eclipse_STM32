################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SPL/src/stm32f10x_exti.c \
../SPL/src/stm32f10x_gpio.c \
../SPL/src/stm32f10x_rcc.c 

OBJS += \
./SPL/src/stm32f10x_exti.o \
./SPL/src/stm32f10x_gpio.o \
./SPL/src/stm32f10x_rcc.o 

C_DEPS += \
./SPL/src/stm32f10x_exti.d \
./SPL/src/stm32f10x_gpio.d \
./SPL/src/stm32f10x_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
SPL/src/%.o: ../SPL/src/%.c SPL/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -DUSE_STDPERIPH_DRIVER -I"C:\Users\Romas\eclipse-workspace\proj2\startup" -I"C:\Users\Romas\eclipse-workspace\proj2\CMSIS" -I"C:\Users\Romas\eclipse-workspace\proj2\SPL\inc" -I"C:\Users\Romas\eclipse-workspace\proj2\SPL\src" -I"C:\Users\Romas\eclipse-workspace\proj2\src" -I"C:\Users\Romas\eclipse-workspace\proj2\CMSIS\CM3\CoreSupport" -I"C:\Users\Romas\eclipse-workspace\proj2\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


