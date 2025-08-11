################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/UART/UART.c" 

COMPILED_SRCS += \
"code/UART/UART.src" 

C_DEPS += \
"./code/UART/UART.d" 

OBJS += \
"code/UART/UART.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/UART/UART.src":"../code/UART/UART.c" "code/UART/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/UART/UART.o":"code/UART/UART.src" "code/UART/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-UART

clean-code-2f-UART:
	-$(RM) ./code/UART/UART.d ./code/UART/UART.o ./code/UART/UART.src

.PHONY: clean-code-2f-UART

