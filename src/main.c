#include "define.h"

int main (void)
{
	rcc_init();
	gpio_init ();
	systic_init();

	while (1)
	{
		delay_ms(100);
		GPIOB->ODR ^= GPIO_Pin_2;
	}

}
ErrorStatus HSEStartUpStatus;

void rcc_init (void)
{
	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
		 /* Enable Prefetch Buffer */
		 FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		 /* Flash 2 wait state */
		 FLASH_SetLatency(FLASH_Latency_2);
		 /* HCLK = SYSCLK */
		 RCC_HCLKConfig(RCC_SYSCLK_Div1);
		 /* PCLK2 = HCLK */
		 RCC_PCLK2Config(RCC_HCLK_Div1);
		 /* PCLK1 = HCLK/2 */
		 RCC_PCLK1Config(RCC_HCLK_Div2);

	 #ifdef STM32F10X_CL
	     /* Configure PLLs *********************************************************/
	     /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
	     RCC_PREDIV2Config(RCC_PREDIV2_Div5);
	     RCC_PLL2Config(RCC_PLL2Mul_8);

	     /* Enable PLL2 */
	     RCC_PLL2Cmd(ENABLE);

	     /* Wait till PLL2 is ready */
	     while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
	     {}

	     /* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
	     RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
	     RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
	 #else
	     /* PLLCLK = 8MHz * 9 = 72 MHz */
	     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	 #endif

	     /* Enable PLL */
	     RCC_PLLCmd(ENABLE);
	     /* Wait till PLL is ready */
	     while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
	     {
	     }

	     /* Select PLL as system clock source */
	     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	     /* Wait till PLL is used as system clock source */
	     while(RCC_GetSYSCLKSource() != 0x08)
	     {
	     }
		}
		else
		{ /* If HSE fails to start-up, the application will have wrong clock configuration.
			User can add here some code to deal with this error */

			/* Go to infinite loop */
			while (1)
			{
			}
		}
	 }

/* STM32F10X_LD_VL && STM32F10X_MD_VL */

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

	/* Configure PA13 and PA14 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void HardFault_Handler() {
	while(1);
}
