################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.c" 

COMPILED_SRCS += \
"libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.src" 

C_DEPS += \
"./libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.d" 

OBJS += \
"libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.src":"../libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.c" "libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.o":"libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.src" "libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC38A-2f-Tricore-2f-Stm-2f-Std

clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC38A-2f-Tricore-2f-Stm-2f-Std:
	-$(RM) ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.d ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.o ./libraries/infineon_libraries/iLLD/TC38A/Tricore/Stm/Std/IfxStm.src

.PHONY: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC38A-2f-Tricore-2f-Stm-2f-Std

