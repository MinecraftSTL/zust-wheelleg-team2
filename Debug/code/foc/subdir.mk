################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# ����Щ���ߵ��õ�����������ӵ��������� 
C_SRCS += \
"../code/foc/small_driver_uart_control.c" 

COMPILED_SRCS += \
"code/foc/small_driver_uart_control.src" 

C_DEPS += \
"./code/foc/small_driver_uart_control.d" 

OBJS += \
"code/foc/small_driver_uart_control.o" 


# ÿ����Ŀ¼����Ϊ�����������׵�Դ�ṩ����
"code/foc/small_driver_uart_control.src":"../code/foc/small_driver_uart_control.c" "code/foc/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/foc/small_driver_uart_control.o":"code/foc/small_driver_uart_control.src" "code/foc/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-foc

clean-code-2f-foc:
	-$(RM) ./code/foc/small_driver_uart_control.d ./code/foc/small_driver_uart_control.o ./code/foc/small_driver_uart_control.src

.PHONY: clean-code-2f-foc

