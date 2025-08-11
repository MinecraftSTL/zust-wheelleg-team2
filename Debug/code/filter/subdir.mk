################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/filter/Filter.c" \
"../code/filter/HPF1.c" \
"../code/filter/LPF0.c" \
"../code/filter/LPF1.c" \
"../code/filter/LPF2.c" 

COMPILED_SRCS += \
"code/filter/Filter.src" \
"code/filter/HPF1.src" \
"code/filter/LPF0.src" \
"code/filter/LPF1.src" \
"code/filter/LPF2.src" 

C_DEPS += \
"./code/filter/Filter.d" \
"./code/filter/HPF1.d" \
"./code/filter/LPF0.d" \
"./code/filter/LPF1.d" \
"./code/filter/LPF2.d" 

OBJS += \
"code/filter/Filter.o" \
"code/filter/HPF1.o" \
"code/filter/LPF0.o" \
"code/filter/LPF1.o" \
"code/filter/LPF2.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/filter/Filter.src":"../code/filter/Filter.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/Filter.o":"code/filter/Filter.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/filter/HPF1.src":"../code/filter/HPF1.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/HPF1.o":"code/filter/HPF1.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/filter/LPF0.src":"../code/filter/LPF0.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/LPF0.o":"code/filter/LPF0.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/filter/LPF1.src":"../code/filter/LPF1.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/LPF1.o":"code/filter/LPF1.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/filter/LPF2.src":"../code/filter/LPF2.c" "code/filter/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/filter/LPF2.o":"code/filter/LPF2.src" "code/filter/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-filter

clean-code-2f-filter:
	-$(RM) ./code/filter/Filter.d ./code/filter/Filter.o ./code/filter/Filter.src ./code/filter/HPF1.d ./code/filter/HPF1.o ./code/filter/HPF1.src ./code/filter/LPF0.d ./code/filter/LPF0.o ./code/filter/LPF0.src ./code/filter/LPF1.d ./code/filter/LPF1.o ./code/filter/LPF1.src ./code/filter/LPF2.d ./code/filter/LPF2.o ./code/filter/LPF2.src

.PHONY: clean-code-2f-filter

