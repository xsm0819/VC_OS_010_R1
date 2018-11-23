#include "GSM.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "Device.h"
#include "stdio.h"
#include "string.h"

unsigned char SerialGSM_Counter, SerialGSM_Buff[100], TypGSM_Ans = 0, GSM_ANS = 0;

void InitUART_GSM	(unsigned long BRR)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Configure the GPIOs */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure the USART1 */
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = BRR;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void GSM_Send(char chr)
{
  while(!(USART2->SR & USART_SR_TC));
  USART2->DR = chr;
  USART1->DR = chr;
}

void GSM_Send_String(char* str)
{
  int i=0;
  while(str[i])
	  GSM_Send(str[i++]);
}


		// Обработчик всех прерываний от USART1
void USART2_IRQHandler(void)
{
// Выясняем, какое именно событие вызвало прерывание. Если это приём байта в RxD - обрабатываем.
if (USART2->SR & USART_SR_RXNE)
	{
	// Сбрасываем флаг прерывания
	USART2->SR&=~USART_SR_RXNE;
	SerialGSM_Buff[SerialGSM_Counter] = (USART2->DR);
	SerialGSM_Counter++;
	if (GsmTermSession)
    	USART1->DR = USART2->DR;

    if (USART2->DR == '\n')
		{
    		SerialGSM_Counter = 0;
    		GSM_Anslaver();
    	}
	}
}

unsigned char GSM_Anslaver (void)
{
	TypGSM_Ans = 0;
	unsigned char n = 0;
	if (strstr(SerialGSM_Buff, OK) != 0)
	{
		Terminal_Send_String("ANS OK\n");
		TypGSM_Ans = 1;
		GSM_ANS = 1;
	}
	else if (strstr(SerialGSM_Buff, ERROR_code) != 0)
	{
		Terminal_Send_String("ANS ERR\n");
		TypGSM_Ans = 1;
		GSM_ANS = 0;
	}
	else if (strstr (SerialGSM_Buff, "+CSQ:") != 0)
	{
		TypGSM_Ans = 2;
		Terminal_Send_String (SerialGSM_Buff);
		LevelSignal();
	}
	else if (strstr(SerialGSM_Buff, InputCall) != 0)
	{
		TypGSM_Ans = 3;
				Terminal_Send_String (SerialGSM_Buff);
				InputCalling();

	}
	n=0;
	while (n<100)
	{
		n++;
		SerialGSM_Buff[n] = 0;
	}

}


StatusBar State;
void LevelSignal (void)
{
	unsigned char Level = 0, m = 0, n = 0;
		n = 0;
		while ((SerialGSM_Buff[n] < '0')||(SerialGSM_Buff[n] > '9'))
		{
			n++;
		}
		if (SerialGSM_Buff[n+1]!=',')
		{
			Level = ((SerialGSM_Buff[n+1]-0x30)+(SerialGSM_Buff[n]-0x30)*10);
		}
		if (Level == 99)	 State.connectTyp = Signal0;
		else if (Level < 6)  State.connectTyp = Signal1;
		else if (Level < 10)  State.connectTyp = Signal2;
		else if (Level < 15)  State.connectTyp = Signal3;
		else State.connectTyp = Signal4;
}

void InputCalling (void)
{
	GSM_Send_String("ATA\n");
}


unsigned char GSMErrorCount = 0;
void InitGSM (void)
{
	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_Null);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}

	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_EhoDisable);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}
	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(Autoanswer);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_AutoDetectionNumb);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}
	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_SpecialSMS_Disable);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}
	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_SMS_TextModeGSM);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}

	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_SMS_TextMode);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}

	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_LevelSignal);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}

	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_MaxVolume);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}

	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_MaxVolumeMic);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}

	GSM_ANS = 0;
	TypGSM_Ans = 0;
	GSMErrorCount = 0;
	while (!GSM_ANS)
	{
		GSMErrorCount++;
		GSM_Send_String(CMD_DTMF_Enable);
		while (!TypGSM_Ans);
		if (GSMErrorCount > 20)
			GSM_ANS = 1;
	}
}
