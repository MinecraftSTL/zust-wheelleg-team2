################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/wifiImage/WifiImage.c" 

COMPILED_SRCS += \
"code/wifiImage/WifiImage.src" 

C_DEPS += \
"./code/wifiImage/WifiImage.d" 

OBJS += \
"code/wifiImage/WifiImage.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/wifiImage/WifiImage.src":"../code/wifiImage/WifiImage.c" "code/wifiImage/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/wifiImage/WifiImage.o":"code/wifiImage/WifiImage.src" "code/wifiImage/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-wifiImage

clean-code-2f-wifiImage:
	-$(RM) ./code/wifiImage/WifiImage.d ./code/wifiImage/WifiImage.o ./code/wifiImage/WifiImage.src

.PHONY: clean-code-2f-wifiImage

