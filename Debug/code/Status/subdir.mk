################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/status/Status.c" 

COMPILED_SRCS += \
"code/status/Status.src" 

C_DEPS += \
"./code/status/Status.d" 

OBJS += \
"code/status/Status.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/status/Status.src":"../code/status/Status.c" "code/status/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/status/Status.o":"code/status/Status.src" "code/status/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-status

clean-code-2f-status:
	-$(RM) ./code/status/Status.d ./code/status/Status.o ./code/status/Status.src

.PHONY: clean-code-2f-status

