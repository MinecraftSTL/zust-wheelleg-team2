################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Gyro/Gyroscope.c \
../code/Gyro/Gyroscope_Process.c 

COMPILED_SRCS += \
./code/Gyro/Gyroscope.src \
./code/Gyro/Gyroscope_Process.src 

C_DEPS += \
./code/Gyro/Gyroscope.d \
./code/Gyro/Gyroscope_Process.d 

OBJS += \
./code/Gyro/Gyroscope.o \
./code/Gyro/Gyroscope_Process.o 


# Each subdirectory must supply rules for building sources it contributes
code/Gyro/%.src: ../code/Gyro/%.c code/Gyro/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/Gyro\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo '已结束构建： $<'
	@echo ' '

code/Gyro/%.o: ./code/Gyro/%.src code/Gyro/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-code-2f-Gyro

clean-code-2f-Gyro:
	-$(RM) ./code/Gyro/Gyroscope.d ./code/Gyro/Gyroscope.o ./code/Gyro/Gyroscope.src ./code/Gyro/Gyroscope_Process.d ./code/Gyro/Gyroscope_Process.o ./code/Gyro/Gyroscope_Process.src

.PHONY: clean-code-2f-Gyro

