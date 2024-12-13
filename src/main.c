#include "define.h"

void (*task_ptr)(void);
void (*task_1ms_ptr)(void);
void (*task_5ms_ptr)(void);
void task_1ms(void);
void task_5ms(void);
void task_1ms_1(void);
void task_1ms_2(void);
void task_5ms_1(void);
void task_5ms_2(void);

uint16_t temper = 0;
uint8_t temper_start = 0;

int main (void)
{
	uint8_t serial_number[8];

	rcc_init();
	gpio_init ();
	systic_init();

	task_ptr = &task_1ms;
	task_1ms_ptr = &task_1ms_1;
	task_5ms_ptr = &task_5ms_1;

	//while (ds18b20_init(MODE_SKIP_ROM));
	while (ds18b20_ReadRom_(serial_number));
	send_CAN(serial_number,8);

	delay_ms(100);

	ds18b20_init(MODE_SKIP_ROM);

	while (1)
	{
		(*task_ptr)();

		if (temper_start || !temper) {
			temper_start = 0;
			temper = ds18b20_Tread();
		}
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

	/* Configure PA13 and PA14 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = TM1637_CLK_PIN | TM1637_DIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(TM1637_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TM1637_PORT,TM1637_CLK_PIN | TM1637_DIO_PIN);

	GPIO_InitStructure.GPIO_Pin = DS18b20_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(DS18b20_PORT, &GPIO_InitStructure);
	GPIO_SetBits(DS18b20_PORT, DS18b20_pin);
}


void HardFault_Handler() {
	while(1);
}
