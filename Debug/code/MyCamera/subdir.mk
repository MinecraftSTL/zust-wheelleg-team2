################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../code/myCamera/Image.c" \
"../code/myCamera/MyCamera.c" 

COMPILED_SRCS += \
"code/myCamera/Image.src" \
"code/myCamera/MyCamera.src" 

C_DEPS += \
"./code/myCamera/Image.d" \
"./code/myCamera/MyCamera.d" 

OBJS += \
"code/myCamera/Image.o" \
"code/myCamera/MyCamera.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"code/myCamera/Image.src":"../code/myCamera/Image.c" "code/myCamera/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/myCamera/Image.o":"code/myCamera/Image.src" "code/myCamera/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/myCamera/MyCamera.src":"../code/myCamera/MyCamera.c" "code/myCamera/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=0 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"code/myCamera/MyCamera.o":"code/myCamera/MyCamera.src" "code/myCamera/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code-2f-myCamera

clean-code-2f-myCamera:
	-$(RM) ./code/myCamera/Image.d ./code/myCamera/Image.o ./code/myCamera/Image.src ./code/myCamera/MyCamera.d ./code/myCamera/MyCamera.o ./code/myCamera/MyCamera.src

.PHONY: clean-code-2f-myCamera

