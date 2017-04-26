################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Aspect.cpp \
../src/Entity381.cpp \
../src/EntityMgr.cpp \
../src/GfxMgr.cpp \
../src/as3.cpp \
../src/engine.cpp \
../src/inputMgr.cpp \
../src/mgr.cpp 

OBJS += \
./src/Aspect.o \
./src/Entity381.o \
./src/EntityMgr.o \
./src/GfxMgr.o \
./src/as3.o \
./src/engine.o \
./src/inputMgr.o \
./src/mgr.o 

CPP_DEPS += \
./src/Aspect.d \
./src/Entity381.d \
./src/EntityMgr.d \
./src/GfxMgr.d \
./src/as3.d \
./src/engine.d \
./src/inputMgr.d \
./src/mgr.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I"/home/sushil/workspace/as3/inc" -I/usr/local/include/OGRE/Overlay -I/usr/include/OIS -I/usr/local/include -I/usr/local/include/OGRE -I/usr/include -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


