################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/myEncoder/MyEncoder.c" 

COMPILED_SRCS += \
"code/myEncoder/MyEncoder.src" 

C_DEPS += \
"./code/myEncoder/MyEncoder.d" 

OBJS += \
"code/myEncoder/MyEncoder.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/myEncoder/MyEncoder.src":"../code/myEncoder/MyEncoder.c" "code/myEncoder/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/myEncoder/MyEncoder.o":"code/myEncoder/MyEncoder.src" "code/myEncoder/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-myEncoder

clean-code-2f-myEncoder:
	-$(RM) ./code/myEncoder/MyEncoder.d ./code/myEncoder/MyEncoder.o ./code/myEncoder/MyEncoder.src

.PHONY: clean-code-2f-myEncoder

