1. 模块以及基本使用
```verilog
module  top_module( input in ,output out);
assign in = 1 ; -- 使用assign来指定，这是必须的
assign out = 0;


------------------- wire,Verilog中描述电线是通过 =
assign out = in ;  -- 这里的= 是连续的赋值,就像电线一样
endmodule
```
---
2. input和output都可以视为wire. input wire a 等价 input a
---
按位取反: ~
逻辑取反: !
# 2. **与门**
按位与: &   
逻辑与: &&  //就是对整个与
# 3.或门
按位:|
逻辑:||
# 4. 异或门
只有按位异或:^
没有逻辑异或
# 5. 声明wires
wire:是导线也可以是一个节点,也类似于一个门,但是线上的电压都一样,<span style="color:rgb(146, 208, 80)">同电压的同一线都可以用</span>
![[Pasted image 20250423143410.png]]
wire  name;
# 6. vector(向量,矩阵)

使用向量将同类信号合组
声明时: 将维数(亦位宽)置于向量之前,且 形式为\[99:0]
```verilog


wire [99:0] my_vector;	// Verilog中的100位wire型向量的声明
int your_array[100];   //C语言中的100个元素int型数组的声明

assign out = my_vector[10];	--Verilog中对向量的第11位信号的选用
a = your_array[10];			--C语言中对数组的第11个元素的访问调用
```

## 2. 横向处理vector
```verilog
assign out = ^in[7:0];  -- 这里是对in的所有位都做了连续的异或处理
 -- 也可以不加后面的[],表示一样的问题.
```

# 7. vector 

<span style="color:rgb(255, 0, 0)"> 声明:</span>
type \[upper:lower] vector_name;
- <span style="color:rgb(146, 208, 80)">详解:</span>
	1. type 可为 wire or reg(寄存器)
	2. upper和lower是默认的==高位到低位==,无关填入的数字如\[0:7],也是如此
	3. 数字也可以为负数如:\[3:-2]
	4. 随后使用时,不能颠倒向量顺序\[0:3] 不能在使用时写为\[3:0]
	5. \[0:0]被认为是1-bit 
	6. \[7:0] 8-bit; \[4:1] 4-bit;
- <span style="color:rgb(146, 208, 80)">隐式声明:</span>
	1. 使用assign来创建一个新变量, 该变量默认为1bit位宽
	2. 所有没有提前声明的变量都是隐式变量[^1]
	3. **可以用 'default_nettype none** 来禁止使用隐式变量
- <span style="color:rgb(146, 208, 80)">向量组和存储器</span>
	1. 名前,是向量的维数(位宽)
	2. 名后,是向量的个数
如下:

| 向量组  | 存储器    |          |
| ---- | ------ | -------- |
| 向量   | 存储单元   |          |
| 向量维数 | 存储单元位宽 | packed   |
| 向量个数 | 存储单元个数 | unpacked |
- 向量的部分访问
	1. 使用全名,访问全部(会补0和截断)
	2. \[3:0]访问最低的四位

# 13.**Bitwise operators**
**<span style="color:rgb(146, 208, 80)">区分按位和逻辑</span>
- 按位:是每一位都进行操作,
- 逻辑:将向量作为整体,<span style="color:rgb(146, 208, 80)">非零为1,零才为0</span>

# 14. vector拼接
3. <span style="color:rgb(0, 176, 240)">数字表示:</span>
	1. 3'b111 <span style="color:rgb(146, 208, 80)">表示二进制的7</span> -- 前面表示位宽
	2. 4'ha <span style="color:rgb(146, 208, 80)">表示16进制的10</span>
	3. 5'd10 <span style="color:rgb(146, 208, 80)">表示十进制的10</span>
4. <span style="color:rgb(0, 176, 240)">拼接:</span>
	1. {a,b} = c; 也可以调换方向 c= {a,b};
	2. 快速拼接{num{vector}},<span style="color:rgb(255, 0, 0)">num必须为常量</span>
	3. 其中可以是多个vector,{a,b,c}  //<span style="color:rgb(255, 0, 0)">是 {a,a,...}这种</span>
	4. 连用2,3时,要使用<span style="color:rgb(146, 208, 80)">双层括号</span> {a,<span style="color:rgb(0, 176, 240)">{2{b}}</span>},<span style="color:rgb(0, 176, 240)">{num{vector}}</span>,其本身才算一个拼接后的vector
	5. [^2]在二进制的补码中多使用
# 15. 多模块联动
5. <span style="color:rgb(0, 176, 240)">在一个模块中使用另一个模块</span>
	1. 使用位置匹配:
		1. mod_a instance1 (wa,wb,wc) //<span style="color:rgb(146, 208, 80)">将mod_a实例化为 instance1 并将 wa,wb,wc作为输入传入</span>
		2. 缺点是:<span style="color:rgb(146, 208, 80)">如果原模块参数的位置发生改变,所有使用的mod,都要再重新修改一遍</span>
	2. 使用名字匹配:
		1.  mod_a instance2 (.out(wc), .in1(wa), .in2(wb));
		2. 注意:<span style="color:rgb(255, 0, 0)">前面加上句点:     形式为 .in(wa) </span> 其间使用逗号隔开.
# 16 .Always语法
## 1. always使用模板
```Verilog
always @(*) begin
	 中间填上赋值语句,即reg,assign等
end
```
## 2. always的注意事项
6. () ,内填的是语句触发条件,即()中有一项东西发生变动,就会再次执行一次always语句,(与assign区分:assign,就是导线,=的变动自然会传递)[^3]
7. <span style="color:rgb(255, 0, 0)">上古语法</span>:使用assign时,左边必须为 net type (e,g, wire)
8. 而使用 always 时, 左边必须为  variable type (e,g, reg)
9. <span style="color:rgb(24, 205, 160)">always@(posedge clk) </span> <span style="color:rgb(24, 205, 160)">时钟驱动的always块</span>
	1. posedge-即上升沿,表示<span style="color:rgb(146, 208, 80)">clk上升沿触发</span>
	2. 在时钟驱动的always块中,后面必须使用使用<span style="color:rgb(146, 208, 80)">非阻塞赋值</span> 即 <=





## 3. always 的语法特性

10. <span style="color:rgb(24, 205, 160)">always if 语法</span>  
```Verilog
always @(*) begin
	if (condition) 
		out =x;
	else 
		out =y;
end
-----------------------
上述也等价于 assign out = (conditon) ? x:y;  
----------------------
可能出错的地方:
out 如果没有一直被赋值,则,其不为一个组合电路,所以if else都需要使用
```
11. <span style="color:rgb(0, 176, 240)">always case语法</span>: ----------<span style="color:rgb(255, 0, 0)">尽管貌似是分支语句,其实这里的case更多是一个并行语句,搭建起来的.</span>
	1.使用:
```verilog
	always @(*)begin
	case(in)          -- 1. in 就是拿来判断的数
	1'b1:begin        -- 2. 表示的意思是:位宽·'进制·值
		out =1'b1;
		end           -- 3. 对于每个触发的case语句仍然要使用begin和end
	1'b0: out = 1'b0;
	default:out =1'bx;
	endcase           -- 4. endcase来终结case语句
end
```

- 语法讲解:
	1. always后面的依然是触发电路更新的逻辑组合电路
	2. case的使用时在case(in)和endcase之间,
	3. in----即填写的相关表达式
	4. <span style="color:rgb(166, 83, 38)">只执行每个值对应的语句</span>
	5. 在值后面填上<span style="color:rgb(166, 83, 38)">:</span>
	6. 对于每个case item ,如果表达式只有一行则可省去<span style="color:rgb(166, 83, 38)">begin 和 end(否则添上)</span>
	7. <span style="color:rgb(166, 83, 38)">多个同值case不报错</span>
	8. <span style="color:rgb(255, 0, 0)">绝对不要忘记default</span>
12. <span style="color:rgb(0, 176, 240)">casez </span>:
```Verilog
always @(*) begin
	casez(in[3:0])
	4'bzzz1: out =0; -- 1. 其中,z占位的就会被忽略
	4'bxx1x: out =1; -- 2.  
	4'b?1??: out =2; -- 3. ?和z的功能一样都作为忽略占位符
	default: out =0; -- 4. always中的case语句不是简单并行而是第一个符合的执行
	endcase
end
```
- <span style="color:rgb(255, 0, 0)">踩的坑</span>:
	- 如果要用x来作为忽略占位符,就需要用casex来代替casez
## 4.避免latches (锁存器)
	技巧1: 使用前赋默认值



# 18.Verilog中有三种賦值类型
13. 连续赋值: assign x= y;  <span style="color:rgb(255, 0, 0)">always块中禁止使用</span>
14. 过程阻塞赋值; x= y;<span style="color:rgb(255, 0, 0)">仅在always中使用</span>, <span style="color:rgb(146, 208, 80)">且用在组合逻辑的always块中</span>
15. 过程非阻塞赋值 : x<= y; <span style="color:rgb(255, 0, 0)">尽在always中使用</span><span style="color:rgb(146, 208, 80)">且仅在时钟驱动的always块中</span>

# 19.verilog的并行
- 体现在一个输出不能多次赋值，因为多次赋值会不清楚，导线到底是怎么连接的。所以会使用组合逻辑电路，来安排。


[^1]: my_module i1 (d,e); // d and e are implicitly one-bit wide if not declared.

[^2]: One common place to see a replication operator is when sign-extending a smaller number to a larger one, while preserving its signed value. This is done by replicating the sign bit (the most significant bit) of the smaller number to the left. For example, sign-extending 4'b**0**101 (5) to 8 bits results in 8'b**0000**0101 (5), while sign-extending 4'b**1**101 (-3) to 8 bits results in 8'b**1111**1101 (-3).

[^3]: 注意是并行触发,不管语句位置
