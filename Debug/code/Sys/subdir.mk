################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/sys/Sys.c" 

COMPILED_SRCS += \
"code/sys/Sys.src" 

C_DEPS += \
"./code/sys/Sys.d" 

OBJS += \
"code/sys/Sys.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/sys/Sys.src":"../code/sys/Sys.c" "code/sys/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/sys/Sys.o":"code/sys/Sys.src" "code/sys/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-sys

clean-code-2f-sys:
	-$(RM) ./code/sys/Sys.d ./code/sys/Sys.o ./code/sys/Sys.src

.PHONY: clean-code-2f-sys

