################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/Status/Status.c" 

COMPILED_SRCS += \
"code/Status/Status.src" 

C_DEPS += \
"./code/Status/Status.d" 

OBJS += \
"code/Status/Status.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/Status/Status.src":"../code/Status/Status.c" "code/Status/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/Status/Status.o":"code/Status/Status.src" "code/Status/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Status

clean-code-2f-Status:
	-$(RM) ./code/Status/Status.d ./code/Status/Status.o ./code/Status/Status.src

.PHONY: clean-code-2f-Status

