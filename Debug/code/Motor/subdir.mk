################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/motor/Motor.c" 

COMPILED_SRCS += \
"code/motor/Motor.src" 

C_DEPS += \
"./code/motor/Motor.d" 

OBJS += \
"code/motor/Motor.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/motor/Motor.src":"../code/motor/Motor.c" "code/motor/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/motor/Motor.o":"code/motor/Motor.src" "code/motor/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-motor

clean-code-2f-motor:
	-$(RM) ./code/motor/Motor.d ./code/motor/Motor.o ./code/motor/Motor.src

.PHONY: clean-code-2f-motor

