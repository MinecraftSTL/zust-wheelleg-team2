################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../libraries/zf_common/zf_common_clock.c" \
"../libraries/zf_common/zf_common_debug.c" \
"../libraries/zf_common/zf_common_fifo.c" \
"../libraries/zf_common/zf_common_font.c" \
"../libraries/zf_common/zf_common_function.c" \
"../libraries/zf_common/zf_common_interrupt.c" 

COMPILED_SRCS += \
"libraries/zf_common/zf_common_clock.src" \
"libraries/zf_common/zf_common_debug.src" \
"libraries/zf_common/zf_common_fifo.src" \
"libraries/zf_common/zf_common_font.src" \
"libraries/zf_common/zf_common_function.src" \
"libraries/zf_common/zf_common_interrupt.src" 

C_DEPS += \
"./libraries/zf_common/zf_common_clock.d" \
"./libraries/zf_common/zf_common_debug.d" \
"./libraries/zf_common/zf_common_fifo.d" \
"./libraries/zf_common/zf_common_font.d" \
"./libraries/zf_common/zf_common_function.d" \
"./libraries/zf_common/zf_common_interrupt.d" 

OBJS += \
"libraries/zf_common/zf_common_clock.o" \
"libraries/zf_common/zf_common_debug.o" \
"libraries/zf_common/zf_common_fifo.o" \
"libraries/zf_common/zf_common_font.o" \
"libraries/zf_common/zf_common_function.o" \
"libraries/zf_common/zf_common_interrupt.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"libraries/zf_common/zf_common_clock.src":"../libraries/zf_common/zf_common_clock.c" "libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/zf_common/zf_common_clock.o":"libraries/zf_common/zf_common_clock.src" "libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/zf_common/zf_common_debug.src":"../libraries/zf_common/zf_common_debug.c" "libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/zf_common/zf_common_debug.o":"libraries/zf_common/zf_common_debug.src" "libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/zf_common/zf_common_fifo.src":"../libraries/zf_common/zf_common_fifo.c" "libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/zf_common/zf_common_fifo.o":"libraries/zf_common/zf_common_fifo.src" "libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/zf_common/zf_common_font.src":"../libraries/zf_common/zf_common_font.c" "libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/zf_common/zf_common_font.o":"libraries/zf_common/zf_common_font.src" "libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/zf_common/zf_common_function.src":"../libraries/zf_common/zf_common_function.c" "libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/zf_common/zf_common_function.o":"libraries/zf_common/zf_common_function.src" "libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/zf_common/zf_common_interrupt.src":"../libraries/zf_common/zf_common_interrupt.c" "libraries/zf_common/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/zf_common/zf_common_interrupt.o":"libraries/zf_common/zf_common_interrupt.src" "libraries/zf_common/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-libraries-2f-zf_common

clean-libraries-2f-zf_common:
	-$(RM) ./libraries/zf_common/zf_common_clock.d ./libraries/zf_common/zf_common_clock.o ./libraries/zf_common/zf_common_clock.src ./libraries/zf_common/zf_common_debug.d ./libraries/zf_common/zf_common_debug.o ./libraries/zf_common/zf_common_debug.src ./libraries/zf_common/zf_common_fifo.d ./libraries/zf_common/zf_common_fifo.o ./libraries/zf_common/zf_common_fifo.src ./libraries/zf_common/zf_common_font.d ./libraries/zf_common/zf_common_font.o ./libraries/zf_common/zf_common_font.src ./libraries/zf_common/zf_common_function.d ./libraries/zf_common/zf_common_function.o ./libraries/zf_common/zf_common_function.src ./libraries/zf_common/zf_common_interrupt.d ./libraries/zf_common/zf_common_interrupt.o ./libraries/zf_common/zf_common_interrupt.src

.PHONY: clean-libraries-2f-zf_common

