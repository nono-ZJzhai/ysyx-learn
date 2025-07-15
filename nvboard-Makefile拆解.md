```Makefile
TOPNAME = top
NXDC_FILES =constr/top.nxdc
INC_PATH ?=    #头文件,include
              #?= 表示若此前没有赋值则,此处赋值
VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc
	-O3 --x-assign fast --x-initial fast --noassert 
BUILD_DIR = ./build
OBJ_DIR =$(BUILD_DIR)/obj_dir
BIN =$(BUILD_DIR)/$(TOPNAME)

default: $(BIN)   #默认先构建BIN目录下的文件而，BIN目录下目录的构建则需要依靠另一条BIN的构建指令

$(shell mkdir -p $(BUILD_DIR))  #递归创建目录

# constraint file   约束文件
SRC_AUTO_BIND = $(abspath $(BUILD_DIR)/auto_bin_bind.cpp)  #abspath用一个$
$(SRC_AUTO_BIND): $(NXDC_FILES)  #表示生成SRC_AUTO_BIND文件
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@
#冒号后面跟依赖文件  
#project source
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp") 
CSRCS += $(SRC_AUTO_BIND) #把自动生成的代码加入

# rules for NVboard
include $(NV_BOARD_HOME)/scripts/nvboard.mk

# rulses for verilator 
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""

$(BIN): $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)	
	$(VERILATOR) $(VERILATOR_CFLAGS) \
	--top-module $(TOPNAME) $^ \
	$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
	--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

all: default

run: $(BIN)
	@S^
	
	
```

`#rules for verilator`[^1]
`$(BIN): $(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)`[^2]
`命令行末尾\的作用`[^3]

[^1]: `$(addprefix -I, ...)`：Makefile 的内置函数，给 `$(INC_PATH)` 里的 **每个目录前面加上 `-I` 前缀**
- 示例：如果 `INC_PATH = ./include /usr/local/include` ，执行后 `INCFLAGS` 会变成 `-I./include -I/usr/local/include` 。
- 作用：`-I` 是给 C/C++ 编译器（如 `g++` ）用的，告诉编译器 “去这些目录里找头文件（`#include` 时用的）

- `-D` 是编译器选项，用来 **定义宏** 。这里会定义一个叫 `TOP_NAME` 的宏，值是 `"V$(TOPNAME)"` （`$(TOPNAME)` 是另一个变量，假设是项目里的模块名、顶层模块名 ）。
总结:
1. 去 `INC_PATH` 里的目录找头文件（因为有 `-I` 选项 ）。
2. 定义 `TOP_NAME` 宏，值是 `V` 拼接 `TOPNAME` 变量，代码里能通过 `#define` 相关语法使用这个宏。

[^2]: #### （1）`@rm -rf $(OBJ_DIR)`
	
	- `@`：Makefile 里加 `@` 表示 **执行命令时不打印命令本身**（安静模式 ）。
	- `rm -rf $(OBJ_DIR)`：强制删除 `$(OBJ_DIR)` 目录（`-r` 递归删目录，`-f` 强制不提示 ），一般用来清理旧的编译中间文件，保证重新构建干净。
	#### （2）`$(VERILATOR) $(VERILATOR_CFLAGS) \`
	
	- `$(VERILATOR)`：指向 `Verilator` 工具的可执行程序路径（比如 `/usr/bin/verilator` ），用来把 Verilog 代码转成 C++ 模型，或直接编译仿真。
	- `$(VERILATOR_CFLAGS)`：`Verilator` 的编译选项（比如优化级别、调试开关、是否保留调试信息 ），是 Makefile 里定义的变量。
	
	#### （3）`--top-module $(TOPNAME) $^ \`
	
	- `--top-module $(TOPNAME)`：告诉 `Verilator` ，以 `$(TOPNAME)` 为 **顶层模块** 处理 Verilog 代码（Verilog 是硬件描述语言，需要指定顶层入口 ）。
	- `$^`：Makefile 自动变量，代表 **所有依赖文件列表**（即 `$(VSRCS) $(CSRCS) $(NVBOARD_ARCHIVE)` ），会把这些文件传给 `Verilator` 处理。
	
	#### （4）`$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \`
	
	- `$(addprefix -CFLAGS , $(CXXFLAGS))`：给 `$(CXXFLAGS)`（C++ 编译选项，比如头文件路径、宏定义 ）前面加 `-CFLAGS` 前缀，传给 `Verilator` ，让它把这些选项传递给 C++ 编译器。
	- `$(addprefix -LDFLAGS , $(LDFLAGS))`：类似上面，给链接选项（`$(LDFLAGS)` ，比如库文件路径、要链接的库 ）前面加 `-LDFLAGS` 前缀，用于最终链接可执行程序。
	
	#### （5）`--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))`
	
	- `--Mdir $(OBJ_DIR)`：指定 `Verilator` 生成的 **中间文件（比如 C++ 模型、编译临时文件 ）存放目录** ，`$(OBJ_DIR)` 是中间目录变量。
	- `--exe`：告诉 `Verilator` 要生成 **可执行程序**（而非单纯的 C++ 模型库 ）。
	- `-o $(abspath $(BIN))`：指定最终输出的可执行文件路径，`$(abspath $(BIN))` 会把 `$(BIN)` 转成绝对路径，保证输出位置明确。
	  
	

[^3]:   在 Makefile 里，命令行末尾的 `\` 是 **续行符** ，作用是 **把一行过长的命令拆分成多行写，让代码更清晰**，但逻辑上仍算 “同一行命令
