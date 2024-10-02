################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/CM3/CoreSupport/core_cm3.c 

OBJS += \
./CMSIS/CM3/CoreSupport/core_cm3.o 

C_DEPS += \
./CMSIS/CM3/CoreSupport/core_cm3.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/CM3/CoreSupport/%.o: ../CMSIS/CM3/CoreSupport/%.c CMSIS/CM3/CoreSupport/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -DUSE_STDPERIPH_DRIVER -I"C:\Users\Romas\eclipse-workspace\proj2\startup" -I"C:\Users\Romas\eclipse-workspace\proj2\CMSIS" -I"C:\Users\Romas\eclipse-workspace\proj2\SPL\inc" -I"C:\Users\Romas\eclipse-workspace\proj2\SPL\src" -I"C:\Users\Romas\eclipse-workspace\proj2\src" -I"C:\Users\Romas\eclipse-workspace\proj2\CMSIS\CM3\CoreSupport" -I"C:\Users\Romas\eclipse-workspace\proj2\CMSIS\CM3\DeviceSupport\ST\STM32F10x" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


