################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/fps/Fps.c" 

COMPILED_SRCS += \
"code/fps/Fps.src" 

C_DEPS += \
"./code/fps/Fps.d" 

OBJS += \
"code/fps/Fps.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/fps/Fps.src":"../code/fps/Fps.c" "code/fps/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/fps/Fps.o":"code/fps/Fps.src" "code/fps/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-fps

clean-code-2f-fps:
	-$(RM) ./code/fps/Fps.d ./code/fps/Fps.o ./code/fps/Fps.src

.PHONY: clean-code-2f-fps

