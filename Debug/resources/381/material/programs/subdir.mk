################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../resources/381/material/programs/Example_FresnelPS.asm \
../resources/381/material/programs/OffsetMapping_specular.asm 

OBJS += \
./resources/381/material/programs/Example_FresnelPS.o \
./resources/381/material/programs/OffsetMapping_specular.o 


# Each subdirectory must supply rules for building sources it contributes
resources/381/material/programs/%.o: ../resources/381/material/programs/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


