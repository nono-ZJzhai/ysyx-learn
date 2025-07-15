```cpp
#include<nvboard.h> 
#include <Vtop.h> // 包含Verilator生成的顶层模块类定义
static TOP_NAME dut; // 静态实例化顶层模块(TOP_NAME是verilator实例化出来的类名,这里调用来实例化)
void nvboard_bind_all_pins(TOP_NAME* top); //绑定引脚

static void single_cycle() {
    dut.clk = 0; dut.eval(); // 时钟下降沿
    dut.clk = 1; dut.eval(); // 时钟上升沿，评估电路状态
}

static void reset(int n){
	dut.rst = 1;
	while(n -- > 0)single_cycle();
	dut.rst = 0;
}

int main() {
	nvboard_bind_all_pins(&dut);
    nvboard_init();
    
	reset(10);
 
	 while(1) {
	    nvboard_update();
	    single_cycle();
	}
	 
}
```

