USERMODE=0x10
FIQMODE=0x11
IRQMODE=0x12
SVCMODE=0x13
ABORTMODE=0x17
UNDEFMODE=0x1b
MODEMASK=0x1f
NOINT=0xc0

// 栈的分配
_STACK_BASEADDRESS=0x3fff8000					
UserStack=(_STACK_BASEADDRESS - 0x5000)	
SVCStack=(_STACK_BASEADDRESS - 0x4000)	
UndefStack=(_STACK_BASEADDRESS - 0x3000)	
AbortStack=(_STACK_BASEADDRESS - 0x2000)	
IRQStack=(_STACK_BASEADDRESS - 0x1000)	
FIQStack=(_STACK_BASEADDRESS - 0x0)	
.global _start


_start:
	
	//初始化堆栈
	bl InitStacks
	// 开中断
	mov r0, #0x53			
	msr CPSR_cxsf, r0	

	// 汇编初始化时钟		
	bl clock_init	

	// 调用main函数
	bl main
	
InitStacks:
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1									// UndefMode
	ldr	sp,=UndefStack		 

	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1									// AbortMode
	ldr	sp,=AbortStack		 

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1									// IRQMode
	ldr	sp,=IRQStack		 

	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1									// FIQMode
	ldr	sp,=FIQStack		 

	bic	r0,r0,#MODEMASK|NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1									// SVCMode
	ldr	sp,=SVCStack		 

	mov	pc,lr	

