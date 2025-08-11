################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/gyro/Gyro.c" \
"../code/gyro/Gyro_Process.c" 

COMPILED_SRCS += \
"code/gyro/Gyro.src" \
"code/gyro/Gyro_Process.src" 

C_DEPS += \
"./code/gyro/Gyro.d" \
"./code/gyro/Gyro_Process.d" 

OBJS += \
"code/gyro/Gyro.o" \
"code/gyro/Gyro_Process.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/gyro/Gyro.src":"../code/gyro/Gyro.c" "code/gyro/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/gyro/Gyro.o":"code/gyro/Gyro.src" "code/gyro/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/gyro/Gyro_Process.src":"../code/gyro/Gyro_Process.c" "code/gyro/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/gyro/Gyro_Process.o":"code/gyro/Gyro_Process.src" "code/gyro/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-gyro

clean-code-2f-gyro:
	-$(RM) ./code/gyro/Gyro.d ./code/gyro/Gyro.o ./code/gyro/Gyro.src ./code/gyro/Gyro_Process.d ./code/gyro/Gyro_Process.o ./code/gyro/Gyro_Process.src

.PHONY: clean-code-2f-gyro

