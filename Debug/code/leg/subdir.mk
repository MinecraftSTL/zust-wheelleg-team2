################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/leg/Leg.c" 

COMPILED_SRCS += \
"code/leg/Leg.src" 

C_DEPS += \
"./code/leg/Leg.d" 

OBJS += \
"code/leg/Leg.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/leg/Leg.src":"../code/leg/Leg.c" "code/leg/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/leg/Leg.o":"code/leg/Leg.src" "code/leg/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-leg

clean-code-2f-leg:
	-$(RM) ./code/leg/Leg.d ./code/leg/Leg.o ./code/leg/Leg.src

.PHONY: clean-code-2f-leg

