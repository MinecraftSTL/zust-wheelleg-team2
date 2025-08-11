################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/pit/Pit.c" 

COMPILED_SRCS += \
"code/pit/Pit.src" 

C_DEPS += \
"./code/pit/Pit.d" 

OBJS += \
"code/pit/Pit.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/pit/Pit.src":"../code/pit/Pit.c" "code/pit/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/pit/Pit.o":"code/pit/Pit.src" "code/pit/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-pit

clean-code-2f-pit:
	-$(RM) ./code/pit/Pit.d ./code/pit/Pit.o ./code/pit/Pit.src

.PHONY: clean-code-2f-pit

