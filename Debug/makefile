################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# 此处定义了参与生成的所有源
-include sources.mk
-include user/subdir.mk
-include libraries/zf_driver/subdir.mk
-include libraries/zf_device/subdir.mk
-include libraries/zf_components/subdir.mk
-include libraries/zf_common/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/_PinMap/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/_Lib/DataHandling/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/_Impl/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Src/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Scu/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Qspi/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Qspi/SpiMaster/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Port/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Pms/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Mtu/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Gtm/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Gtm/Atom/Pwm/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Gpt12/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Gpt12/IncrEnc/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Flash/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Evadc/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Evadc/Adc/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Dma/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Dma/Dma/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Cpu/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Cpu/Irq/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Ccu6/Timer/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Ccu6/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Asclin/Std/subdir.mk
-include libraries/infineon_libraries/iLLD/TC38A/Tricore/Asclin/Asc/subdir.mk
-include libraries/infineon_libraries/Service/CpuGeneric/SysSe/Math/subdir.mk
-include libraries/infineon_libraries/Service/CpuGeneric/SysSe/Bsp/subdir.mk
-include libraries/infineon_libraries/Service/CpuGeneric/StdIf/subdir.mk
-include libraries/infineon_libraries/Service/CpuGeneric/If/subdir.mk
-include libraries/infineon_libraries/Infra/Ssw/TC38A/Tricore/subdir.mk
-include libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk
-include libraries/infineon_libraries/Configurations/subdir.mk
-include code/wifiImage/subdir.mk
-include code/process/subdir.mk
-include code/pit/subdir.mk
-include code/pid/subdir.mk
-include code/myKey/subdir.mk
-include code/myEncoder/subdir.mk
-include code/myCamera/subdir.mk
-include code/menu/subdir.mk
-include code/leg/subdir.mk
-include code/icm42688/subdir.mk
-include code/gyro/subdir.mk
-include code/fps/subdir.mk
-include code/foc/subdir.mk
-include code/flash/subdir.mk
-include code/filter/subdir.mk
-include code/debugArg/subdir.mk
-include code/beep/subdir.mk
-include code/VOFA/subdir.mk
-include code/UART/subdir.mk
-include code/SysTimer/subdir.mk
-include code/Sys/subdir.mk
-include code/Status/subdir.mk
-include code/Motor/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Seekfree_TC387_Opensource_Library
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# 将这些工具调用的输入和输出添加到构建变量 
SECONDARY_SIZE += \
Seekfree_TC387_Opensource_Library.siz \


# 所有目标
all: main-build

# 主构建目标
main-build: Seekfree_TC387_Opensource_Library.elf secondary-outputs

# 工具调用
Seekfree_TC387_Opensource_Library.elf: $(OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在构建目标： $@'
	@echo '正在调用： TASKING Linker'
	cctc --lsl-file="../Lcf_Tasking_Tricore_Tc.lsl" -Wl-Oc -Wl-OL -Wl-Ot -Wl-Ox -Wl-Oy "E:\User\S\smartc\TC387_Library-master\Seekfree_TC387_Opensource_Library\libraries\zf_device\zf_device_config.a" -Wl--map-file="Seekfree_TC387_Opensource_Library.map" -Wl-mc -Wl-mf -Wl-mi -Wl-mk -Wl-ml -Wl-mm -Wl-md -Wl-mr -Wl-mu --no-warnings= -Wl--error-limit=42 --fp-model=3 -lrt -lcs_fpu --lsl-core=vtc --exceptions --strict --anachronisms --force-c++ -Ctc38x -o"Seekfree_TC387_Opensource_Library.elf" -Wl-o"Seekfree_TC387_Opensource_Library.hex:IHEX" $(OBJS)
	@echo '已结束构建目标： $@'
	@echo ' '

Seekfree_TC387_Opensource_Library.siz: Seekfree_TC387_Opensource_Library.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在调用： Print Size'
	elfsize  "Seekfree_TC387_Opensource_Library.elf"
	@echo '已结束构建： $@'
	@echo ' '

# 其他目标
clean:
	-$(RM) Seekfree_TC387_Opensource_Library.siz Seekfree_TC387_Opensource_Library.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
