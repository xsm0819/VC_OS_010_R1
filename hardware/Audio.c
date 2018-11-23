
#include "Audio.h"
#include "Device.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x.h"
#include "misc.h"

#define DMA_BUFF_SIZE 64//Размер буфера

static volatile uint16_t AudioA[DMA_BUFF_SIZE], AudioB[DMA_BUFF_SIZE];

unsigned int f_Discret =  10000;


void DAC_Setup (void);
void ADC_Setup (uint32_t ADC_Num, char CH);
void Timer1_Init  (uint32_t Period);

void InitAudioMP 	(void)
{
	DAC_Setup();
	ADC_Setup (ADC1, 6);
	Timer1_Init(16000);
	TIM_Cmd(TIM1, ENABLE);
}

void InitAudioGSM 	(void)
{

}

void InitAudioCall 	(void)
{

}

void DAC_Setup (void)
{
	DAC->CR      |= DAC_CR_EN1;             // включить канал №1
	DAC->CR      |= DAC_CR_EN2;             // включить канал №2


}

ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
TIM_TimeBaseInitTypeDef TIMER_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

void ADC_Setup (uint32_t ADC_Num, char CH)
{
	 // настройки ADC
	 ADC_StructInit(&ADC_InitStructure);
	 ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // режим работы - одиночный, независимый
	 ADC_InitStructure.ADC_ScanConvMode = DISABLE; // не сканировать каналы, просто измерить один канал
	 ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // однократное измерение
	 ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // без внешнего триггера
	 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left; //выравнивание битов результат - прижать вправо
	 ADC_InitStructure.ADC_NbrOfChannel = 1; //количество каналов - одна штука
	 ADC_Init(ADC1, &ADC_InitStructure);
	 ADC_Cmd(ADC1, ENABLE);

	 // настройка канала
	 ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);

	 // калибровка АЦП
	 ADC_ResetCalibration(ADC1);
	 while (ADC_GetResetCalibrationStatus(ADC1));
	 ADC_StartCalibration(ADC1);
	 while (ADC_GetCalibrationStatus(ADC1));

	 // настройки ADC
	 ADC_StructInit(&ADC_InitStructure);
	 ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // режим работы - одиночный, независимый
	 ADC_InitStructure.ADC_ScanConvMode = DISABLE; // не сканировать каналы, просто измерить один канал
	 ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // однократное измерение
	 ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // без внешнего триггера
	 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left; //выравнивание битов результат - прижать вправо
	 ADC_InitStructure.ADC_NbrOfChannel = 1; //количество каналов - одна штука
	 ADC_Init(ADC2, &ADC_InitStructure);
	 ADC_Cmd(ADC2, ENABLE);

	 // настройка канала
	 ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5);

	 // калибровка АЦП
	 ADC_ResetCalibration(ADC2);
	 while (ADC_GetResetCalibrationStatus(ADC2));
	 ADC_StartCalibration(ADC2);
	 while (ADC_GetCalibrationStatus(ADC2));

}

void InitTimer (void)
{
    TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 72;
    TIMER_InitStructure.TIM_Period = 100;
    TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

    /* NVIC Configuration */
    /* Enable the TIM4_IRQn Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void Timer1_Init  (uint32_t Period)
{
	uint32_t ARR = 0xFFFF, PCSR = 1;
	//Tim = (1/Period * 1000000);

	ARR = F_APB2/Period;
		while (ARR > 0xFFFF)
		{
			PCSR++;
			ARR = (F_APB2/Period)/PCSR;
		}

		TIM_TimeBaseInitTypeDef TIMER_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

		    TIM_TimeBaseStructInit(&TIMER_InitStructure);
		    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		    TIMER_InitStructure.TIM_Prescaler = PCSR;
		    TIMER_InitStructure.TIM_Period = ARR;
		    TIM_TimeBaseInit(TIM1, &TIMER_InitStructure);
		    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
		    TIM_Cmd(TIM1, DISABLE);


		    /* NVIC Configuration */
		    /* Enable the TIM4_IRQn Interrupt */
		    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
		    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		    NVIC_Init(&NVIC_InitStructure);

}


unsigned int print;
void TIM1_UP_IRQHandler(void)
{
        if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
        {
            TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
            ADC_Cmd(ADC1,ENABLE);//Включаем преобразование АЦП
            ADC_Cmd(ADC2,ENABLE);//Включаем преобразование АЦП
            Call_Send((ADC2->DR)>>4);
            GSM_SPK_Data = (USART3->DR);
        }
}



//=================================================================================

