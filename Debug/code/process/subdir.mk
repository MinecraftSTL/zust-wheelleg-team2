################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/process/Process.c" 

COMPILED_SRCS += \
"code/process/Process.src" 

C_DEPS += \
"./code/process/Process.d" 

OBJS += \
"code/process/Process.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/process/Process.src":"../code/process/Process.c" "code/process/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/process/Process.o":"code/process/Process.src" "code/process/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-process

clean-code-2f-process:
	-$(RM) ./code/process/Process.d ./code/process/Process.o ./code/process/Process.src

.PHONY: clean-code-2f-process

