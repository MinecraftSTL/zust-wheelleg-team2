################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.c" \
"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.c" \
"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.c" \
"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.c" 

COMPILED_SRCS += \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src" \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src" \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src" \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src" 

C_DEPS += \
"./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.d" \
"./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.d" \
"./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.d" \
"./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.d" 

OBJS += \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.o" \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.o" \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.o" \
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.o" 


# 每个子目录必须为构建它所贡献的源提供规则
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src":"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.c" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.o":"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src":"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.c" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.o":"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src":"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.c" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.o":"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src":"../libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.c" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 "-fE:/User/S/smartc/TC387_Library-master/Seekfree_TC387_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc38x -Y0 -N0 -Z0 -o "$@" "$<"
"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.o":"libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src" "libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-libraries-2f-infineon_libraries-2f-Infra-2f-Platform-2f-Tricore-2f-Compilers

clean-libraries-2f-infineon_libraries-2f-Infra-2f-Platform-2f-Tricore-2f-Compilers:
	-$(RM) ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.d ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.o ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerDcc.src ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.d ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.o ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGhs.src ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.d ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.o ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerGnuc.src ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.d ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.o ./libraries/infineon_libraries/Infra/Platform/Tricore/Compilers/CompilerTasking.src

.PHONY: clean-libraries-2f-infineon_libraries-2f-Infra-2f-Platform-2f-Tricore-2f-Compilers

