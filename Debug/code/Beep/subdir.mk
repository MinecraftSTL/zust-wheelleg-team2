################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/beep/Beep.c" 

COMPILED_SRCS += \
"code/beep/Beep.src" 

C_DEPS += \
"./code/beep/Beep.d" 

OBJS += \
"code/beep/Beep.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/beep/Beep.src":"../code/beep/Beep.c" "code/beep/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/beep/Beep.o":"code/beep/Beep.src" "code/beep/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-beep

clean-code-2f-beep:
	-$(RM) ./code/beep/Beep.d ./code/beep/Beep.o ./code/beep/Beep.src

.PHONY: clean-code-2f-beep

