################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/Wifi_Image/Wifi_Image.c" 

COMPILED_SRCS += \
"code/Wifi_Image/Wifi_Image.src" 

C_DEPS += \
"./code/Wifi_Image/Wifi_Image.d" 

OBJS += \
"code/Wifi_Image/Wifi_Image.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/Wifi_Image/Wifi_Image.src":"../code/Wifi_Image/Wifi_Image.c" "code/Wifi_Image/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/Wifi_Image/Wifi_Image.o":"code/Wifi_Image/Wifi_Image.src" "code/Wifi_Image/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-Wifi_Image

clean-code-2f-Wifi_Image:
	-$(RM) ./code/Wifi_Image/Wifi_Image.d ./code/Wifi_Image/Wifi_Image.o ./code/Wifi_Image/Wifi_Image.src

.PHONY: clean-code-2f-Wifi_Image

