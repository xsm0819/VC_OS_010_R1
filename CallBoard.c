
#include "CallBoard.h"
#include "Device.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

void InitCall485	(unsigned long BRR)
{
    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    /* NVIC Configuration */
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure the GPIOs */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure the USART1 */
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = BRR;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);

    /* Enable USART1 */
    USART_Cmd(USART3, ENABLE);

		/* Enable the USART1 Receive interrupt: this interrupt is generated when the
			USART1 receive data register is not empty */
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
}

void Call_Send(char chr)
{
	DIRECT_call(1);
  while(!(USART3->SR & USART_SR_TC));
  //USART1->DR = chr;
  USART3->DR = chr;
  while(!(USART3->SR & USART_SR_TC));
  DIRECT_call(0);
}

void Call_Send_String(char* str) {
  int i=0;
  while(str[i])
    USART3_Send(str[i++]);
}

char MULL = 0;
uint32_t Prop = 255;
	// Обработчик всех прерываний от USART1	`
	void USART3_IRQHandler(void)
	{
	// Выясняем, какое именно событие вызвало прерывание. Если это приём байта в RxD - обрабатываем.
	if (USART3->SR & USART_SR_RXNE)
		{
		USART3->SR&=~USART_SR_RXNE;
		}
	}
