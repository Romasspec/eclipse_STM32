#include "stm32f10x.h"

void RCC_init (void);

int main (void)
{
	RCC_init();
	while (1)
	{

	}
	return 0;
}

void RCC_init (void)
{

}


void HardFault_Handler() {
	while(1);
}

void SysTick_Handler () {

}
