################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/srccpp/BussinessLogic/ApplicationSW.cpp 

OBJS += \
./Core/srccpp/BussinessLogic/ApplicationSW.o 

CPP_DEPS += \
./Core/srccpp/BussinessLogic/ApplicationSW.d 


# Each subdirectory must supply rules for building sources it contributes
Core/srccpp/BussinessLogic/%.o Core/srccpp/BussinessLogic/%.su: ../Core/srccpp/BussinessLogic/%.cpp Core/srccpp/BussinessLogic/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L433xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/BussinessLogic" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/Common" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/DisplayCPP" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/Ethernet" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/Modbus" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/OfflineStorage" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/Shift" -I"D:/PROGRAM/Naveen/melting/Spectrumresult_16_12_23 - Copy/Core/srccpp/WifiEsp8266" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-srccpp-2f-BussinessLogic

clean-Core-2f-srccpp-2f-BussinessLogic:
	-$(RM) ./Core/srccpp/BussinessLogic/ApplicationSW.d ./Core/srccpp/BussinessLogic/ApplicationSW.o ./Core/srccpp/BussinessLogic/ApplicationSW.su

.PHONY: clean-Core-2f-srccpp-2f-BussinessLogic

