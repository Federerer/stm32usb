################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Target/STM32F0/Endpoint.cpp \
../Target/STM32F0/USBLowLevel_STM32F0.cpp 

OBJS += \
./Target/STM32F0/Endpoint.o \
./Target/STM32F0/USBLowLevel_STM32F0.o 

CPP_DEPS += \
./Target/STM32F0/Endpoint.d \
./Target/STM32F0/USBLowLevel_STM32F0.d 


# Each subdirectory must supply rules for building sources it contributes
Target/STM32F0/%.o: ../Target/STM32F0/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -std=c++14 -DSTM32F042x6 -I"C:\ARM3\USBDevice\Include" -I"C:\ARM3\USBDevice\Target\STM32F0" -I"C:\ARM3\USBDevice\Target\STM32F0\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


