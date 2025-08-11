################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/debug/Debug.c" 

COMPILED_SRCS += \
"code/debug/Debug.src" 

C_DEPS += \
"./code/debug/Debug.d" 

OBJS += \
"code/debug/Debug.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/debug/Debug.src":"../code/debug/Debug.c" "code/debug/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/debug/Debug.o":"code/debug/Debug.src" "code/debug/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-debug

clean-code-2f-debug:
	-$(RM) ./code/debug/Debug.d ./code/debug/Debug.o ./code/debug/Debug.src

.PHONY: clean-code-2f-debug

