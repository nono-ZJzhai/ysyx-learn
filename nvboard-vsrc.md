```verilog
moudule top(
	input clk,
	input rst,
	input [4:0] btn,
	input [7:0] sw,
	input ps2_clk,
	input ps2_data,
	input uart_rx,
	input uart_rt,
	output [15:0] ledr,
    output VGA_CLK,                       
    output VGA_HSYNC,                          output VGA_VSYNC,                          output VGA_BLANK_N,                        output [7:0] VGA_R,                              
    output [7:0] VGA_G,                              
    output [7:0] VGA_B,                              
    output [7:0] seg0,                               
    output [7:0] seg1,                               
    output [7:0] seg2,                              
    output [7:0] seg3,                               
    output [7:0] seg4,                               
    output [7:0] seg5,                             
    output [7:0] seg6,                    
    output [7:0] seg7   
);                 //可以看到都是引脚绑定

led my_led(          //其中led模块是在本文件之外的当前文件夹下
	.clk(clk),       //这里是把led的clk输入端口接到top模块中的clk上
	.rst(rst),
	.btn(btn),
	.sw(sw),
	.ledr(ledr),
)

... //后面都是和这个相似的引脚绑定,都差不多.

```

引脚绑定必不可少,随后是执行逻辑.(在main.cpp中