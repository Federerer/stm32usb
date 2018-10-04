################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../USBDevice.cpp \
../USBDeviceConfigration.cpp \
../USBHID.cpp 

OBJS += \
./USBDevice.o \
./USBDeviceConfigration.o \
./USBHID.o 

CPP_DEPS += \
./USBDevice.d \
./USBDeviceConfigration.d \
./USBHID.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-none-eabi-g++ -std=c++14 -DSTM32F042x6 -I"C:\ARM3\USBDevice\Include" -I"C:\ARM3\USBDevice\Target\STM32F0" -I"C:\ARM3\USBDevice\Target\STM32F0\inc" -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


