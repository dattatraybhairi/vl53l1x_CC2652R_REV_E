################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../cc13x2_cc26x2_tirtos.cmd 

SYSCFG_SRCS += \
../empty.syscfg 

C_SRCS += \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c \
../main_tirtos.c \
../vl53l1x_application.c 

GEN_FILES += \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./syscfg/ti_devices_config.d \
./syscfg/ti_drivers_config.d \
./main_tirtos.d \
./vl53l1x_application.d 

OBJS += \
./syscfg/ti_devices_config.obj \
./syscfg/ti_drivers_config.obj \
./main_tirtos.obj \
./vl53l1x_application.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/syscfg_c.rov.xs \
./syscfg/ti_utils_runtime_model.gv \
./syscfg/ti_utils_runtime_Makefile 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"syscfg\ti_devices_config.obj" \
"syscfg\ti_drivers_config.obj" \
"main_tirtos.obj" \
"vl53l1x_application.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.genlibs" \
"syscfg\syscfg_c.rov.xs" \
"syscfg\ti_utils_runtime_model.gv" \
"syscfg\ti_utils_runtime_Makefile" 

C_DEPS__QUOTED += \
"syscfg\ti_devices_config.d" \
"syscfg\ti_drivers_config.d" \
"main_tirtos.d" \
"vl53l1x_application.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_devices_config.c" \
"syscfg\ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../empty.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_devices_config.c" \
"./syscfg/ti_drivers_config.c" \
"../main_tirtos.c" \
"../vl53l1x_application.c" 


