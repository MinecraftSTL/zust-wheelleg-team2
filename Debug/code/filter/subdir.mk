################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/filter/Filter.c \
../code/filter/HPF.c \
../code/filter/LPF.c 

COMPILED_SRCS += \
./code/filter/Filter.src \
./code/filter/HPF.src \
./code/filter/LPF.src 

C_DEPS += \
./code/filter/Filter.d \
./code/filter/HPF.d \
./code/filter/LPF.d 

OBJS += \
./code/filter/Filter.o \
./code/filter/HPF.o \
./code/filter/LPF.o 


# Each subdirectory must supply rules for building sources it contributes
code/filter/%.src: ../code/filter/%.c code/filter/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/filter\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo '已结束构建： $<'
	@echo ' '

code/filter/%.o: ./code/filter/%.src code/filter/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-code-2f-filter

clean-code-2f-filter:
	-$(RM) ./code/filter/Filter.d ./code/filter/Filter.o ./code/filter/Filter.src ./code/filter/HPF.d ./code/filter/HPF.o ./code/filter/HPF.src ./code/filter/LPF.d ./code/filter/LPF.o ./code/filter/LPF.src

.PHONY: clean-code-2f-filter

