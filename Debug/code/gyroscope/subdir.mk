################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
../code/gyroscope/Gyroscope.c \
../code/gyroscope/Gyroscope_Process.c 

COMPILED_SRCS += \
./code/gyroscope/Gyroscope.src \
./code/gyroscope/Gyroscope_Process.src 

C_DEPS += \
./code/gyroscope/Gyroscope.d \
./code/gyroscope/Gyroscope_Process.d 

OBJS += \
./code/gyroscope/Gyroscope.o \
./code/gyroscope/Gyroscope_Process.o 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
code/gyroscope/%.src: ../code/gyroscope/%.c code/gyroscope/subdir.mk
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/gyroscope\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo '�ѽ��������� $<'
	@echo ' '

code/gyroscope/%.o: ./code/gyroscope/%.src code/gyroscope/subdir.mk
	@echo '���ڹ����ļ��� $<'
	@echo '���ڵ��ã� TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo '�ѽ��������� $<'
	@echo ' '


clean: clean-code-2f-gyroscope

clean-code-2f-gyroscope:
	-$(RM) ./code/gyroscope/Gyroscope.d ./code/gyroscope/Gyroscope.o ./code/gyroscope/Gyroscope.src ./code/gyroscope/Gyroscope_Process.d ./code/gyroscope/Gyroscope_Process.o ./code/gyroscope/Gyroscope_Process.src

.PHONY: clean-code-2f-gyroscope

