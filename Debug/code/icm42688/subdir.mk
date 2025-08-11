################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/icm42688/icm42688.c" 

COMPILED_SRCS += \
"code/icm42688/icm42688.src" 

C_DEPS += \
"./code/icm42688/icm42688.d" 

OBJS += \
"code/icm42688/icm42688.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/icm42688/icm42688.src":"../code/icm42688/icm42688.c" "code/icm42688/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/icm42688/icm42688.o":"code/icm42688/icm42688.src" "code/icm42688/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-icm42688

clean-code-2f-icm42688:
	-$(RM) ./code/icm42688/icm42688.d ./code/icm42688/icm42688.o ./code/icm42688/icm42688.src

.PHONY: clean-code-2f-icm42688

