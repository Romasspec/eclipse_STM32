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
uint8_t ds18b20found = 0, ds18b20read = 0;
uint8_t serial_number[8];

int main (void)
{
	uint32_t timeout;
	state_temper_sensor state;
	rcc_init();
	gpio_init ();
	systic_init();

	task_ptr = &task_1ms;
	task_1ms_ptr = &task_1ms_1;
	task_5ms_ptr = &task_5ms_1;

//	timeout = millis() + TIMEOUT_INIT;
//	while ((int32_t)(millis()-timeout) < 0)							// инициализация датчика
//	{
//		if (!ds18b20_init(MODE_SKIP_ROM))
//		{
//			while (1)
//			{
//				if(!ds18b20_ReadRom_(serial_number)) {
//					ds18b20found = 1;
//					break;
//				}
//			}
//		}
////		if(ds18b20found) {
////			break;
////		}
//	}

	delay_ms(100);

	while (1)
	{
		(*task_ptr)();

		if (!ds18b20found && temper_start) {										// инициализация датчика при его подключении в процессе работы
			if (!ds18b20_init(MODE_SKIP_ROM)) {
				while (1)
				{
					if(!ds18b20_ReadRom_(serial_number)) {
						ds18b20found = 1;
						break;
					}
				}
			}
			temper_start = 0;
		}

		if(temper_start && ds18b20found) {
			state = ds18b20_Tread();
			if(state == MEASURE_COMPLETE) {
				temper = ds18b20_GetTemp();
				temper_start = 0;
				ds18b20read = 1;
			} else if (state == ERROR_CRC) {
				ds18b20found = 0;
				temper_start = 0;
				temper = 0;
				ds18b20read = 0;
			} else if ((int32_t)(millis()-timeout) >= 0) {			// определение отсутствия датчика
				ds18b20found = 0;
				temper_start = 0;
				temper = 0;
				ds18b20read = 0;
			//	LED_ON();
			}
		} else if (ds18b20found) {
			timeout = millis() + 2000;								// таймаут для определения отсутствия датчика
		}
	}
}


//=================================================================================
//  STATE-MACHINE SEQUENCING AND SYNCRONIZATION
//=================================================================================

//--------------------------------- FRAMEWORK -------------------------------------
void task_1ms(void)
{
	static uint32_t time_task = 0;
	uint32_t current_time = micros();
	if ((int32_t)(current_time-time_task) >= 0)
	{
		time_task = current_time + 1000;
		(*task_1ms_ptr)();
	}

	task_ptr = &task_5ms;
}

void task_5ms(void)
{
	static uint32_t time_task = 0;
	uint32_t current_time = micros();
	if ((int32_t)(current_time-time_task) >= 0)
	{
		time_task = current_time +  5000;
		(*task_5ms_ptr)();
	}
	task_ptr = &task_1ms;
}

//=================================================================================
//  A - TASKS - 1мс
//=================================================================================
void task_1ms_1(void)
{
	task_1ms_ptr = &task_1ms_2;
}
//extern uint8_t dt[8];
void task_1ms_2(void)
{
	task_1ms_ptr = &task_1ms_1;
}

//=================================================================================
//  B - TASKS - 5 мс
//=================================================================================
void task_5ms_1(void)
{
	static uint32_t time_task = 0, time_led_on;
	uint32_t current_time = micros();
	if ((int32_t)(current_time-time_task) >= 0)
	{
		time_task = current_time + 1000000;
		time_led_on = current_time + 10000;
		temper_start = 1;
		LED_ON();
	}

	if ((int32_t)(current_time-time_led_on) >= 0)
	{
		LED_OFF();
	}

	task_5ms_ptr = &task_5ms_2;
}

void task_5ms_2(void)
{
	uint8_t buf[4];
	static uint32_t time_task = 0;
	uint32_t current_time = micros();
	if ((int32_t)(current_time-time_task) >= 0)
	{
		time_task = current_time + 500000;

		buf[0] = (uint8_t) ((temper>>8) & 0x7F);
		if (buf[0] > 99) {
			buf[1] = digToHEX((buf[0] % 1000) / 100);
			buf[2] = digToHEX((buf[0] % 100) / 10);
			buf[3] = digToHEX((buf[0] % 10) / 1);
		} else if (buf[0] > 9) {
			buf[1] = digToHEX((buf[0] % 100) / 10);
			buf[2] = digToHEX((buf[0] % 10) / 1) | 0x80;
			buf[3] = digToHEX(((uint8_t)(temper & 0x00FF) % 100) / 10);
		} else {
			buf[1] = digToHEX((buf[0] % 1) % 10) | 0x80;
			buf[2] = digToHEX(((uint8_t)(temper & 0x00FF) % 100) / 10);
			buf[3] = digToHEX(((uint8_t)(temper & 0x00FF) % 10) / 1);
		}

		if (temper & 0x8000) {
			buf[0] = digToHEX (11);
		} else {
			buf[0] = digToHEX(10);
		}

		if(!ds18b20found) {
			buf[0] = buf[1] = buf[2] = buf[3] = digToHEX (11);
		} else if (!ds18b20read) {
			buf[0] = buf[1] = buf[3] =  digToHEX (10);
			buf[2] = 0x80;
		}

		tm1637_send_buf(buf, 4);
		tm1637_set_brightness(BRIGHTNESS1);
	}

	task_5ms_ptr = &task_5ms_1;
}
//------------------------------END FRAMEWORK -------------------------------------

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

	/* Configure PA13 and PA14 in output open drive mode */
	GPIO_InitStructure.GPIO_Pin = TM1637_CLK_PIN | TM1637_DIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(TM1637_PORT, &GPIO_InitStructure);
	GPIO_SetBits(TM1637_PORT,TM1637_CLK_PIN | TM1637_DIO_PIN);

	GPIO_InitStructure.GPIO_Pin = LED_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
//	GPIO_SetBits(LED_PORT, LED_pin);

	GPIO_InitStructure.GPIO_Pin = DS18b20_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(DS18b20_PORT, &GPIO_InitStructure);
	GPIO_SetBits(DS18b20_PORT, DS18b20_pin);

	GPIO_InitStructure.GPIO_Pin = DS18b20_GND_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(DS18b20_GND_PORT, &GPIO_InitStructure);
}


void HardFault_Handler() {
	while(1);
}
