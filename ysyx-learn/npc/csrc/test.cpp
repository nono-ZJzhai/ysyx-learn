#include<stdio.h>
#include <stdlib.h>
#include<assert.h>
#include"Vtop.h"
#include"verilated.h"

#include"verilated_vcd_c.h"
int main(int argc,char** argv)
{
	  VerilatedContext* contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);                                                                  
  Vtop* top = new Vtop{contextp}; 
	
	VerilatedVcdC *tfp = new VerilatedVcdC;
  contextp->traceEverOn(true);  //打开追踪
	top->trace(tfp,0);tfp->open("wave.vcd"); //保存位置

	while (!contextp->gotFinish()){
		int a = rand()&1;
		int b =rand()&1;
		top->a = a;
		top->b =b;
		top->eval();
		printf("a=%d,b=%d,f=%d\n",a,b,top->f);

		tfp->dump(contextp->time());
		contextp->timeInc(1);  //仿真时间推进

		assert(top->f==(a^b));
}
delete top;
tfp->close();
delete contextp;
return 0;
}

