################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/debugArg/DebugArg.c" 

COMPILED_SRCS += \
"code/debugArg/DebugArg.src" 

C_DEPS += \
"./code/debugArg/DebugArg.d" 

OBJS += \
"code/debugArg/DebugArg.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/debugArg/DebugArg.src":"../code/debugArg/DebugArg.c" "code/debugArg/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/debugArg/DebugArg.o":"code/debugArg/DebugArg.src" "code/debugArg/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-debugArg

clean-code-2f-debugArg:
	-$(RM) ./code/debugArg/DebugArg.d ./code/debugArg/DebugArg.o ./code/debugArg/DebugArg.src

.PHONY: clean-code-2f-debugArg

