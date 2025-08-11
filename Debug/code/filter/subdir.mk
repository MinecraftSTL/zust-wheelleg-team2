################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/filter/Filter.c" \
"../code/filter/HPF.c" \
"../code/filter/LPF.c" 

COMPILED_SRCS += \
"code/filter/Filter.src" \
"code/filter/HPF.src" \
"code/filter/LPF.src" 

C_DEPS += \
"./code/filter/Filter.d" \
"./code/filter/HPF.d" \
"./code/filter/LPF.d" 

OBJS += \
"code/filter/Filter.o" \
"code/filter/HPF.o" \
"code/filter/LPF.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/filter/Filter.src":"../code/filter/Filter.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/Filter.o":"code/filter/Filter.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/filter/HPF.src":"../code/filter/HPF.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/HPF.o":"code/filter/HPF.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/filter/LPF.src":"../code/filter/LPF.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/LPF.o":"code/filter/LPF.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-filter

clean-code-2f-filter:
	-$(RM) ./code/filter/Filter.d ./code/filter/Filter.o ./code/filter/Filter.src ./code/filter/HPF.d ./code/filter/HPF.o ./code/filter/HPF.src ./code/filter/LPF.d ./code/filter/LPF.o ./code/filter/LPF.src

.PHONY: clean-code-2f-filter

