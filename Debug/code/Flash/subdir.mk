################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/Flash/flash.c" 

COMPILED_SRCS += \
"code/Flash/flash.src" 

C_DEPS += \
"./code/Flash/flash.d" 

OBJS += \
"code/Flash/flash.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/Flash/flash.src":"../code/Flash/flash.c" "code/Flash/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/Flash/flash.o":"code/Flash/flash.src" "code/Flash/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Flash

clean-code-2f-Flash:
	-$(RM) ./code/Flash/flash.d ./code/Flash/flash.o ./code/Flash/flash.src

.PHONY: clean-code-2f-Flash

