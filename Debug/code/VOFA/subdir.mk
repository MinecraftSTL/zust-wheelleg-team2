################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/VOFA/VOFA.c" 

COMPILED_SRCS += \
"code/VOFA/VOFA.src" 

C_DEPS += \
"./code/VOFA/VOFA.d" 

OBJS += \
"code/VOFA/VOFA.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/VOFA/VOFA.src":"../code/VOFA/VOFA.c" "code/VOFA/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/VOFA/VOFA.o":"code/VOFA/VOFA.src" "code/VOFA/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-VOFA

clean-code-2f-VOFA:
	-$(RM) ./code/VOFA/VOFA.d ./code/VOFA/VOFA.o ./code/VOFA/VOFA.src

.PHONY: clean-code-2f-VOFA

