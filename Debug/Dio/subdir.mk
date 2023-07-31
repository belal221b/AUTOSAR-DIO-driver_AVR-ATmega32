################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dio/Dio.c \
../Dio/Dio_PBcfg.c 

OBJS += \
./Dio/Dio.o \
./Dio/Dio_PBcfg.o 

C_DEPS += \
./Dio/Dio.d \
./Dio/Dio_PBcfg.d 


# Each subdirectory must supply rules for building sources it contributes
Dio/%.o: ../Dio/%.c Dio/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


