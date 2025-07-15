module top(
    input clk,
		input [15:0]sw,
		output [15:0] ledr
);
		led myled(
			.clk(clk),
			.sw(sw),
			.ledr(ledr)
);
endmodule
