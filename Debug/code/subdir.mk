################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/camera.c \
../code/encoder.c \
../code/key.c \
../code/motor.c \
../code/sys.c 

COMPILED_SRCS += \
./code/camera.src \
./code/encoder.src \
./code/key.src \
./code/motor.src \
./code/sys.src 

C_DEPS += \
./code/camera.d \
./code/encoder.d \
./code/key.d \
./code/motor.d \
./code/sys.d 

OBJS += \
./code/camera.o \
./code/encoder.o \
./code/key.o \
./code/motor.o \
./code/sys.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fE:/User/S/smartc/E09_14_mt9v03x_ips200_display_demo/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo '已结束构建： $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/camera.d ./code/camera.o ./code/camera.src ./code/encoder.d ./code/encoder.o ./code/encoder.src ./code/key.d ./code/key.o ./code/key.src ./code/motor.d ./code/motor.o ./code/motor.src ./code/sys.d ./code/sys.o ./code/sys.src

.PHONY: clean-code

