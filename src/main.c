#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <misc.h>
#include <stm32f10x_exti.h>

void Init_gpio (void);
void Init_timer (void);
void Init_nvic (void);
//void InitEXTI (void);
volatile uint8_t time;

void EXTI9_5_IRQHandler (void)
{
	if ( EXTI_GetITStatus (EXTI_Line5)==1)
	{
		GPIOA->ODR ^= (1<<4);
	}
	
	EXTI_ClearITPendingBit (EXTI_Line5);
}

void TIM4_IRQhahdler (void)
{
//	if (TIM_GetITStatus (TIM4, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit (TIM4, TIM_IT_Update);
		TIM4->SR = 0x00;//0xE1A0;
	
//		time++;
//		GPIO_SetBits (GPIOA, GPIO_Pin_4);
//		GPIOA->ODR ^= GPIO_Pin_4;
	GPIOA->ODR ^= (1<<4);
//	}
}
 int main (void)
 {
	 u16 k,k1;
	 u8 flag;
	 
	 Init_gpio ();
	 Init_timer ();
//	 Init_nvic ();
//	 InitEXTI ();
	 
	 while (1)
	 {
		 //GPIOA->ODR &=~(1<<4);
//		 if (((GPIO_ReadInputDataBit (GPIOA, GPIO_Pin_5))==1) && (flag & (1<<0)))
//		 {
//			 //GPIO_SetBits (GPIOA, GPIO_Pin_4);
//			 GPIO_ResetBits (GPIOA, GPIO_Pin_4);
//			 flag &=~(1<<0);
//		 }
//		 else if (flag & (1<<0))
//		 {
//			 GPIOA->ODR ^= (1<<4);
//			 flag &=~(1<<0);
//		 }
//				k1= TIM4->CNT;
//		 if (k1 > 250)
//		 {
//			 GPIOA->ODR |= (1<<4);
//		 }
//		 else
//		 {
//			 GPIOA->ODR &=~(1<<4);
//		 }
			 
		 if (((GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_13))==1) && (flag & (1<<1)))
		 {
			 GPIO_ResetBits (GPIOB, GPIO_Pin_14);
			 flag &=~(1<<1);
		 }
		 else if (flag & (1<<1))
		 {
			 GPIO_SetBits (GPIOB, GPIO_Pin_14);
			 flag &=~(1<<1);
		 }
		 k++; 
			if (k>65000)
			{
				k=0;
				flag |= (1<<0)|(1<<1);
			}
	 }
 } 
void Init_gpio (void)
 {
	 GPIO_InitTypeDef InitA;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	 
	 InitA.GPIO_Pin 				= GPIO_Pin_4;
	 InitA.GPIO_Mode				 = GPIO_Mode_Out_PP;
	 InitA.GPIO_Speed				 = GPIO_Speed_2MHz;
	 GPIO_Init (GPIOA, &InitA);
	 
	 InitA.GPIO_Pin 				= GPIO_Pin_14;
	 InitA.GPIO_Mode				 = GPIO_Mode_Out_PP;
	 InitA.GPIO_Speed				 = GPIO_Speed_2MHz;
	 GPIO_Init (GPIOB, &InitA);
	 
	 InitA.GPIO_Pin 				= GPIO_Pin_5;
	 InitA.GPIO_Mode				 = GPIO_Mode_IPU;
	 InitA.GPIO_Speed				 = GPIO_Speed_2MHz;
	 GPIO_Init (GPIOA, &InitA);
	 
	 InitA.GPIO_Pin 				= GPIO_Pin_13;
	 InitA.GPIO_Mode				 = GPIO_Mode_IPU;
	 InitA.GPIO_Speed				 = GPIO_Speed_2MHz;
	 GPIO_Init (GPIOB, &InitA);
 } 
 void Init_timer (void)
 {
//	 TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	 GPIOA->ODR |= (1<<4);
//	 TIM_TimeBaseStructInit (&TIMER_InitStructure);
//	 TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	 TIMER_InitStructure.TIM_Prescaler = 8000;
//	 TIMER_InitStructure.TIM_Period = 500;
//	 TIM_TimeBaseInit (TIM4, &TIMER_InitStructure);
//	 TIM_ITConfig (TIM4, TIM_IT_Update, ENABLE);
//	 
//	 NVIC_EnableIRQ (TIM4_IRQn);
//	 TIM_Cmd (TIM4, ENABLE);
//	 GPIOA->ODR &=~(1<<4);
//	 GPIOA->ODR |= (1<<4);
	 TIM4->ARR = 500;
	 TIM4->DIER |= TIM_DIER_UIE;
	 TIM4->PSC = 16000-1;
	 TIM4->CR1 |= TIM_CR1_CEN;
	 TIM4->SR = 0x00;//0xE1A0;
	 NVIC_EnableIRQ (TIM4_IRQn);
	 
 } 
 void Init_nvic (void)
 {
	 NVIC_InitTypeDef NVIC_InitStructure;
	 GPIOA->ODR |= (1<<4);
	 NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init (&NVIC_InitStructure);
	 GPIOA->ODR &=~(1<<4);
	 NVIC_EnableIRQ (TIM4_IRQn);
	 
	 
//	 NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	 NVIC_Init (&NVIC_InitStructure);
//	 NVIC_EnableIRQ (EXTI9_5_IRQn);
 }
//void InitEXTI (void)
// {
//	 EXTI_InitTypeDef EXTI_InitStructure;
//	 EXTI_StructInit (&EXTI_InitStructure);
//	 EXTI_InitStructure.EXTI_Line = EXTI_Line5;
//	 EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//	 
//	 EXTI_Init (&EXTI_InitStructure);
// }
