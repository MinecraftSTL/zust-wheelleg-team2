################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/Sys/Sys.c" 

COMPILED_SRCS += \
"code/Sys/Sys.src" 

C_DEPS += \
"./code/Sys/Sys.d" 

OBJS += \
"code/Sys/Sys.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/Sys/Sys.src":"../code/Sys/Sys.c" "code/Sys/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/Sys/Sys.o":"code/Sys/Sys.src" "code/Sys/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Sys

clean-code-2f-Sys:
	-$(RM) ./code/Sys/Sys.d ./code/Sys/Sys.o ./code/Sys/Sys.src

.PHONY: clean-code-2f-Sys

